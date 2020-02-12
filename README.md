<div align="center">
  <h1>Object-oriented Kernel Hacking</h1>
  <p>Linux kernel workspace for term research in GSU's CSC 4320 (Operating Systems) course. Yet another attempt at marrying an intermediate runtime that has first-class support for object orientation with the operating system for great good.</p>
</div>

## Vision
1. Implement an intermediate bytecode runtime with first-class support for classes and class instances into the Linux kernel at the lowest level possible
2. Implement as much of the kernel's responsibilities in bytecode format as possible
Completing the runtime is the project expectation, rebuilding the rest of the kernel interface is an optimistic target.

## Goals
The overarching goal is to create a whole new runtime and embed it at (nearly) the bottom of the operating system, rather than simply grafting the .NET Core runtime into the kernel sources or something like that. We should:
* Locate where to insert the runtime into the Linux kernel. This is likely to be at least after all memory-related setup in the boot process.
* Build a reference-counting heap over the memory management part of the kernel to hold class instances.
* Build the bytecode program loader. First priority is copying into memory. If we have more time we can have it validate programs.
* Build the bytecode interpreter for running loaded programs.
* Create the a minimum viable kernel API to expose to bytecode programs.
* (Optionally) Create a quick and dirty translator to turn .NET intermediate language into our bytecode. This could be more fun than handwriting bytecode programs.

## Formal Research Proposal
<div align="center">
  <h3>Enabling extensibility in operating systems throughobject-orientation and reflection protocols</h3>
</div>
Object-orientation has proved itself to be a robust approach to modeling systems of softwarein a way that naturally allows programmers to add new capabilities to existing code. Thispotential for extensibility can also be maximized through deliberate software design decisions.Reflection and metaobject protocols, which provide a way for object-oriented systems to inspectand modify themselves at runtime, provide additional flexibility within individual programs andalso eases the process of creating multi-program systems (also known as system integration).
Research into object-oriented and similarly-extensible operating systems has been done in awide variety of contexts, though no system developed as a part of these projects has managed totake a dominant position in the current operating system landscape. Corporations that markettheir own object-oriented programming languages and frameworks have often pursueddevelopment of systems based on these frameworks. Microsoft’s Singularity operating system isone such project, which seeks to utilize the stability of their .NET framework and re-evaluatemany core design decisions in current systems [1][2]. There have also been plenty of academicprojects in this area, most of which attempt to establish the viability of systems that make use ofobject-orientation and reflective protocols [3][4]. At the University of Illinois atUrbana-Champaign, the Choices and μChoices projects sought to develop a well-structuredsystem that exposed an object-based application interface [5]. Other efforts, such as the PUREproject for embedded systems, apply object-oriented principles to more specific domains or typesof systems [6].
This project seeks primarily to harness object-orientation and reflective protocols in order tocreate an operating system that provides a flexible platform for system services and higher-levelprograms. It also hopes to encourage operating system research by modeling and implementing the system in a manner that is intuitive for programmers who are accustomed primarily with object-oriented programming practices. The proposed system will provide first-class support for object-orientation to user applications and to itself by implementing - at the lowest level possible within the kernel - the runtime for an intermediate instruction set that includes instructions forinteractions with objects. User applications and as much of the operating system as possible will be implemented using the intermediate instruction set. In order to minimize the cognitive load for individuals who are exploring the composition of the system for the first time, the systeminterface will be derived from that of the widely-used Unix family of operating systems. This will allow programmers who are already comfortable with Unix systems to familiarize themselves with the proposed system quickly.

[1] G. Hunt and J. Larus, “Singularity Technical Report 1: Singularity Design Motivation,” ​Microsoft Research​, MSR-TR-2004-105, November 2004. [Online]. Available: https://www.microsoft.com/en-us/research/publication/singularity-design-motivation/​. [Accessed: Jan.24, 2020].

[2] G. Hunt and J. Larus, “Singularity: Rethinking the Software Stack,” ​ACM SIGOPS Operating SystemsReview​, vol. 41, no. 2, Apr., pp. 37-49, 2007.

[3] R. Lea, Y. Yokote, and J.-I. Itoh, “Adaptive operating system design using reflection,” In Proc. 5th Workshop on Hot Topics in Operating Systems, pages 95-100, 1995.

[4] Z. Wu and R. Stroud, “Using Metaobject Protocols to Structure Operating Systems,” In Proc. International Workshop on Object Orientation in Operating Systems, ‘08, 1995.

[5] S. Tan, D. Raila, R. Campbell, “An Object-Oriented Nano-Kernel for Operating System Hardware Support,” In Proc. International Workshop on Object Orientation in Operating Systems, ‘08, 1995, pp.220-223.

[6] D. Beuche, A. Guerrouat, H. Papajewski, W. Schröder-Preikschat, O. Spinczyk, and U. Spinczyk. The PURE Family of Object-Oriented Operating Systems for Deeply Embedded Systems. In Proc. 2nd IEEE International Symposium on Object-Oriented Real-Time Distributed Computing (ISORC ’99), St. Malo, France, May 1999.

## Section Header Text
This kernel workspace is based on [this Gist](https://gist.github.com/chrisdone/02e165a0004be33734ac2334f215380e) by [Chris Done](https://gist.github.com/chrisdone), since I have not done much like this before. I'll work further into the proper way to do kernel work as I get better aquainted with the kernel source.
