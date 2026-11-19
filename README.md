# protobuf-m

> Protocol Buffers 35.1 (the C++ runtime), through C++23 modules.

**English** · [简体中文](README.zh.md) · [Upstream](https://github.com/protocolbuffers/protobuf) · [Releases](https://github.com/mcpplibs/protobuf-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/protobuf-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/protobuf-m/actions/workflows/ci.yml)

The upstream C++ API is unchanged: `google::protobuf::Message`, `DescriptorPool`,
`util::MessageDifferencer`, `Timestamp` are the upstream entities. The library is
`compat.protobuf` 35.1 over `compat.abseil` 20250512.1, built from the releases, and the
`codegen` feature adds protoc, built for the build machine from the same release, with
`protogen`, a build rule that generates the code of every `.proto` during the build.

## Modules

- `import mcpplibs.mwrap.protobuf;`: messages, descriptors, reflection, arenas, repeated fields, maps, text format, the well-known types; re-exports the modules below
- `import mcpplibs.mwrap.protobuf.io;`: coded and zero-copy streams, the tokenizer
- `import mcpplibs.mwrap.protobuf.util;`: JSON (`json::`), `MessageDifferencer`, `TimeUtil`, `FieldMaskUtil`, delimited messages
- `import mcpplibs.mwrap.protobuf.compiler;`: parsing `.proto` files at run time (`Importer`, `Parser`)
- `import mcpplibs.mwrap.protobuf.io.gzip;`: `GzipInputStream`, `GzipOutputStream` (feature `gzip`)

> More Modules API -> [Usage](docs/usage.md) · [every exported name](docs/api.md)

## Getting started

```sh
mcpp new people --template mcpplibs.mwrap.protobuf@35.1
cd people
mcpp run
```

The default template describes a message type at run time and uses it through reflection:
wire bytes, text and JSON, then Timestamp arithmetic, Struct and Any — no protoc. With
`--template mcpplibs.mwrap.protobuf@35.1:codegen`, `proto/addressbook.proto` is compiled by
protoc during the build and the generated classes are used with the module.

```toml
[dependencies]
mcpplibs.mwrap.protobuf = { version = "35.1", features = ["codegen"] }
```

```cpp
// build.mcpp
import mcpp;
import protogen;
int main() { return protogen::generate_all() ? 0 : 1; }
```

```cpp
// src/main.cpp: includes first (the prelude, then generated headers), imports after
#include <mwrap/protobuf/prelude.hpp>
#include "addressbook.pb.h"
import std;
import mcpplibs.mwrap.protobuf;
```

## Features

| Feature | Purpose |
|---|---|
| `codegen` | protoc for the build machine, from the same release, and the `protogen` rule (not on Windows: protoc is not built there) |
| `gzip` | `GzipInputStream` / `GzipOutputStream` over compat.zlib, `import mcpplibs.mwrap.protobuf.io.gzip;` |
| `upb` | protobuf's C runtime from the same release, as gRPC links it (one protobuf in a program with both) |

## Abseil and headers

protobuf's API takes and returns Abseil types (`absl::Status`, `absl::string_view`, which
is `std::string_view`); this package exports no `absl` name. To name them, import
`mcpplibs.mwrap.abseil` or include the Abseil header. With GCC every `#include` of a
translation unit comes before its first `import`; one that includes generated headers
includes `<mwrap/protobuf/prelude.hpp>` first. The version macros are constants in
`mcpplibs::mwrap::protobuf::macros`; `GOOGLE_PROTOBUF_VERIFY_VERSION` and the other
macros come from `<mwrap/protobuf/macros.hpp>`.

## Build and testing

CI on Linux (x86_64, aarch64; GCC 16 and LLVM 23), macOS and Windows builds the modules
and runs the tests in the dev and release profiles: wire bytes, text and JSON round trips,
the well-known types, MessageDifferencer and FieldMaskUtil, coded and delimited streams,
a `.proto` parsed at run time, every macro mapping, header mixing; the gzip feature; and
code generation through protogen (not on Windows). The Web row runs the tests as
wasm32 under node. The export audit derives the record again from the headers on the
Linux LLVM row. A separate job scaffolds both templates as an installed package and runs
them.

## License

[Apache-2.0](LICENSE) for the wrapper, by mcpp-community ([NOTICE](NOTICE)). Protocol
Buffers remains under the BSD 3-Clause License ([upstream/protobuf/LICENSE](upstream/protobuf/LICENSE),
[README](upstream/protobuf/README.md)), utf8_range under MIT
([upstream/utf8_range/LICENSE](upstream/utf8_range/LICENSE)), and Abseil, which it links,
under Apache-2.0 ([upstream/abseil-cpp/LICENSE](upstream/abseil-cpp/LICENSE)).
