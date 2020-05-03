from .class_ import ILClass
from .fragment import CFragment, CVectorizedArray

class ILAssembly:
  def __init__(self, lines):
    self.classes = []
    i = 0
    while i < len(lines):
      if lines[i][0] == '.class':
        self.classes.append(ILClass(lines, i))
        i = self.classes[-1].end
      i += 1

  PRIMITIVE_TYPE_IDS = {
    'void': 0,
    'int32': 1,
    'string[]': 2
  }

  def get_type_by_name(self, name):
    if name in ILAssembly.PRIMITIVE_TYPE_IDS:
      return '{ .assembly = 1, .class = %d }' \
        % (ILAssembly.PRIMITIVE_TYPE_IDS[name])
    for class_ in self.classes:
      if class_.name == name:
        return '{ .assembly = 0, .class = %d }' \
          % (self.classes.index(class_))
    return '{ .assembly = 0, .class = 0 }'
    #raise Exception('Undefined class %s' % (name))

  def as_c_fragment(self):
    class_vector = CVectorizedArray(
      'class_vector_t',
      list(self.classes[i].as_c_fragment(self, i)
        for i in range(len(self.classes))))
    return CFragment(
      text=""" {
          .classes = %0s,
          .entryClass = %1s,
          .entryMethod = %2s
        } """,
      format_vars=[
        class_vector,
        class_vector.entry_class,
        class_vector.entry_method
      ])
