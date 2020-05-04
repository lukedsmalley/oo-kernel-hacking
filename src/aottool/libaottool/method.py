from .common import ILElement, split_tokens
from .parameter import ILParameter
from .fragment import CFragment, CVectorizedString, CVectorizedArray, CBlock
from .local import ILLocal
from .block import ILBlock

class ILMethod(ILElement):
  next_label_number = 0

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
