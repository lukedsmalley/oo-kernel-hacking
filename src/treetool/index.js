const { readdir, lstat, readFile, writeFile, readJSON } = require('fs-extra')
const { join } = require('path')
const { createHash } = require('crypto')
const { redBright, blueBright, greenBright } = require('chalk')

async function getSHA(path) { 
  const hash = createHash('sha512')
  hash.update(await readFile(path))
  return hash.digest('base64')
}

async function getTree(path) {
  const tree = {
    size: 0,
    fileCount: 0,
    directoryCount: 0,
    files: {},
    directories: {}
  }
  for (const name of await readdir(path)) {
    const entry = join(path, name)
    const stat = await lstat(entry)
    tree.size += stat.size
    if (stat.isDirectory()) {
      const child = await getTree(entry)
      tree.size += child.size
      tree.fileCount += child.fileCount
      tree.directoryCount += child.directoryCount + 1
      tree.directories[name] = child
    } else {
      try {
        const hash = await getSHA(entry)
        tree.fileCount++
        tree.files[name] = {
          size: stat.size,
          hash
        }
      } catch (err) {
        console.log(`Failed to compute hash for ${join(path, name)} due to ${err}`)
      }
    }
  }
  return tree
}

function formatCount(n) {
  n = String(n)
  formattedN = ''
  for (let i = n.length; i--; ) {
    formattedN = n.charAt(i) + formattedN
    if ((n.length - i) % 3 === 0 && i > 0) {
      formattedN = ',' + formattedN
    }
  }
  return formattedN
}

function compareTrees(from, to, base='') {
  for (const name in from.files) {
    if (!to.files[name]) {
      console.log(`${redBright('-')}  F  ${join(base, name)}`)
    } else if (to.files[name].size !== from.files[name].size
        || to.files[name].hash !== from.files[name].hash) {
      console.log(`${blueBright('>')}  F  ${join(base, name)}`)
    }
  }

  for (const name in to.files) {
    if (!from.files[name]) {
      console.log(`${greenBright('+')}  F  ${join(base, name)}`)
    }
  }

  for (const name in from.directories) {
    if (!to.directories[name]) {
      console.log(`${redBright('-')}  D  ${join(base, name)}`)
    } else {
      compareTrees(from.directories[name], to.directories[name], join(base, name))
    }
  }

  for (const name in to.directories) {
    if (!from.directories[name]) {
      console.log(`${greenBright('+')}  D  ${join(base, name)}`)
    }
  }
}

async function main() {
  if (process.argv.length < 3) {
    console.log('Expected treetool command ("snapshot" or "compare")')
    console.log()
    console.log('Usage: treetool (snapshot|s) [<source-dir>] <snap-file-dest>')
    console.log('       treetool (compare|c) <from-tree-file> <to-tree-file>')
  } else if ('snapshot'.startsWith(process.argv[2])) {
    if (process.argv.length < 4) {
      console.log('Expected source directory and/or snapshot destination')
      console.log()
      console.log('Usage: treetool (snapshot|s) [<source-dir>] <snap-file-dest>')
    } else {
      const destination = process.argv[process.argv.length < 5 ? 3 : 4]
      const source = process.argv.length < 5 ? '.' : process.argv[3]
      const tree = await getTree(source)
      await writeFile(destination, JSON.stringify(tree, null, 2), { encoding: 'utf8' })
      console.log(`Snapshot of ${source} saved into ${destination}`)
      console.log(`with ${tree.fileCount} files and ${tree.directoryCount} directories found (${formatCount(tree.size)} bytes)`)
    }
  } else if ('compare'.startsWith(process.argv[2])) {
    if (process.argv.length < 5) {
      console.log('Expected two snapshot file paths')
      console.log()
      console.log('Usage: treetool (compare|c) <from-snap-file> <to-snap-file>')
    } else {
      compareTrees(
        await readJSON(process.argv[3], { encoding: 'utf8' }),
        await readJSON(process.argv[4], { encoding: 'utf8' }))
    }
  } else {
    console.log('Expected treetool command ("snapshot" or "compare")')
    console.log()
    console.log('Usage: treetool (snapshot|s) [<source-dir>] <snap-file-dest>')
    console.log('       treetool (compare|c) <from-tree-file> <to-tree-file>')
  }
  console.log()
}

main().catch(err => console.log(`Unhandled ${err}`))
