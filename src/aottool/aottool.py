#!/usr/bin/env python3
from argparse import ArgumentParser
from shutil import copyfile
import os
import subprocess
from libaottool.assembly import ILAssembly
from libaottool.lexer import lex_il_line

parser = ArgumentParser(description='Compile C# projects to alternative binary formats')
parser.add_argument('project_path', nargs='?', default='.', help='Path to a C# project folder')
parser.add_argument('-o', '--output', help='Path to which output is written')
parser.add_argument('-f', '--format', choices=['cil', 'c', 'bin'], default='cil', help='Compiler mode')

args = parser.parse_args()

aot_dir = os.path.join(args.project_path, 'aot')
if not os.path.isdir(aot_dir):
  os.makedirs(aot_dir)
subprocess.call(['dotnet', 'build', '-o', aot_dir, args.project_path])
project_name = os.path.basename(os.path.abspath(args.project_path))
binary_path = os.path.join(aot_dir, project_name + '.dll')
if not os.path.isfile(binary_path):
  print('Project name must match its parent folder (Cannot find %s)' % (project_name + '.dll'))
  exit(1)
il_path = os.path.join(aot_dir, project_name + '.il')
subprocess.call(['dotnet-ildasm', binary_path, '-f', '-o', il_path])

if args.format == 'cil':
  if not args.output == None:
    copyfile(il_path, args.output)
  exit(0)

assembly_c = None
with open(il_path) as il_file:
  tokenized_lines = []
  for line in il_file:
    tokens = lex_il_line(line)
    if len(tokens) < 1 or tokens[0] == '{':
      continue
    tokenized_lines.append(tokens)
  assembly_c = ILAssembly(tokenized_lines).as_c_fragment()

c = """
#include "stackmachine.h"

%s

int main() {
  %s
  assembly_t assemblies[] = {
    .items = %s
  };
  assembly_vector_t assembly_vector = {
    .items = assemblies,
    .count = 1
  };
  runApplication(assembly_vector, 0);
}
""" % (
  assembly_c.get_fn_defs(),
  assembly_c.get_before(),
  assembly_c.text
)

c_path = os.path.join(aot_dir, project_name + '.c')
with open(c_path, 'w') as c_file:
  c_file.write(c)

try:
  subprocess.call(['clang-format', '-i', c_path])
except:
  print('Warning: clang-format is required to prettify the emitted C code')
  pass

print('Assembly IL transpiled to %s' % (c_path))

if args.format == 'c':
  if not args.output == None:
    copyfile(c_path, args.output)
  exit(0)

bin_path = os.path.join(aot_dir, project_name + '.bin')
subprocess.call(['gcc', '-Ilibaottool/include', '-o', bin_path, c_path])
