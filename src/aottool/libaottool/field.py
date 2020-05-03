from .common import ILElement
from .fragment import CFragment, CVectorizedString


class ILField(ILElement):
  def __init__(self, line):
    ILElement.__init__(self, line[1:], {
      0: 'type',
      1: 'name'
    }, ['private', 'hidebysig', 'class'])

  def as_c_fragment(self, assembly):
    return CFragment(
      text=""" {
          .name = %0s,
          .type = %1s
        } """,
      format_vars=[
        CVectorizedString(self.name),
        assembly.get_type_by_name(self.type)
      ])
