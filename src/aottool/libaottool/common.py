class ILElement:
  def __init__(self, tokens, positionals, flags):
    for flag in flags:
      self.__dict__[flag] = False
    i = 0
    for token in tokens:
      if token in flags:
        self.__dict__[token] = True
        continue
      elif i in positionals:
        self.__dict__[positionals[i]] = token
      i += 1


def split_tokens(tokens, delimeters):
  results = [[]]
  for token in tokens:
    if token in delimeters:
      results.append([])
    else:
      results[-1].append(token)
  return results


def split_string(string, delimeters):
  delimeters.sort(reverse=True, key=len)
  results = [string]
  for delimeter in delimeters:
    for i in range(len(results)):
      bottom = results[0]
      results = results[1:]
      results.append(bottom.split(delimeter))
  return results


def error_if(condition, message):
  if condition:
    print('\u001b[33m' + message + '\u001b[0m')
    exit(1)


def warn_if(condition, message):
  if condition:
    print('\u001b[33m' + message + '\u001b[0m')
    exit(1)
