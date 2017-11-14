//
// Provide efficient memory management utilities.
//

#include "memory.h"

namespace pbrt {

// Memory Allocation Functions
void *AllocAligned(size_t size) {
#if defined(PBRT_IS_WINDOWS)
    return _aligned_malloc(size, PBRT_L1_CACHE_LINE_SIZE);
#elif defined(PBRT_IS_OPENBSD) || defined(PBRT_IS_OSX) || defined(PBRT_IS_FREEBSD)
    void *ptr;
    if (posix_memalign(&ptr, PBRT_L1_CACHE_LINE_SIZE, size) != 0) ptr = nullptr;
    return ptr;
#else
    return memalign(PBRT_L1_CACHE_LINE_SIZE, size);
#endif
}

void FreeAligned(void *ptr) {
    if (!ptr) return;
#if defined(PBRT_IS_WINDOWS)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

}  // namespace pbrt