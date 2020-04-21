# Class whose constructor parses the takens it's given into
# instance properties automatically. This used as a base 
# class for classes representing parts of the program 
# (.class, .method). This constructor is called using the
# tokens from the declaration line and the child constructor
# handles all subsequent lines.

# 'positionals' is a dictionary of numbers to property names
# 'flags' is a list of strings

# The resulting object will have properties for each flag,
# either True or False based on whether the flag was 
# encountered in the line. The resulting object will also
# have properties for each property named in 'positionals'.
# The value of these properties is that of the token at
# the index given for the property in 'positionals'.
# *Flag tokens do not count toward this index.*

# See ClassElement or MethodElement for usage examples
class Element:
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

# Exactly like splitting strings, but splits an array of strings
# instead. 'delimiters' is also list of strings. Tokens will be
# split when *any one of* of the delimiters are encountered.
def split_tokens(tokens, delimeters):
  results = [[]]
  for token in tokens:
    if token in delimeters:
      results.append([])
    else:
      results[-1].append(token)
  return results
