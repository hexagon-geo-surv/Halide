#include "Simplify_Internal.h"

#include "ExprUsesVar.h"
#include "IRMutator.h"
#include "Substitute.h"

namespace Halide {
namespace Internal {

using std::pair;
using std::string;
using std::vector;

Stmt Simplify::visit(const IfThenElse *op) {
    Expr condition = mutate(op->condition, nullptr);
    if (in_unreachable) {
        return op;
    }

    // Remove tags
    Expr unwrapped_condition = unwrap_tags(condition);

    // If (true) ...
    if (is_const_one(unwrapped_condition)) {
        return mutate(op->then_case);
    }

    // If (false) ...
    if (is_const_zero(unwrapped_condition)) {
        if (op->else_case.defined()) {
            return mutate(op->else_case);
        } else {
            return Evaluate::make(0);
        }
    }

    Stmt then_case, else_case;
    {
        auto f = scoped_truth(unwrapped_condition);
        then_case = mutate(op->then_case);
        Stmt learned_then_case = f.substitute_facts(then_case);
        if (!learned_then_case.same_as(then_case)) {
            then_case = mutate(learned_then_case);
        }
    }
    bool then_unreachable = in_unreachable;
    in_unreachable = false;

    {
        auto f = scoped_falsehood(unwrapped_condition);
        else_case = mutate(op->else_case);
        Stmt learned_else_case = f.substitute_facts(else_case);
        if (!learned_else_case.same_as(else_case)) {
            else_case = mutate(learned_else_case);
        }
    }
    bool else_unreachable = in_unreachable;

    if (then_unreachable && else_unreachable) {
        return then_case;
    }
    in_unreachable = false;
    if (else_unreachable) {
        return then_case;
    } else if (then_unreachable) {
        if (else_case.defined()) {
            return else_case;
        } else {
            return Evaluate::make(0);
        }
    }

    if (is_no_op(else_case)) {
        // If both sides are no-ops, bail out.
        if (is_pure(condition) && is_no_op(then_case)) {
            return then_case;
        }
        // Replace no-ops with empty stmts.
        else_case = Stmt();
    }

    // Pull out common nodes, but only when the "late in lowering" flag is set. This
    // avoids simplifying specializations before they have a chance to specialize.
    if (remove_dead_code && equal(then_case, else_case)) {
        return then_case;
    }
    const Acquire *then_acquire = then_case.as<Acquire>();
    const Acquire *else_acquire = else_case.as<Acquire>();
    const ProducerConsumer *then_pc = then_case.as<ProducerConsumer>();
    const ProducerConsumer *else_pc = else_case.as<ProducerConsumer>();
    const Block *then_block = then_case.as<Block>();
    const Block *else_block = else_case.as<Block>();
    const For *then_for = then_case.as<For>();
    const IfThenElse *then_if = then_case.as<IfThenElse>();
    const IfThenElse *else_if = else_case.as<IfThenElse>();
    if (then_acquire &&
        else_acquire &&
        equal(then_acquire->semaphore, else_acquire->semaphore) &&
        equal(then_acquire->count, else_acquire->count)) {
        // TODO: This simplification sometimes prevents useful loop partioning/no-op
        // trimming from happening, e.g. it rewrites:
        //
        //   for (x, min + -2, extent + 2) {
        //    if (x < min) {
        //     acquire (f24.semaphore_0, 1) {}
        //    } else {
        //     acquire (f24.semaphore_0, 1) { ... }
        //    }
        //   }
        //
        // This could be partitioned and simplified, but not after this simplification.
        return Acquire::make(then_acquire->semaphore, then_acquire->count,
                             mutate(IfThenElse::make(condition, then_acquire->body, else_acquire->body)));
    } else if (then_pc &&
               else_pc &&
               then_pc->name == else_pc->name &&
               then_pc->is_producer == else_pc->is_producer) {
        return ProducerConsumer::make(then_pc->name, then_pc->is_producer,
                                      mutate(IfThenElse::make(condition, then_pc->body, else_pc->body)));
    } else if (then_pc &&
               is_no_op(else_case)) {
        return ProducerConsumer::make(then_pc->name, then_pc->is_producer,
                                      mutate(IfThenElse::make(condition, then_pc->body)));
    } else if (then_block &&
               else_block &&
               equal(then_block->first, else_block->first)) {
        return Block::make(then_block->first,
                           mutate(IfThenElse::make(condition, then_block->rest, else_block->rest)));
    } else if (then_block &&
               else_block &&
               equal(then_block->rest, else_block->rest)) {
        return Block::make(mutate(IfThenElse::make(condition, then_block->first, else_block->first)),
                           then_block->rest);
    } else if (then_block && equal(then_block->first, else_case)) {
        return Block::make(else_case,
                           mutate(IfThenElse::make(condition, then_block->rest)));
    } else if (then_block && equal(then_block->rest, else_case)) {
        return Block::make(mutate(IfThenElse::make(condition, then_block->first)),
                           else_case);
    } else if (else_block && equal(then_case, else_block->first)) {
        return Block::make(then_case,
                           mutate(IfThenElse::make(condition, Evaluate::make(0), else_block->rest)));
    } else if (else_block && equal(then_case, else_block->rest)) {
        return Block::make(mutate(IfThenElse::make(condition, Evaluate::make(0), else_block->first)),
                           then_case);
    } else if (then_for &&
               !else_case.defined() &&
               equal(unwrapped_condition, 0 < then_for->extent)) {
        // This guard is redundant
        return then_case;
    } else if (then_if &&
               else_if &&
               !then_if->else_case.defined() &&
               !else_if->else_case.defined() &&
               is_pure(condition) &&
               is_pure(then_if->condition) &&
               is_pure(else_if->condition) &&
               equal(then_if->condition, else_if->condition)) {
        // Rewrite if(a) { if(b) X } else { if(b) Y }
        // to if(b) { if(a) X else Y }
        return mutate(IfThenElse::make(then_if->condition,
                                       IfThenElse::make(condition, then_if->then_case, else_if->then_case)));
    } else if (condition.same_as(op->condition) &&
               then_case.same_as(op->then_case) &&
               else_case.same_as(op->else_case)) {
        return op;
    } else {
        return IfThenElse::make(condition, then_case, else_case);
    }
}

Stmt Simplify::visit(const AssertStmt *op) {
    Expr cond = mutate(op->condition, nullptr);

    // The message is only evaluated when the condition is false
    Expr message;
    {
        auto f = scoped_falsehood(cond);
        message = mutate(op->message, nullptr);
    }

    if (is_const_zero(cond)) {
        // Usually, assert(const-false) should generate a warning;
        // in at least one case (specialize_fail()), we want to suppress
        // the warning, because the assertion is generated internally
        // by Halide and is expected to always fail.
        const Call *call = message.as<Call>();
        const bool const_false_conditions_expected =
            call && call->name == "halide_error_specialize_fail";
        if (!const_false_conditions_expected) {
            user_warning << "This pipeline is guaranteed to fail an assertion at runtime: \n"
                         << message << "\n";
        }
    } else if (is_const_one(cond)) {
        return Evaluate::make(0);
    }

    if (cond.same_as(op->condition) && message.same_as(op->message)) {
        return op;
    } else {
        return AssertStmt::make(cond, message);
    }
}

Stmt Simplify::visit(const For *op) {
    ExprInfo min_info, extent_info;
    Expr new_min = mutate(op->min, &min_info);
    if (in_unreachable) {
        return Evaluate::make(new_min);
    }
    Expr new_extent = mutate(op->extent, &extent_info);
    if (in_unreachable) {
        return Evaluate::make(new_extent);
    }

    ScopedValue<bool> old_in_vector_loop(in_vector_loop,
                                         (in_vector_loop ||
                                          op->for_type == ForType::Vectorized));

    Expr extent_positive = mutate(0 < new_extent, nullptr);
    if (is_const_zero(extent_positive)) {
        // This loop never runs
        return Evaluate::make(0);
    }

    ExprInfo loop_var_info;
    // Deduce bounds for the loop var that are true for any code than runs
    // inside the loop body. Code in the inner loop only runs if the extent is
    // at least one, so we can throw a max around the extent bounds.

    loop_var_info.bounds =
        ConstantInterval::make_union(min_info.bounds,
                                     min_info.bounds + max(extent_info.bounds, 1) - 1);
    Stmt new_body;
    {
        ScopedBinding<ExprInfo> bind_if((loop_var_info.bounds.max_defined ||
                                         loop_var_info.bounds.min_defined),
                                        bounds_and_alignment_info,
                                        op->name,
                                        loop_var_info);

        // If we're in the loop, the extent must be greater than 0.
        ScopedFact fact_extent_positive = scoped_truth(extent_positive);

        // The loop variable will never exceed the loop bound.
        Expr loop_var = Variable::make(Int(32), op->name);
        Expr new_max = mutate(new_min + new_extent, nullptr);
        ScopedFact fact_loop_var_less_than_extent = scoped_truth(loop_var < new_max);

        ScopedFact fact_loop_var_ge_than_min = scoped_truth(new_min <= loop_var);

        new_body = mutate(op->body);
    }

    if (in_unreachable) {
        // We found that the body of this loop is unreachable when recursively
        // mutating it, so we can remove the loop. Additionally, if we know the
        // extent is greater than zero, then the code *outside* the loop must be
        // unreachable too, because if it weren't, it'd run the unreachable body
        // at least once.
        in_unreachable = extent_info.bounds > 0;
        return Evaluate::make(0);
    }

    if (const Acquire *acquire = new_body.as<Acquire>()) {
        if (is_no_op(acquire->body)) {
            // Rewrite iterated no-op acquires as a single acquire.
            return Acquire::make(acquire->semaphore, mutate(acquire->count * new_extent, nullptr), acquire->body);
        }
    }

    if (is_no_op(new_body)) {
        return new_body;
    } else if (extent_info.bounds <= 0) {
        return Evaluate::make(0);
    } else if (extent_info.bounds <= 1 &&
               op->device_api == DeviceAPI::None) {
        // Loop body runs at most once
        Stmt s = LetStmt::make(op->name, new_min, new_body);
        if (extent_info.bounds.contains(0)) {
            // Loop body might not run at all
            s = IfThenElse::make(0 < new_extent, s);
        }
        return mutate(s);
    } else if (!stmt_uses_var(new_body, op->name) && !is_const_zero(op->min)) {
        return For::make(op->name, make_zero(Int(32)), new_extent, op->for_type, op->partition_policy, op->device_api, new_body);
    } else if (op->min.same_as(new_min) &&
               op->extent.same_as(new_extent) &&
               op->body.same_as(new_body)) {
        return op;
    } else {
        return For::make(op->name, new_min, new_extent, op->for_type, op->partition_policy, op->device_api, new_body);
    }
}

Stmt Simplify::visit(const Provide *op) {
    found_buffer_reference(op->name, op->args.size());

    // Mutate the args
    auto [new_args, changed_args] = mutate_with_changes(op->args);
    auto [new_values, changed_values] = mutate_with_changes(op->values);
    Expr new_predicate = mutate(op->predicate, nullptr);

    if (!(changed_args || changed_values) && new_predicate.same_as(op->predicate)) {
        return op;
    } else {
        return Provide::make(op->name, new_values, new_args, new_predicate);
    }
}

Stmt Simplify::visit(const Store *op) {
    found_buffer_reference(op->name);

    Expr predicate = mutate(op->predicate, nullptr);
    Expr value = mutate(op->value, nullptr);

    ExprInfo index_info;
    Expr index = mutate(op->index, &index_info);

    // If the store is fully unconditional and out of bounds, drop it.
    // This should only occur inside branches that make the store unreachable,
    // but perhaps the branch was hard to prove constant true or false. This
    // provides an alternative mechanism to simplify these unreachable stores.
    string alloc_extent_name = op->name + ".total_extent_bytes";
    if (is_const_one(op->predicate)) {
        if (const auto *alloc_info = bounds_and_alignment_info.find(alloc_extent_name)) {
            if (index_info.bounds < 0 ||
                index_info.bounds * op->value.type().bytes() > alloc_info->bounds) {
                in_unreachable = true;
                return Evaluate::make(unreachable());
            }
        }
    }

    ExprInfo base_info;
    if (const Ramp *r = index.as<Ramp>()) {
        mutate(r->base, &base_info);
    }
    base_info.alignment = ModulusRemainder::intersect(base_info.alignment, index_info.alignment);

    const Load *load = value.as<Load>();
    const Broadcast *scalar_pred = predicate.as<Broadcast>();
    if (scalar_pred && !scalar_pred->value.type().is_scalar()) {
        // Nested vectorization
        scalar_pred = nullptr;
    }

    ModulusRemainder align = ModulusRemainder::intersect(op->alignment, base_info.alignment);

    if (is_const_zero(predicate)) {
        // Predicate is always false
        return Evaluate::make(0);
    } else if (scalar_pred && !is_const_one(scalar_pred->value)) {
        return IfThenElse::make(scalar_pred->value,
                                Store::make(op->name, value, index, op->param, const_true(value.type().lanes(), nullptr), align));
    } else if (is_undef(value) || (load && load->name == op->name && equal(load->index, index))) {
        // foo[x] = foo[x] or foo[x] = undef is a no-op
        return Evaluate::make(0);
    } else if (predicate.same_as(op->predicate) && value.same_as(op->value) && index.same_as(op->index) && align == op->alignment) {
        return op;
    } else {
        return Store::make(op->name, value, index, op->param, predicate, align);
    }
}

Stmt Simplify::visit(const Allocate *op) {
    std::vector<Expr> new_extents;
    bool all_extents_unmodified = true;
    ExprInfo total_extent_info;
    total_extent_info.bounds = ConstantInterval::single_point(op->type.bytes());
    for (size_t i = 0; i < op->extents.size(); i++) {
        ExprInfo extent_info;
        new_extents.push_back(mutate(op->extents[i], &extent_info));
        all_extents_unmodified &= new_extents[i].same_as(op->extents[i]);
        total_extent_info.bounds *= extent_info.bounds;
    }
    total_extent_info.bounds -= 1;

    ScopedBinding<ExprInfo> b(bounds_and_alignment_info, op->name + ".total_extent_bytes", total_extent_info);

    Stmt body = mutate(op->body);
    Expr condition = mutate(op->condition, nullptr);
    Expr new_expr;
    if (op->new_expr.defined()) {
        new_expr = mutate(op->new_expr, nullptr);
    }
    const IfThenElse *body_if = body.as<IfThenElse>();
    if (body_if &&
        op->condition.defined() &&
        equal(op->condition, body_if->condition)) {
        // We can move the allocation into the if body case. The
        // else case must not use it.
        Stmt stmt = Allocate::make(op->name, op->type, op->memory_type,
                                   new_extents, condition, body_if->then_case,
                                   new_expr, op->free_function, op->padding);
        return IfThenElse::make(body_if->condition, stmt, body_if->else_case);
    } else if (all_extents_unmodified &&
               body.same_as(op->body) &&
               condition.same_as(op->condition) &&
               new_expr.same_as(op->new_expr)) {
        return op;
    } else {
        return Allocate::make(op->name, op->type, op->memory_type,
                              new_extents, condition, body,
                              new_expr, op->free_function, op->padding);
    }
}

Stmt Simplify::visit(const Evaluate *op) {
    Expr value = mutate(op->value, nullptr);

    // Rewrite Lets inside an evaluate as LetStmts outside the Evaluate.
    vector<pair<string, Expr>> lets;
    while (const Let *let = value.as<Let>()) {
        lets.emplace_back(let->name, let->value);
        value = let->body;
    }

    if (value.same_as(op->value)) {
        internal_assert(lets.empty());
        return op;
    } else {
        // Rewrap the lets outside the evaluate node
        Stmt stmt = Evaluate::make(value);
        for (const auto &[var, value] : reverse_view(lets)) {
            stmt = LetStmt::make(var, value, stmt);
        }
        return stmt;
    }
}

Stmt Simplify::visit(const ProducerConsumer *op) {
    Stmt body = mutate(op->body);

    if (is_no_op(body)) {
        return Evaluate::make(0);
    } else if (body.same_as(op->body)) {
        return op;
    } else {
        return ProducerConsumer::make(op->name, op->is_producer, body);
    }
}

Stmt Simplify::visit(const Block *op) {
    Stmt first = mutate(op->first);
    Stmt rest = op->rest;

    if (const AssertStmt *first_assert = first.as<AssertStmt>()) {
        bool unchanged = first.same_as(op->first);

        // Handle an entire sequence of asserts here to avoid a deeply
        // nested stack.  We won't be popping any knowledge until
        // after the end of this chain of asserts, so we can use a
        // single ScopedFact and progressively add knowledge to it.
        ScopedFact knowledge(this);
        vector<Stmt> result;
        result.push_back(first);
        knowledge.learn_true(first_assert->condition);

        // Loop invariants: 'first' has already been mutated and is in
        // the result list. 'first' was an AssertStmt before it was
        // mutated, and its condition has been captured in
        // 'knowledge'. 'rest' has not been mutated and is not in the
        // result list.
        const Block *rest_block;
        while ((rest_block = rest.as<Block>()) &&
               (first_assert = rest_block->first.as<AssertStmt>())) {
            first = mutate(first_assert);
            unchanged &= first.same_as(first_assert);
            rest = rest_block->rest;
            result.push_back(first);
            if ((first_assert = first.as<AssertStmt>())) {
                // If it didn't fold away to trivially true or false,
                // learn the condition.
                knowledge.learn_true(first_assert->condition);
            }
        }

        Stmt new_rest = mutate(rest);
        Stmt learned_new_rest = knowledge.substitute_facts(new_rest);
        if (!learned_new_rest.same_as(new_rest)) {
            new_rest = mutate(learned_new_rest);
        }
        unchanged &= new_rest.same_as(rest);

        if (unchanged) {
            return op;
        } else {
            result.push_back(new_rest);
            return Block::make(result);
        }

    } else {
        rest = mutate(op->rest);
    }

    // Check if both halves start with a let statement.
    const LetStmt *let_first = first.as<LetStmt>();
    const LetStmt *let_rest = rest.as<LetStmt>();
    const Block *block_rest = rest.as<Block>();
    const IfThenElse *if_first = first.as<IfThenElse>();
    const IfThenElse *if_next = block_rest ? block_rest->first.as<IfThenElse>() : rest.as<IfThenElse>();
    Stmt if_rest = block_rest ? block_rest->rest : Stmt();

    const Store *store_first = first.as<Store>();
    const Store *store_next = block_rest ? block_rest->first.as<Store>() : rest.as<Store>();

    if (is_no_op(first) &&
        is_no_op(rest)) {
        return Evaluate::make(0);
    } else if (is_no_op(first)) {
        return rest;
    } else if (is_no_op(rest)) {
        return first;
    } else if (let_first &&
               let_rest &&
               equal(let_first->value, let_rest->value) &&
               is_pure(let_first->value)) {

        // Do both first and rest start with the same let statement (occurs when unrolling).
        Stmt new_block = mutate(Block::make(let_first->body, let_rest->body));

        // We need to make a new name since we're pulling it out to a
        // different scope.
        string var_name = unique_name('t');
        Expr new_var = Variable::make(let_first->value.type(), var_name);
        new_block = substitute(let_first->name, new_var, new_block);
        new_block = substitute(let_rest->name, new_var, new_block);

        return LetStmt::make(var_name, let_first->value, new_block);
    } else if (store_first &&
               store_next &&
               store_first->name == store_next->name &&
               equal(store_first->index, store_next->index) &&
               equal(store_first->predicate, store_next->predicate) &&
               is_pure(store_first->index) &&
               is_pure(store_first->value) &&
               is_pure(store_first->predicate) &&
               !expr_uses_var(store_next->index, store_next->name) &&
               !expr_uses_var(store_next->value, store_next->name) &&
               !expr_uses_var(store_next->predicate, store_next->name)) {
        // Second store clobbers first
        if (block_rest) {
            return Block::make(store_next, block_rest->rest);
        } else {
            return store_next;
        }
    } else if (if_first &&
               if_next &&
               equal(if_first->condition, if_next->condition) &&
               is_pure(if_first->condition)) {
        // Two ifs with matching conditions.
        Stmt then_case = Block::make(if_first->then_case, if_next->then_case);
        Stmt else_case;
        if (if_first->else_case.defined() && if_next->else_case.defined()) {
            else_case = Block::make(if_first->else_case, if_next->else_case);
        } else if (if_first->else_case.defined()) {
            // We already simplified the body of the ifs.
            else_case = if_first->else_case;
        } else {
            else_case = if_next->else_case;
        }
        Stmt result = IfThenElse::make(if_first->condition, then_case, else_case);
        if (if_rest.defined()) {
            result = Block::make(result, if_rest);
        }
        // We must mutate the entire IfThenElse block without first mutating the
        // branches to compute reachability accurately.
        return mutate(result);
    } else if (if_first &&
               if_next &&
               !if_next->else_case.defined() &&
               is_pure(if_first->condition) &&
               is_pure(if_next->condition) &&
               is_const_one(mutate((if_first->condition && if_next->condition) == if_next->condition, nullptr))) {
        // Two ifs where the second condition is tighter than
        // the first condition.  The second if can be nested
        // inside the first one, because if it's true the
        // first one must also be true.
        Stmt then_case = Block::make(if_first->then_case, if_next);
        Stmt else_case = if_first->else_case;
        Stmt result = IfThenElse::make(if_first->condition, then_case, else_case);
        if (if_rest.defined()) {
            result = Block::make(result, if_rest);
        }
        // As above, we must mutate the entire IfThenElse block without first
        // mutating the branches to compute reachability accurately.
        return mutate(result);
    } else if (if_first &&
               if_next &&
               !if_next->else_case.defined() &&
               is_pure(if_first->condition) &&
               is_pure(if_next->condition) &&
               is_const_one(mutate(!(if_first->condition && if_next->condition), nullptr))) {
        // Two ifs where the first condition being true implies the
        // second is false.  The second if can be nested inside the
        // else case of the first one, turning a block of if
        // statements into an if-else chain.
        Stmt then_case = if_first->then_case;
        Stmt else_case = if_next;
        if (if_first->else_case.defined()) {
            else_case = Block::make(if_first->else_case, else_case);
        }
        Stmt result = IfThenElse::make(if_first->condition, then_case, else_case);
        if (if_rest.defined()) {
            result = Block::make(result, if_rest);
        }
        return mutate(result);
    } else if (op->first.same_as(first) &&
               op->rest.same_as(rest)) {
        return op;
    } else {
        return Block::make(first, rest);
    }
}

Stmt Simplify::visit(const Realize *op) {
    // Mutate the bounds
    auto [new_bounds, bounds_changed] = mutate_region(this, op->bounds, nullptr);

    Stmt body = mutate(op->body);
    Expr condition = mutate(op->condition, nullptr);
    if (!bounds_changed &&
        body.same_as(op->body) &&
        condition.same_as(op->condition)) {
        return op;
    }
    return Realize::make(op->name, op->types, op->memory_type, new_bounds,
                         std::move(condition), std::move(body));
}

Stmt Simplify::visit(const Prefetch *op) {
    Stmt body = mutate(op->body);
    Expr condition = mutate(op->condition, nullptr);

    if (is_const_zero(op->condition)) {
        // Predicate is always false
        return body;
    }

    // Mutate the bounds
    auto [new_bounds, bounds_changed] = mutate_region(this, op->bounds, nullptr);

    if (!bounds_changed &&
        body.same_as(op->body) &&
        condition.same_as(op->condition)) {
        return op;
    } else {
        return Prefetch::make(op->name, op->types, new_bounds, op->prefetch, std::move(condition), std::move(body));
    }
}

Stmt Simplify::visit(const Free *op) {
    return op;
}

Stmt Simplify::visit(const Acquire *op) {
    Expr sema = mutate(op->semaphore, nullptr);
    Expr count = mutate(op->count, nullptr);
    Stmt body = mutate(op->body);
    if (sema.same_as(op->semaphore) &&
        body.same_as(op->body) &&
        count.same_as(op->count)) {
        return op;
    } else {
        return Acquire::make(std::move(sema), std::move(count), std::move(body));
    }
}

Stmt Simplify::visit(const Fork *op) {
    Stmt first = mutate(op->first);
    Stmt rest = mutate(op->rest);
    if (is_no_op(first)) {
        return rest;
    } else if (is_no_op(rest)) {
        return first;
    } else if (op->first.same_as(first) &&
               op->rest.same_as(rest)) {
        return op;
    } else {
        return Fork::make(first, rest);
    }
}

Stmt Simplify::visit(const Atomic *op) {
    Stmt body = mutate(op->body);
    if (is_no_op(body)) {
        return Evaluate::make(0);
    } else if (body.same_as(op->body)) {
        return op;
    } else {
        return Atomic::make(op->producer_name,
                            op->mutex_name,
                            std::move(body));
    }
}

Stmt Simplify::visit(const HoistedStorage *op) {
    Stmt body = mutate(op->body);
    if (body.same_as(op->body)) {
        return op;
    } else {
        return HoistedStorage::make(op->name, body);
    }
}

}  // namespace Internal
}  // namespace Halide
