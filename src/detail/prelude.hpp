// Included first by every module unit of protobuf-m, in the global module
// fragment, before any protobuf header.
#pragma once

// On Windows protobuf's headers can reach <windows.h>, whose min/max
// function-like macros break std::numeric_limits<T>::max() in the headers that
// follow; compat.protobuf builds with the same two macros.
#if defined(_WIN32)
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#endif
