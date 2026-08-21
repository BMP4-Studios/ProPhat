#pragma once

// Function-effect attributes consumed by Clang's RealtimeSanitizer (RTSan),
// upstreamed in LLVM 20. Annotating a function as RTSAN_NONBLOCKING tells
// RTSan that the body and its transitive callees must not block (no allocs,
// locks, syscalls, etc). RTSAN_BLOCKING marks an explicit boundary where
// blocking is allowed and analysis should stop descending.
//
// Older Clang (and non-Clang compilers) don't recognize the attributes; we
// expand to nothing there so the code still compiles and IDE noise is reduced.

// NOLINTBEGIN
#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(clang::nonblocking)
#define RTSAN_NONBLOCKING [[clang::nonblocking]]
#endif
#if __has_cpp_attribute(clang::blocking)
#define RTSAN_BLOCKING [[clang::blocking]]
#endif
#endif

#ifndef RTSAN_NONBLOCKING
#define RTSAN_NONBLOCKING
#endif

#ifndef RTSAN_BLOCKING
#define RTSAN_BLOCKING
#endif
// NOLINTEND
