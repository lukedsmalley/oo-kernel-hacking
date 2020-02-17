const { readFile, lstat } = require('fs-extra')
const { join, parse } = require('path')

async function preprocessLibInclude(name, includePaths, macros={}, visitedFiles=[]) {
  for (const includePath of includePaths) {
    const path = join(includePath, name)
    let stat = {
      isFile() {
        return
      }
    }
    try {
      stat = await lstat(path)
    } catch { }
    if (stat.isFile()) {
      console.group()
      await preprocess(path, includePaths, macros, visitedFiles)
      console.groupEnd()
      return
    }
  }
  throw new Error(`No file ${name} found in include directories`)
}

const INCLUDE_LIB_PREFIX = '#include <'
const INCLUDE_FILE_PREFIX = '#include "'
const IFDEF_PREFIX = '#ifdef '
const ENDIF_PREFIX = '#endif'
const DEFINE_PREFIX = '#define '

class State {
  onLine() { }
  onExit() { }
}

async function preprocess(path, includePaths, macros={}, visitedFiles=[]) {
  console.log(path)
  if (visitedFiles.indexOf(path) >= 0) {
    return
  }

  const lines = await (await readFile(path, { encoding: 'utf8' })).split('\n')

  class IfDefSkipState extends State {
    depth = 0

    onLine(line) {
      if (line.startsWith(IFDEF_PREFIX)) {
        this.depth++
        return this
      } else if (line.startsWith(ENDIF_PREFIX)) {
        if (this.depth < 1) {
          return new DefaultState()
        }
        this.depth--
        return this
      }
    }

    onExit(lineNumber) {
      if (depth > 0) {
        throw new Error(`#ifdef without #endif in ${path} at line ${lineNumber}`)
      }
    }
  }

  class DefaultState extends State {
    async onLine(line) {
      if (line.startsWith(INCLUDE_LIB_PREFIX)) {
        const name = line.split('>')[0].substring(INCLUDE_LIB_PREFIX.length)
        await preprocessLibInclude(name, includePaths, macros, visitedFiles)
      } else if (line.startsWith(INCLUDE_FILE_PREFIX)) {
        const name = line.substring(INCLUDE_FILE_PREFIX.length).split('"')[0]
        console.group()
        await preprocess(join(parse(path).dir, name), includePaths, macros, visitedFiles)
        console.groupEnd()
      } else if (line.startsWith(IFDEF_PREFIX)) {
        return new IfDefSkipState()
      } else if (line.startsWith(ENDIF_PREFIX)) {
        throw new Error(`#endif without #ifdef in ${path}`)
      } else if (line.startsWith(DEFINE_PREFIX)) {
        const name = line.substring(DEFINE_PREFIX.length).split(' ')[0]
        if (name.indexOf('(') >= 0) {
          const value = line.substring(DEFINE_PREFIX.length + name.length + 1)
          macros[name] = value
        }
      }
      return this
    }
  }

  let state = new DefaultState()
  let i = 0
  for (; i < lines.length; i++) {
    const nextState = await state.onLine(lines[i], i)
    if (nextState !== state) {
      await state.onExit(i)
      state = nextState
    }
  }
  await state.onExit(i)
}

async function main() {
  const macros = {}
  await preprocess('src/linux/init/main.c', [
    'build/linux/arch/x86/include/generated/uapi',
    'src/linux/arch/x86/include/uapi',
    'build/linux/arch/x86/include/generated',
    'build/linux/arch/x86/include',
    'src/linux/arch/x86/include',
    'build/linux/include/generated/uapi',
    'src/linux/include/uapi',
    'build/linux/include/generated',
    'build/linux/include',
    'src/linux/include',
  ], macros)
  console.log()
  console.log(macros)
}

main().catch(err => console.log(`Unhandled ${err}`))
