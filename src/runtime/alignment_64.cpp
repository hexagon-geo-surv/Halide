#include "runtime_internal.h"

namespace Halide::Runtime::Internal {

WEAK_INLINE int _malloc_alignment() {
    return 64;
}
}  // namespace Halide::Runtime::Internal
