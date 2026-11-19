// <mwrap/protobuf/macros.hpp> — protobuf's macros (GOOGLE_PROTOBUF_VERIFY_VERSION, the GOOGLE_PROTOBUF_ARCH_*
// switches …), which a module cannot export: every public header the modules include, in their order. Include
// it BEFORE `import mcpplibs.mwrap.protobuf;` (GCC requires the includes of a translation unit to come before its
// imports). Written by tools/surface.py.
#pragma once
#include <mwrap/protobuf/prelude.hpp>
