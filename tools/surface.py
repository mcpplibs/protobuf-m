#!/usr/bin/env python3
"""The module's export lists, derived from protobuf's public headers.

The headers are those of the release compat.protobuf builds (upstream's own
tarball, pinned with its checksum in upstream/documents.json and fetched into
target/upstream), read with the Abseil release compat.abseil builds (the same
file pins it) and utf8_range from the protobuf tarball.

Modules, by the directory of the header that declares a name:

  protobuf   <google/protobuf/*.h>: messages, descriptors, reflection, arenas,
             repeated fields, maps, text format, the well-known types (the root,
             mcpplibs.mwrap.protobuf, which also re-exports the others)
  io         <google/protobuf/io/*.h>: coded streams, zero-copy streams,
             tokenizer, printer
  util       <google/protobuf/util/*.h>, <google/protobuf/json/json.h>: JSON,
             MessageDifferencer, TimeUtil, FieldMaskUtil, delimited messages,
             type resolvers
  compiler   <google/protobuf/compiler/{importer,parser}.h>: parsing .proto
             files at run time (the part of the compiler in the runtime library)
  gzip       <google/protobuf/io/gzip_stream.h>: GzipInputStream and
             GzipOutputStream, only with the package's `gzip` feature (the
             library defines them only then)

With --clang <clang++> the headers are read by the compiler and every name
declared in `namespace google::protobuf` and its public nested namespaces is
recorded in tools/surface.json under the module of the header that declares
it, provided that module's headers make it visible (else under the first
module whose headers do). A function name is exported by every module that
declares an overload of it. Namespaces whose name contains `internal` are
implementation detail and not exported, except their operators, which apply
to public types and are found by argument-dependent lookup. Names of internal
linkage cannot be exported; the record lists them. No `absl` name is
exported: Abseil's names belong to mcpplibs.mwrap.abseil.

The macros: the version macros are constants of the same names in
mcpplibs::mwrap::protobuf::macros (compiled as constant expressions), as is
GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG, a constexpr function; every other macro
of the headers stays a macro, in <mwrap/protobuf/macros.hpp>, listed with its
reason.

--write writes the module sources, <mwrap/protobuf/macros.hpp>,
<mwrap/protobuf/prelude.hpp>, tests/macros_test.cpp and docs/api.md.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import tarfile
import tempfile
import urllib.request

ROOT = Path(__file__).resolve().parent.parent
RECORD = ROOT / 'tools/surface.json'
SRC = ROOT / 'src'
VERSION = '35.1'

MODULES = ('protobuf', 'io', 'util', 'compiler', 'gzip')
MODULE_NAMES = {'protobuf': 'mcpplibs.mwrap.protobuf', 'io': 'mcpplibs.mwrap.protobuf.io', 'util': 'mcpplibs.mwrap.protobuf.util',
                'compiler': 'mcpplibs.mwrap.protobuf.compiler', 'gzip': 'mcpplibs.mwrap.protobuf.io.gzip'}
MODULE_FILES = {'protobuf': 'src/protobuf.cppm', 'io': 'src/io.cppm', 'util': 'src/util.cppm', 'compiler': 'src/compiler.cppm',
                'gzip': 'src/gzip/io_gzip.cppm'}
# Test and mocking support in the public directories: they need GoogleTest/GoogleMock or protobuf's test protos.
EXCLUDED = {
    'google/protobuf/arena_test_util.h': 'test support', 'google/protobuf/map_test_util.h': 'test support (needs test protos)',
    'google/protobuf/reflection_tester.h': 'test support (needs test protos)', 'google/protobuf/test_util.h': 'test support',
    'google/protobuf/test_util2.h': 'test support', 'google/protobuf/test_util_lite.h': 'test support',
    'google/protobuf/test_textproto.h': 'test support (needs GoogleMock)', 'google/protobuf/wire_format_unittest.h': 'test support',
    'google/protobuf/descriptor_test_utils.h': 'test support', 'google/protobuf/internal_visibility_for_testing.h': 'test support',
    'google/protobuf/unredacted_debug_format_for_test.h': 'test support', 'google/protobuf/io/test_zero_copy_stream.h': 'test support',
    'google/protobuf/io/io_win32.h': 'Windows-only portability layer of the compiler (namespace io::win32)',
    'google/protobuf/util/internal_timeval.h': 'implementation detail',
}
# The namespaces whose names are exported; any other namespace a header mentions only forward-declares friends.
SCOPES = {'google::protobuf', 'google::protobuf::io', 'google::protobuf::util', 'google::protobuf::json', 'google::protobuf::compiler'}
# Constants of internal linkage (stubs/common.h: `static const` limits), declared again with their values.
ADAPTED = {f'google::protobuf::{n}' for n in ('kint32max', 'kint32min', 'kint64max', 'kint64min', 'kuint32max', 'kuint64max')}
COMPILER_HEADERS = ('google/protobuf/compiler/importer.h', 'google/protobuf/compiler/parser.h')
GZIP_HEADERS = ('google/protobuf/io/gzip_stream.h',)

KINDS = {
    'CXXRecordDecl': 'types', 'ClassTemplateDecl': 'types', 'EnumDecl': 'types', 'TypeAliasDecl': 'types', 'TypedefDecl': 'types',
    'TypeAliasTemplateDecl': 'types', 'FunctionDecl': 'functions', 'FunctionTemplateDecl': 'functions', 'VarDecl': 'objects',
    'VarTemplateDecl': 'objects', 'UsingDecl': 'reexported',
}
ADL = re.compile(r'operator\W')
MEMBERS = {'CXXMethodDecl', 'CXXConstructorDecl', 'CXXDestructorDecl', 'CXXConversionDecl'}
PASSED = {'UsingDirectiveDecl', 'UsingShadowDecl', 'ClassTemplateSpecializationDecl', 'ClassTemplatePartialSpecializationDecl',
          'CXXDeductionGuideDecl', 'StaticAssertDecl', 'EmptyDecl', 'FriendDecl', 'LinkageSpecDecl', 'FullComment',
          'VarTemplateSpecializationDecl', 'VarTemplatePartialSpecializationDecl', 'ExplicitInstantiationDecl', 'NamespaceAliasDecl',
          'UsingEnumDecl', 'ConceptDecl', *MEMBERS}


def fetch():
    """protobuf's and Abseil's release trees under target/upstream, checked against upstream/documents.json."""
    pins = json.loads((ROOT / 'upstream/documents.json').read_text(encoding='utf-8'))
    cache = ROOT / 'target/upstream'
    cache.mkdir(parents=True, exist_ok=True)
    tops = []
    for key, name in (('upstream', 'protobuf.tar.gz'), ('abseil', 'abseil-cpp.tar.gz')):
        url, digest = pins[key], pins[key + '_sha256']
        archive = cache / name
        if not archive.exists() or hashlib.sha256(archive.read_bytes()).hexdigest() != digest:
            print('+ fetch', url, flush=True)
            with urllib.request.urlopen(url, timeout=300) as response:
                archive.write_bytes(response.read())
        got = hashlib.sha256(archive.read_bytes()).hexdigest()
        if got != digest:
            sys.exit(f'{name}: checksum {got} != {digest}')
        with tarfile.open(archive) as tar:
            top = tar.next().name.split('/')[0]
            if not (cache / top).exists():
                tar.extractall(cache, filter='data')
        tops.append(cache / top)
    return tops[0], tops[1]


def include_dirs(protobuf, abseil):
    return [protobuf / 'src', abseil, protobuf / 'third_party/utf8_range']


def headers(protobuf, module=None):
    src = protobuf / 'src'
    rel = lambda p: p.relative_to(src).as_posix()
    found = {
        'protobuf': sorted(rel(p) for p in (src / 'google/protobuf').glob('*.h')),
        'io': sorted(rel(p) for p in (src / 'google/protobuf/io').glob('*.h') if rel(p) not in GZIP_HEADERS),
        'util': sorted(rel(p) for p in (src / 'google/protobuf/util').glob('*.h')) + ['google/protobuf/json/json.h'],
        'compiler': list(COMPILER_HEADERS),
        'gzip': list(GZIP_HEADERS),
    }
    found = {m: [h for h in hs if h not in EXCLUDED and not h.endswith('/package_info.h')] for m, hs in found.items()}
    return found[module] if module else found


def module_of(path):
    path = path.replace('\\', '/')
    match = re.search(r'google/protobuf/(.*)$', path)
    if not match:
        return None
    rel = 'google/protobuf/' + match.group(1)
    if rel in GZIP_HEADERS:
        return 'gzip'
    for module, prefix in (('compiler', 'google/protobuf/compiler/'), ('io', 'google/protobuf/io/'), ('util', 'google/protobuf/util/'),
                           ('util', 'google/protobuf/json/')):
        if rel.startswith(prefix):
            return module
    return 'protobuf'


def dump(clang, protobuf, abseil, include):
    with tempfile.TemporaryDirectory(prefix='protobuf-surface-') as directory:
        source = Path(directory) / 'all.cpp'
        source.write_text(''.join(f'#include <{h}>\n' for h in include), encoding='utf-8')
        command = [clang, '-std=c++23', '-fsyntax-only', '-w', *[f'-I{d}' for d in include_dirs(protobuf, abseil)],
                   '-Xclang', '-skip-function-bodies', '-Xclang', '-ast-dump=json', '-Xclang', '-ast-dump-filter=google', str(source)]
        text = '\n' + subprocess.run(command, check=True, stdout=subprocess.PIPE, text=True).stdout
    decoder, at, out = json.JSONDecoder(), 0, []
    while True:
        at = text.find('\n{', at - 1 if at else 0)
        if at < 0:
            break
        tree, at = decoder.raw_decode(text, at + 1)
        out.append(tree)
    return out


def annotate(node, state):
    """Give every node the file of its location: clang writes a location's file only when it changes."""
    def touch(loc):
        if 'spellingLoc' in loc:
            touch(loc['spellingLoc'])
            touch(loc.get('expansionLoc', {}))
        elif 'file' in loc:
            state['file'] = loc['file']
    if isinstance(node, dict):
        for key, value in list(node.items()):
            if key == 'loc':
                touch(value)
                node['_file'] = state['file']
            elif key == 'range':
                touch(value.get('begin', {}))
                touch(value.get('end', {}))
            elif key not in ('includedFrom',):
                annotate(value, state)
    elif isinstance(node, list):
        for item in node:
            annotate(item, state)


def internal_linkage(node):
    return node.get('storageClass') == 'static' or (node.get('kind') == 'VarDecl' and not node.get('inline') and node.get('constexpr'))


def declarations(clang, protobuf, abseil, include):
    found, skipped, unknown = {}, set(), set()

    def walk(node, scope, hidden=False):
        for child in node.get('inner', []):
            kind, name = child.get('kind'), child.get('name')
            if kind == 'NamespaceDecl':
                if child.get('isInline'):
                    walk(child, scope, hidden)
                elif not name:
                    skipped.add(f'{scope}::(anonymous)')
                elif hidden or 'internal' in name:
                    skipped.add(f'{scope}::{name}')
                    walk(child, f'{scope}::{name}', True)
                else:
                    walk(child, f'{scope}::{name}')
                continue
            if child.get('isImplicit') or not name or kind in PASSED or name.startswith('<'):
                continue
            if hidden and not (kind in ('FunctionDecl', 'FunctionTemplateDecl') and ADL.match(name)):
                continue
            if 'parentDeclContextId' in child or (kind == 'FunctionTemplateDecl' and any(i.get('kind') in MEMBERS for i in child.get('inner', []))):
                continue
            if kind not in KINDS:
                unknown.add(f'{kind} {scope}::{name}')
                continue
            if kind == 'UsingDecl':
                name = name.split('::')[-1]
            entry = found.setdefault((scope, name), {'kind': 'adl' if hidden else KINDS[kind], 'modules': set(), 'internal': False,
                                                     'definition': set()})
            entry.setdefault('decls', set()).add(kind)
            module = module_of(child.get('_file') or '')
            if module:
                entry['modules'].add(module)
                if child.get('completeDefinition') or any(i.get('completeDefinition') for i in child.get('inner', []) if isinstance(i, dict)):
                    entry['definition'].add(module)
            if kind in ('VarDecl', 'FunctionDecl') and internal_linkage(child):
                entry['internal'] = True
            if kind == 'EnumDecl' and not child.get('scopedEnumTag'):
                for item in child.get('inner', []):
                    if item.get('kind') == 'EnumConstantDecl':
                        e = found.setdefault((scope, item['name']), {'kind': 'enumerators', 'modules': set(), 'internal': False, 'definition': set()})
                        if module:
                            e['modules'].add(module)

    state = {'file': None}
    for tree in dump(clang, protobuf, abseil, include):
        annotate(tree, state)
        if tree.get('kind') == 'NamespaceDecl' and tree.get('name') == 'google':
            for child in tree.get('inner', []):
                if child.get('kind') == 'NamespaceDecl' and child.get('name') == 'protobuf':
                    walk(child, 'google::protobuf')
    assert not unknown, sorted(unknown)
    return found, skipped


# ── macros ───────────────────────────────────────────────────────────────────

MACRO = re.compile(r'#define\s+([A-Za-z_]\w*)(\(([^)]*)\))?\s*(.*)$')
# Mapped by hand: a computation, as a constexpr function of the same name (tests/macros_test.cpp compares them).
FUNCTIONS = {'GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG'}


def macro_definitions(clang, protobuf, abseil, include):
    source = ''.join(f'#include <{h}>\n' for h in include)
    command = [clang, '-std=c++23', '-E', '-dD', '-w', *[f'-I{d}' for d in include_dirs(protobuf, abseil)], '-x', 'c++', '-']
    text = subprocess.run(command, check=True, input=source, stdout=subprocess.PIPE, text=True).stdout
    current, out = '', {}
    for line in text.splitlines():
        if line.startswith('# ') and '"' in line:
            current = line.split('"')[1].replace('\\\\', '/').replace('\\', '/')
            continue
        if '/google/protobuf/' not in current:
            continue
        if line.startswith('#undef '):
            out.pop(line.split()[1], None)
            continue
        match = MACRO.match(line)
        if match:
            out[match.group(1)] = {'header': current.split('/src/', 1)[-1], 'params': match.group(2) is not None, 'body': match.group(4).strip()}
    return out


def constant_expressions(clang, protobuf, abseil, include, names):
    names = list(names)
    while names:
        preamble = ''.join(f'#include <{h}>\n' for h in include)
        offset = preamble.count('\n')
        lines = [f'inline constexpr auto mwrap_probe_{i} = {n};' for i, n in enumerate(names)]
        with tempfile.TemporaryDirectory(prefix='protobuf-probe-') as directory:
            source = Path(directory) / 'probe.cpp'
            source.write_text(preamble + '\n'.join(lines) + '\n', encoding='utf-8')
            result = subprocess.run([clang, '-std=c++23', '-fsyntax-only', '-ferror-limit=0', '-w',
                                     *[f'-I{d}' for d in include_dirs(protobuf, abseil)], str(source)], stderr=subprocess.PIPE, text=True)
        bad = {int(m.group(1)) - offset for m in re.finditer(r'probe\.cpp:(\d+):\d+: error', result.stderr)}
        bad = {names[i - 1] for i in bad if 1 <= i <= len(names)}
        if not bad:
            return names
        names = [n for n in names if n not in bad]
    return names


def classify_macros(clang, protobuf, abseil, include):
    defined = macro_definitions(clang, protobuf, abseil, include)
    textual, candidates = {}, []
    for name, entry in sorted(defined.items()):
        if name in FUNCTIONS:
            continue
        body = entry['body']
        if name.endswith(('_H__', '_H_')) or re.fullmatch(r'google_2fprotobuf_\w+_2eh', name):
            textual[name] = 'header guard'
        elif entry['header'].endswith('stubs/platform_macros.h'):
            textual[name] = 'describes the target (GOOGLE_PROTOBUF_ARCH_*): a switch for #if, different on each platform'
        elif not body:
            textual[name] = 'empty: an annotation or a switch for #ifdef'
        elif entry['params']:
            textual[name] = 'function-like: generates code or attributes, or returns from its caller'
        elif 'INTERNAL' in name or name.startswith('PROTOBUF_TC_') or body.startswith(('[[', 'ABSL_', 'PROTOBUF_EXPORT')):
            textual[name] = 'annotation or an internal parameter list of generated code'
        elif re.search(r'::google::protobuf::internal::', body):
            textual[name] = 'a statement of generated code (calls the library\'s internals)'
        else:
            candidates.append(name)
    constants = constant_expressions(clang, protobuf, abseil, include, candidates)
    for name in candidates:
        if name not in constants:
            textual[name] = 'not a constant expression'
    return {'constants': constants, 'functions': sorted(FUNCTIONS), 'textual': textual}


# ── the record ───────────────────────────────────────────────────────────────

def derive(clang):
    protobuf, abseil = fetch()
    sets = headers(protobuf)
    every = sum(sets.values(), [])
    found, skipped = declarations(clang, protobuf, abseil, every)
    visible = {m: set(declarations(clang, protobuf, abseil, sets[m])[0]) for m in MODULES}
    record = {m: {} for m in MODULES}
    internal = {}
    adapted = []
    for (scope, name), entry in sorted(found.items()):
        key = (scope, name)
        public_scope = scope in SCOPES or scope.split('::')[-1].find('internal') >= 0
        forward_only = entry.get('decls', set()) <= {'CXXRecordDecl', 'ClassTemplateDecl'} and not entry['definition']
        if not public_scope or (scope != 'google::protobuf' and entry['kind'] == 'types' and forward_only):
            internal[f'{scope}::{name}'] = 'only declared (a friend of a protobuf class): defined by another project or by protoc'
            continue
        if entry['internal']:
            if entry['kind'] == 'objects' and f'{scope}::{name}' in ADAPTED:
                adapted.append(name)
                internal[f'{scope}::{name}'] = 'internal linkage: declared again with its value in mcpplibs::mwrap::protobuf'
            else:
                internal[f'{scope}::{name}'] = 'internal linkage (static, or constexpr not inline): every includer has its own'
            continue
        if entry['kind'] in ('functions', 'adl'):
            owners = [m for m in MODULES if m in entry['modules'] and key in visible[m]]
        else:
            preferred = entry['definition'] or entry['modules']
            owners = [m for m in MODULES if m in preferred and key in visible[m]][:1]
        if not owners:
            owners = [m for m in MODULES if key in visible[m]][:1]
        if not owners:
            internal[f'{scope}::{name}'] = 'declared by no public header (only reached through an implementation header)'
            continue
        for module in owners:
            record[module].setdefault(scope, {}).setdefault(entry['kind'], []).append(name)
    macros = classify_macros(clang, protobuf, abseil, every)
    pins = json.loads((ROOT / 'upstream/documents.json').read_text(encoding='utf-8'))
    return {
        'release': pins['upstream'],
        'headers': sets,
        'modules': {m: {s: {k: sorted(v) for k, v in sorted(kinds.items())} for s, kinds in sorted(record[m].items())} for m in MODULES},
        'not exported': {
            'headers': EXCLUDED,
            'implementation namespaces': sorted(skipped),
            'other namespaces': {'pb': 'the C++ feature-set extension of editions (pb::CppFeatures, from cpp_features.proto; extension_set.h '
                                       'forward-declares it), used by protoc and generated code: not exported, so a module-only translation '
                                       'unit keeps the common `namespace pb = google::protobuf;`; <google/protobuf/cpp_features.pb.h> declares it'},
            'names': internal,
        },
        'adapted': adapted,
        'macros': macros,
    }


def record():
    return json.loads(RECORD.read_text(encoding='utf-8'))


# ── the sources ──────────────────────────────────────────────────────────────

HEADER = '// Written by tools/surface.py from tools/surface.json; tools/check_exports.py audits it. Do not edit by hand.\n'
TITLES = (('types', 'types'), ('enumerators', 'enumerators of unscoped enumerations'), ('objects', 'objects'),
          ('functions', 'functions and operators'), ('reexported', 'names protobuf re-declares from another namespace'),
          ('adl', 'operators of public types declared in an implementation namespace, for argument-dependent lookup'))
SUMMARY = {
    'protobuf': 'messages, descriptors, reflection, arenas, repeated fields, maps, text format and the well-known types',
    'io': 'coded and zero-copy streams, the tokenizer and the printer',
    'util': 'JSON, MessageDifferencer, TimeUtil, FieldMaskUtil, delimited messages and type resolvers',
    'compiler': 'parsing .proto files at run time: Importer, DiskSourceTree, Parser',
    'gzip': 'GzipInputStream and GzipOutputStream (the package\'s `gzip` feature)',
}


def module_text(module, surface):
    name = MODULE_NAMES[module]
    lines = [HEADER, f'// {name} — {SUMMARY[module]} (protobuf {VERSION}), unchanged:',
             '// each name is a using-declaration of the upstream entity in the namespace upstream declares it in.']
    if module == 'protobuf':
        lines.append('// The root also re-exports the other modules (gzip only through its own import, with the feature).')
    lines += ['module;', '#include "detail/prelude.hpp"' if module != 'gzip' else '#include "../detail/prelude.hpp"',
              *[f'#include <{h}>' for h in surface['headers'][module]], '', f'export module {name};', '']
    if module == 'protobuf':
        lines += ['export import mcpplibs.mwrap.protobuf.io;', 'export import mcpplibs.mwrap.protobuf.util;',
                  'export import mcpplibs.mwrap.protobuf.compiler;', 'export import mcpplibs.mwrap.protobuf.macros;', '']
    if module == 'protobuf' and surface['adapted']:
        lines += ['// constants of internal linkage in the headers (stubs/common.h), declared again under their names with their values',
                  'export namespace mcpplibs::mwrap::protobuf {']
        lines += [f'inline constexpr auto {n} = ::google::protobuf::{n};' for n in surface['adapted']]
        lines += ['}  // namespace mcpplibs::mwrap::protobuf', '']
    for scope, kinds in surface['modules'][module].items():
        lines.append(f'export namespace {scope} {{')
        for kind, title in TITLES:
            names = kinds.get(kind, [])
            if names:
                lines.append(f'// {title} ({len(names)})')
                lines += [f'using ::{scope}::{n};' for n in names]
        lines += [f'}}  // namespace {scope}', '']
    return '\n'.join(lines)


def macros_text(surface):
    m = surface['macros']
    lines = [HEADER, '// mcpplibs.mwrap.protobuf.macros — the version macros of protobuf\'s headers as constants of the same names in',
             '// mcpplibs::mwrap::protobuf::macros, and GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG as a constexpr function. A module',
             '// does not export macros: the others (GOOGLE_PROTOBUF_VERIFY_VERSION, the platform switches …) come from',
             '// #include <mwrap/protobuf/macros.hpp>, before the import.', 'module;', '#include "detail/prelude.hpp"',
             *[f'#include <{h}>' for h in surface['headers']['protobuf']], '', 'namespace mwrap_protobuf_macros {']
    lines += [f'inline constexpr auto value_{n} = {n};' for n in m['constants']]
    lines.append('}  // namespace mwrap_protobuf_macros\n')
    lines += [f'#undef {n}' for n in m['constants'] + m['functions']]
    lines += ['', 'export module mcpplibs.mwrap.protobuf.macros;', '', 'export namespace mcpplibs::mwrap::protobuf::macros {', '']
    lines += [f'inline constexpr auto {n} = mwrap_protobuf_macros::value_{n};' for n in m['constants']]
    lines += ['',
              '// wire_format_lite.h: the tag of a field, (number << 3) | wire type.',
              'constexpr std::uint32_t GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(int field_number, ::google::protobuf::internal::WireFormatLite::WireType type) {',
              '    return static_cast<std::uint32_t>((static_cast<std::uint32_t>(field_number) << ::google::protobuf::internal::WireFormatLite::kTagTypeBits) | type);',
              '}', '', '}  // namespace mcpplibs::mwrap::protobuf::macros', '']
    return '\n'.join(lines)


def macros_hpp_text(surface):
    lines = ['// <mwrap/protobuf/macros.hpp> — protobuf\'s macros (GOOGLE_PROTOBUF_VERIFY_VERSION, the GOOGLE_PROTOBUF_ARCH_*',
             '// switches …), which a module cannot export: every public header the modules include, in their order. Include',
             '// it BEFORE `import mcpplibs.mwrap.protobuf;` (GCC requires the includes of a translation unit to come before its',
             '// imports). Written by tools/surface.py.', '#pragma once', '#include <mwrap/protobuf/prelude.hpp>', '']
    return '\n'.join(lines)


def prelude_hpp_text(surface):
    lines = ['// <mwrap/protobuf/prelude.hpp> — every header the modules of mcpplibs.mwrap.protobuf include in their global module',
             '// fragments (without gzip_stream.h, which only the `gzip` feature\'s module includes), in their order.',
             '//', '// A translation unit that includes a generated `.pb.h` and imports the module includes this header first:',
             '// with GCC every #include must come before the first import, and a translation unit that includes fewer',
             '// of these headers than the modules did can read a module\'s declaration as conflicting with its own',
             '// (descriptor.h). Written by tools/surface.py.', '#pragma once']
    seen = []
    for module in ('protobuf', 'io', 'util', 'compiler'):
        seen += [h for h in surface['headers'][module] if h not in seen]
    lines += [f'#include <{h}>' for h in seen]
    return '\n'.join(lines) + '\n'


def macros_test_text(surface):
    m = surface['macros']
    lines = ['// Written by tools/surface.py: every constant of mcpplibs.mwrap.protobuf.macros against the macro of the same',
             '// name, and GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG against its mapping. The headers come first; each macro is',
             '// taken, then set aside: while it is defined it would replace the qualified name too.',
             '#include <mwrap/protobuf/macros.hpp>', '#include <cstdint>', '#include <string_view>', '#include <type_traits>', '',
             'namespace header {']
    lines += [f'inline constexpr auto {n.lower()} = {n};' for n in m['constants']]
    lines += ['constexpr std::uint32_t make_tag(int number, ::google::protobuf::internal::WireFormatLite::WireType type) {',
              '    return GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(number, type);', '}', '}  // namespace header', '']
    lines += [f'#undef {n}' for n in m['constants'] + m['functions']]
    lines += ['', '#include <gtest/gtest.h>', '', 'import mcpplibs.mwrap.protobuf;', '',
              'namespace macros = mcpplibs::mwrap::protobuf::macros;', 'using google::protobuf::internal::WireFormatLite;', '',
              'template <class A, class B>', 'constexpr bool same(A a, B b) {',
              '    if constexpr (!std::is_same_v<A, B>) return false;',
              '    else if constexpr (std::is_same_v<A, const char*>) return std::string_view(a) == std::string_view(b);',
              '    else return a == b;', '}', '']
    lines += [f'static_assert(same(macros::{n}, header::{n.lower()}), "{n}");' for n in m['constants']]
    lines += ['static_assert(macros::GOOGLE_PROTOBUF_VERSION == 7035001);', '',
              'TEST(Macros, MakeTagAsTheMacro) {',
              '    for (int number : {1, 2, 15, 16, 2047, 536870911})',
              '        for (auto type : {WireFormatLite::WIRETYPE_VARINT, WireFormatLite::WIRETYPE_FIXED64, WireFormatLite::WIRETYPE_LENGTH_DELIMITED,',
              '                          WireFormatLite::WIRETYPE_START_GROUP, WireFormatLite::WIRETYPE_END_GROUP, WireFormatLite::WIRETYPE_FIXED32})',
              '            EXPECT_EQ(macros::GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(number, type), header::make_tag(number, type));',
              '    static_assert(macros::GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG(1, WireFormatLite::WIRETYPE_LENGTH_DELIMITED) == 10);', '}', '']
    return '\n'.join(lines)


def api_text(surface):
    m, n = surface['macros'], surface['not exported']
    count = lambda module: sum(len(v) for kinds in surface['modules'][module].values() for v in kinds.values())
    lines = ['# Modules and API', '', '[Usage](usage.md) · [README](../README.md) · written by `tools/surface.py` from `tools/surface.json`'
             ' (audited by `tools/check_exports.py`).', '',
             'Every exported name is the upstream entity, in the namespace upstream declares it in. No `absl` name is exported:',
             'protobuf\'s API uses Abseil types (`absl::Status`, `absl::string_view`, `absl::Cord`); to name them, import',
             '`mcpplibs.mwrap.abseil` or include the Abseil header.', '', '| Module | Names | Of |', '|---|---:|---|']
    lines += [f'| `{MODULE_NAMES[mod]}` | {count(mod)} | {SUMMARY[mod]} |' for mod in MODULES]
    lines += [f'| `mcpplibs.mwrap.protobuf.macros` | {len(m["constants"]) + len(m["functions"])} | the version macros and the tag computation |', '']
    for mod in MODULES:
        lines += [f'## {MODULE_NAMES[mod]}', '']
        for scope, kinds in surface['modules'][mod].items():
            for kind, title in TITLES:
                names = kinds.get(kind, [])
                if names:
                    lines.append(f'- `{scope}` {title} ({len(names)}): ' + ', '.join(f'`{x}`' for x in names))
        lines.append('')
    lines += ['## Not exported', '', '- Headers:']
    lines += [f'  - `{h}`: {r}' for h, r in n['headers'].items()]
    lines += ['- Implementation namespaces: ' + ', '.join(f'`{x}`' for x in n['implementation namespaces'])]
    lines += [f'- Namespace `{k}`: {v}' for k, v in n['other namespaces'].items()]
    lines += ['- Names:'] + [f'  - `{k}`: {v}' for k, v in n['names'].items()]
    lines += ['', '## Macros', '', '- Constants in `mcpplibs::mwrap::protobuf::macros`: ' + ', '.join(f'`{x}`' for x in m['constants']),
              '- Functions: ' + ', '.join(f'`{x}`' for x in m['functions']),
              '- Through `<mwrap/protobuf/macros.hpp>` only:']
    lines += [f'  - `{k}`: {v}' for k, v in m['textual'].items()]
    return '\n'.join(lines) + '\n'


def outputs(surface=None):
    surface = surface or record()
    out = {ROOT / MODULE_FILES[m]: module_text(m, surface) for m in MODULES}
    out[SRC / 'macros.cppm'] = macros_text(surface)
    out[ROOT / 'include/mwrap/protobuf/macros.hpp'] = macros_hpp_text(surface)
    out[ROOT / 'include/mwrap/protobuf/prelude.hpp'] = prelude_hpp_text(surface)
    out[ROOT / 'tests/macros_test.cpp'] = macros_test_text(surface)
    out[ROOT / 'docs/api.md'] = api_text(surface)
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--clang', help='derive tools/surface.json with this clang++')
    parser.add_argument('--write', action='store_true', help='write the module sources and the headers')
    args = parser.parse_args()
    if args.clang:
        RECORD.write_text(json.dumps(derive(args.clang), indent=1) + '\n', encoding='utf-8')
    if args.write:
        for path, text in outputs().items():
            path.parent.mkdir(parents=True, exist_ok=True)
            if not path.exists() or path.read_text(encoding='utf-8') != text:
                path.write_text(text, encoding='utf-8')
    surface = record()
    for module, scopes in surface['modules'].items():
        print(module + ': ' + '; '.join(f'{scope} ' + ', '.join(f'{len(v)} {k}' for k, v in kinds.items()) for scope, kinds in scopes.items()))
    print('macros: ' + ', '.join(f'{len(v)} {k}' for k, v in surface['macros'].items()))


if __name__ == '__main__':
    sys.exit(main())
