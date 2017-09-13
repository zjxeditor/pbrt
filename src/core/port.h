//
// Configure the macro definitions used for different platforms.
//

///////////////////////////////////////////////////////////////////////////
// Configure build system macro definitions.

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_PORT_H
#define PBRT_CORE_PORT_H

#if defined(_WIN32) || defined(_WIN64)
#define PBRT_IS_WINDOWS
#if defined(__MINGW32__)  // Defined for both 32 bit/64 bit MinGW
#define PBRT_IS_MINGW
#elif defined(_MSC_VER)a
#define PBRT_IS_MSVC
#endif
#elif defined(__linux__)
#define PBRT_IS_LINUX
#elif defined(__APPLE__)
#define PBRT_IS_OSX
#elif defined(__OpenBSD__)
#define PBRT_IS_OPENBSD
#elif defined(__FreeBSD__)
#define PBRT_IS_FREEBSD
#endif

#if defined(_MSC_VER) && _MSC_VER == 1900
#define PBRT_IS_MSVC2015
#endif

#if defined(_MSC_VER) && _MSC_VER == 1800
#define PBRT_IS_MSVC2013
#endif

///////////////////////////////////////////////////////////////////////////
// Configure macro definitions for features and various target-specific workarounds.

#if !defined(PBRT_IS_MSVC)
#define PBRT_HAVE_HEX_FP_CONSTANTS
#define PBRT_HAVE_BINARY_CONSTANTS
#endif

#if defined(PBRT_IS_LINUX) || defined(PBRT_IS_WINDOWS)
#define PBRT_HAVE_MALLOC_H
#endif

#ifdef PBRT_IS_LINUX
#define PBRT_HAVE_ALLOCA_H
#endif

#if defined(PBRT_IS_MSVC)
#define PBRT_THREAD_LOCAL __declspec(thread)
#else
#define PBRT_THREAD_LOCAL __thread
#endif

#ifdef PBRT_IS_MSVC2013
#define PBRT_CONSTEXPR const
#else
#define PBRT_CONSTEXPR constexpr
#endif

#if !defined(PBRT_IS_MSVC2013)
#define PBRT_HAVE_ALIGNAS
#define PBRT_HAVE_ALIGNOF
#endif

#ifdef PBRT_IS_MSVC2013
#define snprintf _snprintf
#endif

#ifndef PBRT_L1_CACHE_LINE_SIZE
#define PBRT_L1_CACHE_LINE_SIZE 64
#endif

#if defined(PBRT_IS_MSVC)
#define PBRT_FORCEINLINE __forceinline
#else
#define PBRT_FORCEINLINE __attribute__((always_inline)) inline
#endif

// Is setitimer() available (and working)?
#if !defined(PBRT_IS_WINDOWS) && !defined(__CYGWIN__)
#define PBRT_HAVE_ITIMER
#endif

#endif // PBRT_CORE_PORT_H