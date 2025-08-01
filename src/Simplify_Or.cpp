#include "Simplify_Internal.h"

namespace Halide {
namespace Internal {

Expr Simplify::visit(const Or *op, ExprInfo *info) {
    if (truths.count(op)) {
        return const_true(op->type.lanes(), info);
    }

    Expr a = mutate(op->a, nullptr);
    Expr b = mutate(op->b, nullptr);

    if (should_commute(a, b)) {
        std::swap(a, b);
    }

    if (info) {
        info->cast_to(op->type);
    }

    auto rewrite = IRMatcher::rewriter(IRMatcher::or_op(a, b), op->type);

    // clang-format off

    // Cases that fold to a constant
    if (EVAL_IN_LAMBDA
        (rewrite(x || true, true) ||
         rewrite(x != y || x == y, true) ||
         rewrite(x != y || y == x, true) ||
         rewrite((z || x != y) || x == y, true) ||
         rewrite((z || x != y) || y == x, true) ||
         rewrite((x != y || z) || x == y, true) ||
         rewrite((x != y || z) || y == x, true) ||
         rewrite((z || x == y) || x != y, true) ||
         rewrite((z || x == y) || y != x, true) ||
         rewrite((x == y || z) || x != y, true) ||
         rewrite((x == y || z) || y != x, true) ||
         rewrite(x || !x, true) ||
         rewrite(!x || x, true) ||
         rewrite(y <= x || x < y, true) ||
         rewrite(x <= c0 || c1 <= x, true, !is_float(x) && c1 <= c0 + 1) ||
         rewrite(c1 <= x || x <= c0, true, !is_float(x) && c1 <= c0 + 1) ||
         rewrite(x <= c0 || c1 < x, true, c1 <= c0) ||
         rewrite(c1 <= x || x < c0, true, c1 <= c0) ||
         rewrite(x < c0 || c1 < x, true, c1 < c0) ||
         rewrite(c1 < x || x < c0, true, c1 < c0))) {
        set_expr_info_to_constant(info, true);
        return rewrite.result;
    }

    // Cases that fold to one of the args
    if (EVAL_IN_LAMBDA
        (rewrite(x || false, a) ||
         rewrite(x || x, a) ||

         rewrite((x || y) || x, a) ||
         rewrite(x || (x || y), b) ||
         rewrite((x || y) || y, a) ||
         rewrite(y || (x || y), b) ||

         rewrite(((x || y) || z) || x, a) ||
         rewrite(x || ((x || y) || z), b) ||
         rewrite((z || (x || y)) || x, a) ||
         rewrite(x || (z || (x || y)), b) ||
         rewrite(((x || y) || z) || y, a) ||
         rewrite(y || ((x || y) || z), b) ||
         rewrite((z || (x || y)) || y, a) ||
         rewrite(y || (z || (x || y)), b) ||

         rewrite((x && y) || x, b) ||
         rewrite(x || (x && y), a) ||
         rewrite((x && y) || y, b) ||
         rewrite(y || (x && y), a) ||

         rewrite(x != c0 || x == c1, a, c0 != c1) ||
         rewrite(c0 < x || c1 < x, fold(min(c0, c1)) < x) ||
         rewrite(c0 <= x || c1 <= x, fold(min(c0, c1)) <= x) ||
         rewrite(x < c0 || x < c1, x < fold(max(c0, c1))) ||
         rewrite(x <= c0 || x <= c1, x <= fold(max(c0, c1))))) {
        return rewrite.result;
    }
    // clang-format on

    // Cases that need remutation
    if (rewrite(broadcast(x, c0) || broadcast(y, c0), broadcast(x || y, c0)) ||
        rewrite((x && (y || z)) || y, (x && z) || y) ||
        rewrite((x && (z || y)) || y, (x && z) || y) ||
        rewrite(y || (x && (y || z)), y || (x && z)) ||
        rewrite(y || (x && (z || y)), y || (x && z)) ||

        rewrite(((y || z) && x) || y, (z && x) || y) ||
        rewrite(((z || y) && x) || y, (z && x) || y) ||
        rewrite(y || ((y || z) && x), y || (z && x)) ||
        rewrite(y || ((z || y) && x), y || (z && x)) ||

        rewrite((x || (y && z)) || y, x || y) ||
        rewrite((x || (z && y)) || y, x || y) ||
        rewrite(y || (x || (y && z)), y || x) ||
        rewrite(y || (x || (z && y)), y || x) ||

        rewrite(((y && z) || x) || y, x || y) ||
        rewrite(((z && y) || x) || y, x || y) ||
        rewrite(y || ((y && z) || x), y || x) ||
        rewrite(y || ((z && y) || x), y || x) ||

        rewrite((x && y) || (x && z), x && (y || z)) ||
        rewrite((x && y) || (z && x), x && (y || z)) ||
        rewrite((y && x) || (x && z), x && (y || z)) ||
        rewrite((y && x) || (z && x), x && (y || z)) ||

        rewrite(x < y || x < z, x < max(y, z)) ||
        rewrite(y < x || z < x, min(y, z) < x) ||
        rewrite(x <= y || x <= z, x <= max(y, z)) ||
        rewrite(y <= x || z <= x, min(y, z) <= x)) {

        return mutate(rewrite.result, info);
    }

    if (a.same_as(op->a) &&
        b.same_as(op->b)) {
        return op;
    } else {
        return Or::make(a, b);
    }
}

}  // namespace Internal
}  // namespace Halide
