def lex(line):
  # If 'span' is a character, the lexer is inside a literal
  # span ending with that character. If 'span' is None, the
  # lexer is not inside a literal string, char, etc.
  span = None
  tokens = ['']
  
  for i in range(len(line)):
    # If not inside a literal...
    if span == None:
      # Skip single line comment
      # I haven't seen a multiline comment from ILDAsm yet
      if i + 1 < len(line) and line[i] == '/' and line[i + 1] == '/':
        if len(tokens[-1]) < 1:
          del tokens[-1]
        return tokens
      # Parenthesis are added as independent tokens
      elif line[i] == '(' or line[i] == ')':
        if len(tokens[-1]) > 0:
          tokens.append('')
        tokens[-1] += line[i]
        if len(tokens[-1]) > 0:
          tokens.append('')
      # Double and single quotes start a literal span
      elif line[i] == '"' or line[i] == '\'':
        tokens[-1] += line[i]
        span = line[i]
      # Space and newline end current token
      elif line[i] == ' ' or line[i] == '\n':
        if len(tokens[-1]) > 0:
          tokens.append('')
      # Everything else is concatenated with current token
      else:
        tokens[-1] += line[i]
    # If inside a literal...
    else:
      tokens[-1] += line[i]
      if line[i] == span:
        span = None

  if len(tokens[-1]) < 1:
    del tokens[-1]

  return tokens
