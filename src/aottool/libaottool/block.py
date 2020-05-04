from .common import ILElement, split_tokens
from .local import ILLocal
from .instructions import ILInstruction, ILNoOperation
from .fragment import CFragment

class ILBlock(ILElement):
  IGNORED_INSTRUCTIONS = ['.maxstack']
  INSTRUCTION_CONSTRUCTORS = {
    'nop': ILNoOperation
  }

  def __init__(self, lines, start):
    self.locals = []
    self.entry_point = False
    self.instructions = []
    while start < len(lines):
      line = lines[start]
      if line[0] in ILBlock.IGNORED_INSTRUCTIONS:
        start += 1
        continue
      if line[0] == '}':
        break
      elif line[0] == '.entrypoint':
        self.entry_point = True
      elif line[0] == '.try':
        self.instructions.append(ILTry(lines, start + 1))
        self.locals.extend(self.instructions[-1].locals)
        start = self.instructions[-1].end
      elif line[0:2] == ['.locals', 'init']:
        init_parts = split_tokens(line, ['(', ')'])
        if init_parts[1] != []:
          self.locals.extend(ILLocal(l) for l in split_tokens(init_parts[1], [',']))
      else:
        label = line[0][:-1] if line[0][-1] == ':' else None
        opcode_index = 1 if line[0][-1] == ':' else 0
        opcode_parts = line[opcode_index].split('.')
        opcode = opcode_parts[0]
        args = opcode_parts[1:] + line[opcode_index + 1:]
        if opcode in ILBlock.INSTRUCTION_CONSTRUCTORS:
          self.instructions.append(ILBlock.INSTRUCTION_CONSTRUCTORS[opcode](label, args))
        else:
          print('NYI op %s will be emitted as no-op' % (opcode))
      start += 1
    if start >= len(lines):
      raise Exception('Unterminated block')
    self.end = start

  def as_c_fragment(self, class_id, method_id, catch_branch=None):
    c_statements = []
    for instruction in self.instructions:
      args = [catch_branch]
      if isinstance(instruction, ILTry):
        args = [class_id, method_id, catch_branch]
      c_statements.append(instruction.as_c_fragment(*args))
    return CFragment(
      entry_class=class_id if self.entry_point else None,
      entry_method=method_id if self.entry_point else None,
      text=';'.join(list(s.text for s in c_statements) + ['']))


class ILTry(ILElement):
  next_try_number = 0

  def __init__(self, lines, start):
    self.locals = []
    self.block = ILBlock(lines, start)
    self.locals.extend(self.block.locals)
    if self.block.end + 2 >= len(lines) or lines[self.block.end + 1][0] != 'catch':
      raise Exception('Try block without catch block')
    self.type = lines[self.block.end + 1][1]
    self.catch = ILBlock(lines, self.block.end + 2)
    self.locals.extend(self.catch.locals)
    self.end = self.catch.end

  def as_c_fragment(self, class_id, method_id, outer_catch_branch=None):
    try_id = 'catch' + str(ILTry.next_try_number)
    ILTry.next_try_number += 1
    catch_branch = CFragment(
      text='if (sm_caught(%0s)) goto %1s',
      format_vars=[self.type, try_id])
    return CFragment(
      text="""
        %0s
        goto %1s_end;
        %1s:
        %2s
        %1s_end:
        sw_nop()
      """,
      format_vars=[
        self.block.as_c_fragment(class_id, method_id, catch_branch),
        try_id,
        self.catch.as_c_fragment(class_id, method_id, outer_catch_branch),
      ])
