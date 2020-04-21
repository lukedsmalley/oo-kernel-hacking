from .common import Element
from .method_element import MethodElement


class FieldElement(Element):
  def __init__(self, lines, start):
    Element.__init__(self, lines[start][1:], {
      0: 'type',
      1: 'name'
    }, ['private', 'hidebysig'])
    self.end = start


class ClassElement(Element):
  def __init__(self, lines, start):
    # Call parent constructor
    # private, auto, etc are flags
    # With flags and .class ignored, class name is the
    # first token encountered (#0), 'extends' is #1,
    # parent class name is #2.
    Element.__init__(self, lines[start][1:], {
      0: 'name',
      2: 'superclass'
    }, ['private', 'auto', 'ansi', 'beforefieldinit'])

    self.fields = []
    self.methods = []

    # Consume fields and methods from lines until '}' is reached
    start += 1
    while start < len(lines):
      token = lines[start][0]
      if token == '}':
        break
      elif token == '.field':
        self.fields.append(FieldElement(lines, start))
        start = self.fields[-1].end
      elif token == '.method':
        self.methods.append(MethodElement(lines, start))
        start = self.methods[-1].end
      start += 1
    # Set self.end so that the parent Element knows at what line
    # the class stops
    self.end = start

