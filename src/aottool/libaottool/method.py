from .common import ILElement, split_tokens
from .parameter import ILParameter
from .fragment import CFragment, CVectorizedString, CVectorizedArray, CBlock
from .local import ILLocal
from .block import ILBlock

class ILMethod(ILElement):
  next_label_number = 0

  def _process_try(self, lines, start, catch_label=None):
    next_catch_label = 'catch_%d' % (ILMethod.next_label_number)
    end_label = 'end_' + next_catch_label
    ILMethod.next_label_number += 1
    self._process_instructions(lines, start, next_catch_label)
    if self.end + 2 >= len(lines):
      raise Exception('Try block without catch')
    if lines[self.end + 1][0] != 'catch':
      raise Exception('Try block without catch block')
    self.instructions.append(ILBranch(end_label))
    self.instructions.append(ILLabel(catch_label))
    self._process_instructions(lines, self.end + 2, catch_label)
    self.instructions.append(ILLabel(end_label))

  def _process_instructions(self, lines, start, catch_label=None):
    while start < len(lines):
      line = lines[start]
      if line[0] in ILInstruction.IGNORED:
        start += 1
        continue
      if line[0] == '}':
        break
      elif line[0] == '.entrypoint':
        self.entry_point = True
      elif line[0] == '.try':
        self._process_try(lines, start + 1, catch_label)
      elif line[0:2] == ['.locals', 'init']:
        init_parts = split_tokens(lines[start], ['(', ')'])
        self.locals = list(ILLocal(l) for l in split_tokens(init_parts[1], [',']))
      else:
        self.instructions.append(ILInstruction(lines[start], catch_label))
      start += 1
    if start >= len(lines):
      raise Exception('Unterminated method')
    self.end = start

  def __init__(self, lines, start):
    decl_parts = split_tokens(lines[start][1:], ['(', ')'])
    ILElement.__init__(self, decl_parts[0], {
      0: 'type',
      1: 'name'
    }, ['private', 'hidebysig', 'static', 'default', 'cil', 'managed'])
    
    self.parameters = list(ILParameter(t) for t in split_tokens(decl_parts[1], [','])) \
      if decl_parts[1] != [] \
      else []
    self.block = ILBlock(lines, start + 1)
    self.locals = self.block.locals
    self.end = self.block.end
    
  def as_c_fragment(self, assembly, class_id, id):
    return CFragment(
      text=""" {
          .name = %0s,
          .type = %1s,
          .parameters = %2s,
          .locals = %3s
          .body = %0x
        } """,
      fn_defs='call_result_t %0x(call_stack_t stack) { %4s }',
      format_vars=[
        CVectorizedString(self.name),
        assembly.get_type_by_name(self.type),
        CVectorizedArray(
          'field_vector_t',
          list(p.as_c_fragment(assembly) for p in self.parameters)),
        CVectorizedArray(
          'field_vector_t',
          list(l.as_c_fragment(assembly) for l in self.locals)),
        self.block.as_c_fragment(class_id, id)
      ])
