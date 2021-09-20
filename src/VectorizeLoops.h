#ifndef HALIDE_VECTORIZE_LOOPS_H
#define HALIDE_VECTORIZE_LOOPS_H

/** \file
 * Defines the lowering pass that vectorizes loops marked as such
 */

#include <map>
#include <string>

#include "Expr.h"
#include "Function.h"

namespace Halide {

struct Target;

namespace Internal {
class Function;

/** Take a statement with for loops marked for vectorization, and turn
 * them into single statements that operate on vectors. The loops in
 * question must have constant extent.
 */
Stmt vectorize_loops(const Stmt &s, const std::map<std::string, Function> &env);

}  // namespace Internal
}  // namespace Halide

#endif
