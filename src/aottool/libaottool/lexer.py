def lex_il_line(line):
  span = None
  tokens = ['']
  for i in range(0, len(line)):
    if span == None:
      if i + 1 < len(line) and line[i] == '/' and line[i + 1] == '/':
        if len(tokens[-1]) < 1:
          del tokens[-1]
        return tokens
      elif line[i] == '(' or line[i] == ')' or line[i] == ',':
        if len(tokens[-1]) > 0:
          tokens.append('')
        tokens[-1] += line[i]
        if len(tokens[-1]) > 0:
          tokens.append('')
      elif line[i] == '"' or line[i] == '\'':
        span = line[i]
      elif line[i] == ' ' or line[i] == '\n':
        if len(tokens[-1]) > 0:
          tokens.append('')
      else:
        tokens[-1] += line[i]
    elif line[i] == span:
      span = None
    else:
      tokens[-1] += line[i]
  if len(tokens[-1]) < 1:
    del tokens[-1]
  return tokens
