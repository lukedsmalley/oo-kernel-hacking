<div align="center">
  <h1>Object-oriented Kernel Hacking</h1>
  <p>Formally referred to as the research implementation of:</p>
  <h3>Object-oriented virtual execution as a foundation for operating system design</h3>
  <p>This is a Linux kernel workspace for term research in GSU's CSC 4320 (Operating Systems) course. Yet another attempt at marrying an intermediate runtime that has first-class support for object orientation with the operating system for great good.</p>
  <p>This README was created at the beginning of the semester for directing the project. Specific instructions for using the project software are in <a href="MANUAL.md">the manual</a>.
</div>

## Vision
1. Implement an intermediate bytecode runtime with first-class support for classes and class instances into the Linux kernel at the lowest level possible
2. Implement as much of the kernel's responsibilities in bytecode format as possible
Completing the runtime is the project expectation, rebuilding the rest of the kernel interface is an optimistic target.

See [the proposal document](docs/project-proposal.md) for the project goals that were formally proposed at the beginning of the semester.

## Goals
The overarching goal is to create a whole new runtime and embed it at (nearly) the bottom of the operating system, rather than simply grafting the .NET Core runtime into the kernel sources or something like that.

First steps:
* (C) Locate where to insert the runtime into the Linux kernel. This is likely to be, in terms of the boot process, at least after:
  * Memory-related setup
  * Initial RAM-disk has been loaded
* (C) Build a reference-counting heap over the memory management part of the kernel to hold class instances.

The fun things:
* (C) Build the bytecode program loader. First priority is copying into memory. If we have more time we can have it validate programs.
* (C) Build the bytecode interpreter for running loaded programs.

The optimistic things:
* (C#) Implement as much of the rest of the kernel in C# as possible
* (C/C#) Make hooks into the native code in the C# portion of the kernel as necessary
* (C#) Create the a minimum viable kernel API to expose to bytecode programs

We may need to create a quick and dirty translator to turn .NET intermediate language into our bytecode. This could be more fun than handwriting bytecode programs.

## Workspace Details
[The manual](MANUAL.md) contains proper explaination of project usage overall and also usage for each subproject.

This repository contains the Linux kernel 5.5.2 and Busybox 1.31 source trees that are used to build a minimal bootable Linux system as described by the practical references below.

After cloning this repository, run `setup.sh` to do `make suchandsuchconfig` and whatever else happens under the covers, creating the `build` and `tree-snapshots` directories. `tree-snapshots` contains listings of the subdirectories of `build`, which can be compared with `treetool` if you want to see what configuration and building actually do.

Run `build.sh` to build the kernel, busybox, and initial RAM-disk (initramfs/initrd) in `build`. If you pass arguments to this script, they get passed to `make`. Use `-jN` to do N parts of the build concurrently, where N <= your CPU thread count.

Run `start-qemu` to launch the kernel and initramfs in the QEMU emulator.

## Practical References
This kernel workspace is based on [this Gist](https://gist.github.com/chrisdone/02e165a0004be33734ac2334f215380e) by [Chris Done](https://gist.github.com/chrisdone), since I have not done much like this before. I'll work further into the proper way to do kernel work as I get better aquainted with the kernel source. It walks through the kernel build, BusyBox build (really small version of many standard commands and /bin/sh), initial RAM-disk creation, and then launches all of it with the QEMU emulator.

I also read [The Linux Bootdisk HOWTO](http://www.tldp.org/HOWTO/Bootdisk-HOWTO/) while trying to understand how to make the kernel bootable (Didn't get all the way through).

Used [ILDASM](https://docs.microsoft.com/en-us/dotnet/framework/tools/ildasm-exe-il-disassembler) in high school to disassemble .NET binaries into their intermediate language. JetBrains probably has many more tools now for .NET than they had then. Pages [on the Common Intermediate Language](https://en.wikipedia.org/wiki/Common_Intermediate_Language) and [the list of its intructions](https://en.wikipedia.org/wiki/List_of_CIL_instructions).

We need to investigate how well-developed [.NET Core](https://docs.microsoft.com/en-us/dotnet/core/) is now for Linux, as an alternative to the [Mono framework](https://www.mono-project.com/). This will be needed once our kernel source includes C# code.

I'm not certain inspiration should also be drawn from the JVM, but there is also [a description of Java bytecode](https://en.wikipedia.org/wiki/Java_bytecode), [a list of instructions](https://en.wikipedia.org/wiki/Java_bytecode_instruction_listings), and [a description of the class file format](https://en.wikipedia.org/wiki/Java_class_file).

We will be interpreting, not worrying about optimization or just-in-time compilation.
