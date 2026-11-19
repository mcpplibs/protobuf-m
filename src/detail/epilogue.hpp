// Included by every module unit of protobuf-m in the global module fragment,
// after the protobuf headers.
#pragma once
#include <cstdint>
#include <google/protobuf/parse_context.h>

// On little-endian aarch64 (not MSVC) parse_context.h's VarintParse<T> calls
// VarintParseSlowArm, a `static` (internal linkage) overload set. Inline code of
// the headers (ReadVarint64, ReadVarint32 …) instantiates VarintParse<T>, and
// Clang performs those pending instantiations at the end of the translation
// unit — in the module purview, where an entity of internal linkage from the
// global module fragment is not visible: "no matching function for call to
// 'VarintParseSlowArm'". Explicit instantiation definitions here instantiate
// them in the fragment, where the overloads are found.
#if defined(__aarch64__) && defined(ABSL_IS_LITTLE_ENDIAN) && !defined(_MSC_VER)
template const char* google::protobuf::internal::VarintParse<std::uint32_t>(const char*, std::uint32_t*);
template const char* google::protobuf::internal::VarintParse<std::uint64_t>(const char*, std::uint64_t*);
#endif
