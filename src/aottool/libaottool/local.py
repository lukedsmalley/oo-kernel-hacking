from .common import ILElement
from .fragment import CFragment

class ILLocal(ILElement):
  def __init__(self, tokens):
    ILElement.__init__(self, tokens, {
      0: 'type',
      1: 'name'
    }, [])

  def as_c_fragment(self, assembly):
    return CFragment(
      text=""" {
          .name = {
            .items = %0x,
            .count = %1d
          },
          .type = %0s
        } """,
      before='char %0x[] = %1s;',
      format_vars=[
        assembly.get_type_by_name(self.type),
        self.name
      ])
