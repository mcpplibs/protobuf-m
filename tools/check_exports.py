#!/usr/bin/env python3
"""Audit the module surface against protobuf's public headers.

The generated sources (src/*.cppm, src/gzip/io_gzip.cppm, the two side headers,
tests/macros_test.cpp, docs/api.md) must be exactly what tools/surface.py
writes from tools/surface.json, and no `absl` name may be exported. With a
clang++ (--clang, or MWRAP_CLANG) the record itself is derived again from the
release headers and must be the same: no public name left out, none exported
that is not there. MWRAP_REQUIRE_CLANG=1 makes the second half mandatory (CI's
Linux LLVM row).
"""
import argparse
import os
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import surface  # noqa: E402


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--clang', default=os.environ.get('MWRAP_CLANG'))
    args = parser.parse_args()
    record = surface.record()
    for path, text in surface.outputs(record).items():
        assert path.read_text(encoding='utf-8') == text, f'{path.relative_to(surface.ROOT)} is not what tools/surface.py --write gives'
    for module, scopes in record['modules'].items():
        assert all(scope.startswith('google::protobuf') for scope in scopes), (module, list(scopes))
    if args.clang:
        derived = surface.derive(args.clang)
        if derived != record:
            for key in sorted(set(derived) | set(record)):
                if derived.get(key) != record.get(key):
                    print('differs:', key, file=sys.stderr)
            sys.exit('the record is not what clang derives from the headers: run tools/surface.py --clang <clang++> --write')
    else:
        assert os.environ.get('MWRAP_REQUIRE_CLANG') != '1', 'MWRAP_REQUIRE_CLANG=1 and no clang++ given'
    counts = {m: sum(len(v) for kinds in scopes.values() for v in kinds.values()) for m, scopes in record['modules'].items()}
    print(f'exported: {counts}; {len(record["adapted"])} constants adapted; {len(record["not exported"]["names"])} names not exported; '
          f'macros: {len(record["macros"]["constants"])} constants, {len(record["macros"]["functions"])} function, '
          f'{len(record["macros"]["textual"])} textual; '
          + ('the record is what clang derives from the headers' if args.clang else 'the record not derived again (no clang++ given)'))


if __name__ == '__main__':
    main()
