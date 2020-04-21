from .class_element import ClassElement

# Takes an array of arrays of strings (tokens) as
# for only parameter 'lines'. In other words, tokenize
# input prior to this.
class DocumentElement:
  def __init__(self, lines):
    # Document contains classes
    self.classes = [] 

    # Loop over all lines of the document
    i = 0 
    while i < len(lines):
      # If a line begins a class, consume the class with
      # ClassElement, then fast-forward to the end of
      # the element
      if lines[i][0] == '.class':
        self.classes.append(ClassElement(lines, i))
        i = self.classes[-1].end
      i += 1

