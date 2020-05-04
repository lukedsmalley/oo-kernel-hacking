<div align="center">
  <h1>The Manual</h1>
  <p>for the research implementation of</p>
  <h3>Object-oriented virtual execution as a foundation for operating system design</h3>
</div>

<div align="center">
  <h2>Project Organization</h2>
</div>

The project is organized as a set of sub-projects (located in `src`) and scripts used to compile everything into the final product, which is a bootable operating system. Some of the sub-projects are cloned from separate open-source projects. Sub-projects include:
* `linux` - (Cloned) The kernel that we want to modify to include a virtual execution system and loader for binaries targeting that execution system. This is version 5.5.2 of the kernel source with as many driver modules removed as possible.
* `busybox` - (Cloned) Sources for a single-binary Linux userland. We cannot boot the kernel without a userland program to launch, and we need this to act as a placeholder until our runtime is completed.
* `initramfs` - Files which are combined with `busybox` to produce the initial RAMdisk to be loaded alongside the kernel image for booting.
* `treetool` - A directory diff tool that was written to assist in manipulating the kernel source code.
* `preprocesstool` - A C preprocessor intended to further simplify the kernel sources. It was not finished due to prioritization of the runtime development and the complexity of the C preprocessor.
* `stackmachine` - The virtual execution system to be integrated into the kernel.
* `ciltool` - Transpiler to convert .NET assemblies (binaries) into simplified `stackmachine`-compatible binaries. This is to be used to compile the virtualized API and hooks into the kernel API.
* `aottool` - Transpiler to convert .NET assemblies into Linux applications (using `stackmachine` as a framework). This was created to assist in the development of `stackmachine` by allowing programs to be run without constantly adjusting the program loader.

<div align="center">
  <h2>System Requirements</h2>
</div>

Dependencies touched by the main build scripts:
* A Unix-based host system and Bash shell is required by
  * Everything
* The GNU C Compiler is required by
  * `linux`
  * `busybox`
  * `stackmachine`
  * `aottool`
* GNU build tools (or `build-essential`) and [other things](https://www.kernel.org/doc/html/v4.15/process/changes.html#minimal-requirements-to-compile-the-kernel) are required by
  * `linux`
  * `busybox`
* The QEMU emulator for the x86-64 platform is required by
  * Main build (`start-qemu.sh`)

Dependencies for projects not yet part of the build pipeline (Main build can be run without these):
* Node.js (current LTS version suggested) is required by
  * `treetool`
  * `preprocesstool`
* The Yarn package manager (classic) is required by
  * `treetool`
  * `preprocesstool`
* Python 3 is required by
  * `ciltool`
  * `aottool`
* .NET Core 3.1 is required by
  * `ciltool`
  * `aottool`
* [`dotnet-ildasm`](https://www.nuget.org/packages/dotnet-ildasm/) is required by
  * `ciltool`
  * `aottool`

Optional dependencies:
* `clang-format` can be used by
  * `aottool` to format emitted C source code

<div align="center">
  <h2>Main Build Usage</h2>
</div>

To build and run the bootable system in the QEMU emulator, run the main build scripts (present in the root project folder) in order:
1. Run `setup.sh` to configure the `linux` and `busybox` sources.
2. Run `build.sh` to compile the project. All arguments passed to `build.sh` get passed to `make`, and it's a really good idea to increase the job count with `-j4` or the like.
3. Run `start-qemu.sh` to launch the bootable system in QEMU. Use `Ctrl-A X` to terminate the emulator.

After the first build, `setup.sh` does not need to be run again, unless you want to do a complete rebuild. To do a clean rebuild, run `clean.sh` and then repeat the process above.

To run the build process with `treetool` diff output (requires Node.js and Yarn), use `*-and-diff.sh` versions of `setup.sh` and `build.sh`.

<div align="center">
  <h2><code>treetool</code> Usage</h2>
</div>

`treetool` is a tool that records directory contents into files and generates diffs between those files. It was created to aid the process of removing code from the Linux kernel that wouldn't actually be used.

Running `treetool` requires Node.js and the Yarn package manager. The Node LTS version was used to develop the software.

To run treetool, invoke the `treetool` script in the project root directory.

Usage for saving directory contents:
```
[./]treetool (snapshot|s) [<source-dir>] <snap-file-dest>
```

Usage for comparing directory trees:
```
[./]treetool (compare|c) <from-tree-file> <to-tree-file>
```

Running `treetool` without arguments will also display this usage information.

<div align="center">
  <h2><code>preprocesstool</code> Usage</h2>
</div>

`preprocesstool` is an unfinished C preprocessor. It is invoked through the launcher in the project root folder and has no parameters. It does not do anything besides log internal state.

<div align="center">
  <h2><code>stackmachine</code> Usage</h2>
</div>

<div align="center">
  <h2><code>ciltool</code> Usage</h2>
</div>

Running `ciltool` requires Python 3, the .NET Core 3.1 SDK, and [`dotnet-ildasm`](https://www.nuget.org/packages/dotnet-ildasm/).

The .NET Core SDK can be downloaded on Ubuntu by using one of the following two methods:

```
wget https://packages.microsoft.com/config/ubuntu/19.10/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb
```

```
sudo apt-get update
sudo apt-get install apt-transport-https
sudo apt-get update
sudo apt-get install dotnet-sdk-3.1
```

This will probably take a little while to download. The other necessary software is the dotnet IL disassembler, which can be installed with the command:

```    
dotnet tool install -g dotnet-ildasm
```

At this point, you’ll need a sample .NET project to work with. The easiest sample project would be a “Hello, world!” project, which can be created and navigated to like so:

```
dotnet new console -o myApp
cd myApp
```

Then, to run the project:

```
dotnet run
```

Command usage for `ciltool` is as follows:

```
[./]ciltool <build-path>
```

The build path is the direct path to your sample project’s primary directory. So, for instance, I would run the tool like so:

```    
./ciltool /home/aaronm/Documents/os_project/myApp
```

If your system does not have `python3` linked to your Python 3.x installation, you may need to run `ciltool` without the convenience launcher instead:

```
python src/ciltool/ciltool.py <build-path>
```

If you have input a valid build path, the tool should build the project with 0 warnings and errors. It will then prompt you for the build file path, which is the direct path to your sample project’s DLL file. So, for instance, I would input:

```
/home/aaronm/Documents/os_project/myApp/bin/Debug/netcoreapp3.1/myApp.dll
```

Now you will be prompted for the output file path, or the direct path to the IL file you want to create. For the sake of ease, I put the IL file in the same directory as the DLL file, so I input the following:

```
/home/aaronm/Documents/os_project/myApp/bin/Debug/netcoreapp3.1/myApp.il
```

Finally, you will be prompted for the direct path where you would like to create the translated file. At this point, the `ciltool` in itself merely removes comments from the IL file and copies its contents to the translated file you specify. For the purposes of having a file to iterate through to translate into another bytecode file, I made the translated file a .txt file. The following path is what I input when prompted:

```
/home/aaronm/Documents/os_project/myApp/bin/Debug/netcoreapp3.1/translated_file.txt
```

The script will then terminate. Now, if you navigate to the directory that your translated file is in and look at it, you will see that it contains the contents of the IL file, with comments removed.

<div align="center">
  <h2><code>aottool</code> Usage</h2>
</div>

`aottool` is a transpiler that converts .NET assemblies into standalone Linux applications by transpiling .NET bytecode into C code consisting primarily of calls into `stackmachine` functions. This tool was developed as a way to work incrementally through developing `stackmachine` functions without having to also work on an application loader.

Running `aottool` requires Python 3, the .NET Core 3.1 SDK, `dotnet-ildasm`, and `gcc`. If `clang-format` is installed, it will format emitted C source code.

Invoke `aottool` from within a .NET project folder to build the project as a standalone Linux application. All interstitial files, including IL code, generated C code, and `dotnet build` output will be placed in `<project-path>/aot`.
```
[path-to/]aottool [-f <cil|c|bin>] [-o <output-file>] [<project-path>]
```

Project path defaults to the current working directory.

The `-f` (format) flag can be used to stop compilation after either IL Disassembly or C code generation. This defaults to `bin`, a standalone binary.

The `-o` flag can be used to change the destination of the output file. This defaults to `<project-path>/aot/<project-name>.<format>`, where `<format>` is the format selected by `-f`.

In its current state of development, the tool will emit C method bodies with `stackmachine` `nop` calls in the place of unrecognized instructions (which is most instructions).
