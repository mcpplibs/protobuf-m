#!/usr/bin/env python3
"""examples/codegen is templates/codegen instantiated: CI builds and runs it, which is
what tests the template's code generation. Its build.mcpp, proto/ and src/main.cpp
must therefore be the template's, byte for byte (the template's main.cpp.in and
build.mcpp.in carry no substitutions)."""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
pairs = [('templates/codegen/build.mcpp.in', 'examples/codegen/build.mcpp'),
         ('templates/codegen/src/main.cpp.in', 'examples/codegen/src/main.cpp'),
         ('templates/codegen/proto/addressbook.proto', 'examples/codegen/proto/addressbook.proto')]
for template, example in pairs:
    a, b = (ROOT / template).read_bytes(), (ROOT / example).read_bytes()
    assert b'{{' not in a, template
    assert a == b, f'{example} differs from {template}'
print('examples/codegen is templates/codegen instantiated')
