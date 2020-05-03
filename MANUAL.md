<div align="center">
  <h1>The Manual</h1>
  <p>for the reserach implementation of</p>
  <h3>Object-oriented virtual execution as a foundation for operating system design</h3>
</div>

<div align="center">
  <h2>Project Contents</h2>
</div>

The project is organized as a set of subprojects (located in `src`) and scripts used to compile everything into the final product, which is a bootable operating system. Some of the subprojects are cloned from separate open-source projects. Subprojects include:
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

