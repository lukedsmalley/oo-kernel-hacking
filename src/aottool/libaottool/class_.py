from .common import ILElement
from .method import ILMethod
from .field import ILField
from .fragment import CFragment, CVectorizedArray, CVectorizedString

class ILClass(ILElement):
  def __init__(self, lines, start):
    ILElement.__init__(self, lines[start][1:], {
      0: 'name',
      2: 'superclass'
    }, ['private', 'auto', 'ansi', 'beforefieldinit'])
    self.fields = []
    self.methods = []

    start += 1
    while start < len(lines):
      token = lines[start][0]
      if token == '}':
        break
      elif token == '.field':
        self.fields.append(ILField(lines[start]))
      elif token == '.method':
        self.methods.append(ILMethod(lines, start))
        start = self.methods[-1].end
      start += 1
    if start >= len(lines):
      raise Exception('Unterminated class')
    self.end = start

  def as_c_fragment(self, assembly, id):
    return CFragment(
      text=""" {
          .name = %0s,
          .superclass = %1s,
          .fields = %2s,
          .methods = %3s
        } """,
      format_vars=[
        CVectorizedString(self.name),
        assembly.get_type_by_name(self.superclass),
        CVectorizedArray(
          'field_vector_t',
          list(f.as_c_fragment(assembly) for f in self.fields)),
        CVectorizedArray(
          'method_vector_t',
          list(self.methods[i].as_c_fragment(assembly, id, i)
            for i in range(len(self.methods))))
      ])
