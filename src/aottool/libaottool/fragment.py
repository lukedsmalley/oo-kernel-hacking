def is_number(string):
  try:
    int(string)
    return True
  except:
    return False


class CFragment:
  next_id_number = 0

  def _format(self, fmt):
    if fmt == None:
      return None
    i = 0
    while i < len(fmt) - 2:
      if fmt[i] == '%':
        if fmt[i + 1] == '%':
          fmt = fmt[0:i] + '%' + fmt[i + 2:]
        elif is_number(fmt[i + 1]):
          if fmt[i + 2] == 's':
            fmt = fmt[0:i] \
              + self.format_vars[int(fmt[i + 1])] \
              + fmt[i + 3:]
          elif fmt[i + 2] == 'x':
            fmt = fmt[0:i] \
              + self.id + '_' + fmt[i + 1] \
              + fmt[i + 3:]
          i += 1
        i += 1
      i += 1
    return fmt

  def __init__(self, text=None, before=None, fn_defs=None, entry_class=None,
      entry_method=None, format_vars=[], deps=[]):
    self.id = 'a' + str(CFragment.next_id_number)
    CFragment.next_id_number += 1

    self.entry_class = None
    self.entry_method = None
    self.deps = []
    self.format_vars = []

    for var in format_vars:
      if isinstance(var, CFragment):
        self.deps.append(var)
        self.format_vars.append(var.text)
        if var.entry_class != None: self.entry_class = var.entry_class
        if var.entry_method != None: self.entry_method = var.entry_method
      else:
        self.format_vars.append(str(var))

    if entry_class != None: self.entry_class = entry_class
    if entry_method != None: self.entry_method = entry_method
    self.deps += deps
    self.text = self._format(text) if text != None else None
    self.before = self._format(before) if before != None else None
    self.fn_defs = self._format(fn_defs) if fn_defs != None else None

  def get_before(self):
    lines = list(d.get_before() for d in self.deps) + [self.before]
    lines = list(l for l in lines if l != None)
    return None if len(lines) < 1 else '\n'.join(lines)
  
  def get_fn_defs(self, calls=0):
    if self.id == 'a1':
      print(', '.join(d.id for d in self.deps))
    if calls > 10:
      exit(0)
    lines = list(d.get_fn_defs(calls + 1) for d in self.deps) + [self.fn_defs]
    lines = list(l for l in lines if l != None)
    return None if len(lines) < 1 else '\n'.join(lines)


class CVectorizedArray(CFragment):
  def __init__(self, vector_type, values):
    deps = []
    for i in range(len(values)):
      if isinstance(values[i], CFragment):
        deps.append(values[i])
        values[i] = values[i].text
      
    CFragment.__init__(self,
      text=""" {
        .items = %0x,
        .count = %0s
      } """,
      before='%1s %0x[] = { %2s };',
      format_vars=[len(values), vector_type, ', '.join(values)],
      deps=deps)


class CVectorizedString(CFragment):
  def __init__(self, string):
    CFragment.__init__(self,
      text=""" {
        .items = %0x,
        .count = %0s
      } """,
      before='uint8_t %0x[] = "%1s";',
      format_vars=[len(string), string])


class CBlock(CFragment):
  def __init__(self, statements, entry_point=None):
    deps = []
    for i in range(len(statements)):
      if isinstance(statements[i], CFragment):
        deps.append(statements[i])
        statements[i] = statements[i].text
      
    CFragment.__init__(self,
      text='%0s',
      format_vars=['; '.join(statements + [''])],
      deps=deps)
