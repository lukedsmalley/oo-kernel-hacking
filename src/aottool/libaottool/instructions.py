from .common import ILElement
from .fragment import CFragment

class ILInstruction(ILElement):
  def __init__(self, label):
    self.label = label

  def as_c_fragment(self, catch_branch):
    return CFragment(
      text='%1s' if self.label == None else '%0s: %1s',
      format_vars=[self.label, self._get_statement(catch_branch)])


class ILNoOperation(ILInstruction):
  def __init__(self, label, args):
    ILInstruction.__init__(self, label)

  def _get_statement(self, catch_branch):
    return 'sm_nop()'


class ILLoadString(ILInstruction):
  def __init__(self, label, args):
    ILInstruction.__init__(self, label)
    self.string = args[0]

  def _get_statement(self, catch_branch):
    return CFragment(
      text='char %0x[] = "%0s"; sm_ldstr(%0x, sizeof(%0x))',
      format_vars=[self.string])
