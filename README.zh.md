# protobuf-m

> 以 C++23 模块使用 Protocol Buffers 35.1（C++ 运行时）。

[English](README.md) · **简体中文** · [Upstream](https://github.com/protocolbuffers/protobuf) · [Releases](https://github.com/mcpplibs/protobuf-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/protobuf-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/protobuf-m/actions/workflows/ci.yml)

上游 C++ API 不变：`google::protobuf::Message`、`DescriptorPool`、`util::MessageDifferencer`、
`Timestamp` 就是上游实体。库由 `compat.protobuf` 35.1 提供，基于 `compat.abseil` 20250512.1，
均从发布包构建；`codegen` feature 增加 protoc（为构建机从同一发布构建）和构建规则
`protogen`，在构建时为每个 `.proto` 生成代码。

## Modules

- `import mcpplibs.mwrap.protobuf;`：消息、描述符、反射、arena、repeated 字段、map、文本格式、well-known types；并重导出下列模块
- `import mcpplibs.mwrap.protobuf.io;`：coded / zero-copy 流、tokenizer
- `import mcpplibs.mwrap.protobuf.util;`：JSON（`json::`）、`MessageDifferencer`、`TimeUtil`、`FieldMaskUtil`、带长度前缀的消息
- `import mcpplibs.mwrap.protobuf.compiler;`：运行时解析 `.proto`（`Importer`、`Parser`）
- `import mcpplibs.mwrap.protobuf.io.gzip;`：`GzipInputStream`、`GzipOutputStream`（feature `gzip`）

> More Modules API -> [用法](docs/usage.md) · [全部导出名](docs/api.md)

## 快速开始

```sh
mcpp new people --template mcpplibs.mwrap.protobuf@35.1
cd people
mcpp run
```

默认模板在运行时描述一个消息类型并通过反射使用：线上字节、文本与 JSON，再演示
Timestamp 运算、Struct 与 Any —— 不需要 protoc。用
`--template mcpplibs.mwrap.protobuf@35.1:codegen` 时，`proto/addressbook.proto` 在构建中由
protoc 编译，生成的类与模块一起使用。

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
// src/main.cpp：先 include（prelude，再生成的头文件），后 import
#include <mwrap/protobuf/prelude.hpp>
#include "addressbook.pb.h"
import std;
import mcpplibs.mwrap.protobuf;
```

## Features

| Feature | 用途 |
|---|---|
| `codegen` | 为构建机构建的同版本 protoc，以及 `protogen` 规则（Windows 不可用：那里不构建 protoc） |
| `gzip` | 基于 compat.zlib 的 `GzipInputStream` / `GzipOutputStream`，`import mcpplibs.mwrap.protobuf.io.gzip;` |
| `upb` | 同一发布中的 protobuf C 运行时，供 gRPC 链接（两者同用时只有一份 protobuf） |

## Abseil 与头文件

protobuf 的 API 使用 Abseil 类型（`absl::Status`、`absl::string_view`，即
`std::string_view`）；本包不导出任何 `absl` 名字。需要书写这些名字时，import
`mcpplibs.mwrap.abseil` 或 include Abseil 头文件。GCC 要求翻译单元的所有 `#include` 位于
第一个 `import` 之前；包含生成头文件的翻译单元先 include `<mwrap/protobuf/prelude.hpp>`。
版本宏是 `mcpplibs::mwrap::protobuf::macros` 中的常量；`GOOGLE_PROTOBUF_VERIFY_VERSION`
等其余宏由 `<mwrap/protobuf/macros.hpp>` 提供。

## 构建与测试

CI 在 Linux（x86_64、aarch64；GCC 16 与 LLVM 23）、macOS、Windows 上构建模块，并在 dev 与
release 两种 profile 下运行测试：线上字节、文本与 JSON 往返、well-known types、
MessageDifferencer 与 FieldMaskUtil、coded 与带长度前缀的流、运行时解析 `.proto`、每个宏映射、
头文件混用；gzip feature；以及经 protogen 的代码生成（Windows 除外）。Web 行在 node 下以
wasm32 运行测试。导出审计在 Linux LLVM 行上从头文件重新推导记录。另一个作业把两个模板作为
已安装包生成并运行。

## 许可

封装层为 [Apache-2.0](LICENSE)，归属 mcpp-community（[NOTICE](NOTICE)）。Protocol Buffers 保持
BSD 3-Clause（[upstream/protobuf/LICENSE](upstream/protobuf/LICENSE)、[README](upstream/protobuf/README.md)），
utf8_range 为 MIT（[upstream/utf8_range/LICENSE](upstream/utf8_range/LICENSE)），其链接的 Abseil 为
Apache-2.0（[upstream/abseil-cpp/LICENSE](upstream/abseil-cpp/LICENSE)）。
