from .common import Element, split_tokens


def parse_instruction(tokens):
  if tokens[0][-1] == ':':
    print('Ignored instruction %s' % (tokens[1]))
  else:
    print('Ignored instruction %s' % (tokens[0]))


class MethodElement(Element):
  def _parse_args(self, tokens, args):
    for arg_tokens in split_tokens(tokens, [',']):
      args.append(ArgumentElement(arg_tokens))

  def __init__(self, lines, start):
    # Split line into declaration (decl_parts[0])
    # and args (decl_parts[1]) by the parentheses
    decl_parts = split_tokens(lines[start][1:], ['(', ')'])

    Element.__init__(self, decl_parts[0], {
      0: 'type',
      1: 'name'
    }, ['private', 'hidebysig', 'static', 'default', 'cil', 'managed'])
    
    self.instructions = []

    self.args = []
    for arg_tokens in split_tokens(decl_parts[1], [',']):
      self.args.append(Element(arg_tokens, {
        0: 'type',
        1: 'name'
      }, []))

    start += 1
    while start < len(lines):
      if lines[start][0] == '}':
        break
      else:
        parse_instruction(lines[start])
      start += 1
    self.end = start
