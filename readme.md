# GYB Linker

`gyb` is the linker for the `m` compiler frontend
and the `garter` compiler toolchain.
It implements a lightweight, custom object format designed to be super easy to re-implement while also trivial to translate into real object and executable formats.

## Methodology

To make ELF `.o` or executables from a `.gyb` bytecode object,
we'll take template object and inject our code
into the appropriate sections of it,
translate and inject our symbol table into it

## Purpose

The `m` compiler toolchain seeks to separate out
as many responsibilites from each process as possible.

The purpose for designing our own object format is simple;
ELF is complicated and difficult to work with. Documentation for the Executable and Linkable Format is extensive, but really unhelpful, and from the perspective of trying to actually work with it _sucks_.

Using a simplified object format means new backends are easy to write, and frontends are easy to plug in, and that's exactly what this toolchain is all about.

## Linker i/o

This linker takes in
GYB `*.gyo` and `*.gyb` Object files.

It can:
- link multiple GYB objects together into one object
- convert a static `*.gyb` object into an ELF object
- convert a static `*.gyb` object into a static executable

Planned executable targets:
- Windows Portable Executable `x86-64`
- MacOS/RayvnOS MachO `x86-64`/`AArch64`
- Linux/BSD ELF `x86-64`/`AArch64`/`RISC-V`

# Implementation

## How does Relocation Work?

- Load and validate each object is relocatable
- Load all of the symbol tables and update their offsets
- Check to make sure there are no hanging `external` symbols;
    - If we have `external` symbols which haven't been resolved,
    we should give a linker warning to the console and
    set a flag so that the output object is marked as being non-static (`GYO\0`)
- Squash the symbol table back down into a `char *`
- Output a new bytecode file

## How do the Bytecode Symbols Work?

In the executable sections of our `*.gyb` objects,
instructions will contain references to our symbols.
`constant` symbols are just a numerical value
that is not updated by the linking stage,
whereas all the other types are modified during linkage.

If our symbol table has an entry like
```
@0xFF FOOBAR w 0x000C
```
and our executable section contains the instruction
```
store ar @0xFF
```
then the actual address for the `store` instruction
will be the address of our writable section offset by `0x000C`.

## How do we convert a GYB object to an ELF object?

We're going to output an ELF object based on a prefab
which has one executable section,
one read-writable section, one read-only section,
a `symtab` and a `strtab`.

We have to translate our symbol table
from the `GYB` style `symbol_t` structs
into the `symtab` and `strtab` structures,
which is fairly trivial;
`strtab` will contain the names of each symbol
with a null-terminator, and
`symtab` will contain the translated
section and offset data.
We can safely assume any symbols in our object files
were intended to be global.

When building the ELF object, any symbols that were
left as unresolved external symbols are similarly
marked as such in the output ELF object.

## How to we build binaries from a GYB object?

This depends on the type of executable we're building,
but for most types, such as Portable Executables or ELF binaries,
it's pretty much the same.

We can output based on a prefab for that executable type.
For ELF, for example, we can check if we need to allocate
room for the `.rodata` (readonly), `.data` (writable),
and `.text` (executable) sections and how much.
For the `.rodata` and `.data` sections, we just
clone the data in as-is from our reference object,
and for `.text` section, we need to iterate through it,
resolving the instructions and symbols to
architecture-specific instructions and the correct
offsets or literal values.

Garter bytecode objects should always be, essentially,
statically compiled objects, so if we encounter any
`external` symbols in the object, we should error out.

# GYB Object Format

The header starts with one of two magic values:
```c
const char *static_object =      "GYB\0";
const char *relocatable_object = "GYO\0";
```
Then we have the following data:
```c
uint32_t entry;
uint32_t readonly_offset;
uint32_t readonly_size;
uint32_t writable_offset;
uint32_t writable_size;
uint32_t executable_offset;
uint32_t executable_size;
uint32_t symboltable_offset;
uint32_t symboltable_size;
```
These sizes are expressed in bytes,
and the offsets are bytes from the end of this header.

The readable and writable sections are literal data
and are unmodified by the linker during relocation.
The executable section contains platform-independent
bytecode using symbols in the instructions
which will be modified during relocation and linking.

The symbol table contains a block of the raw data
of `symbol_t` entries, which look like
```c
char name[symbol_namelen];
uint32_t offset;
unsigned char section;
unsigned char defined;
```

If `symboltable_size` is not
an exact multiple of `sizeof(symbol_t)`,
then we know that the object is erroneous and can error out.

## The Bytecode

The bytecode is influenced heavily by `x86`
but uses a small subset of registers and
symbol-references in place of addresses and literal values.
GYB bytecode includes integer and floating arithmetic instructions,
instructions for casting and converting between the two types,
various instructions for moving data around,
and instructions for branching and interacting with the system,
including both a platform-agnostic `syscall`,
and a manual platform-dependent `interrupt`.

For more information on writing garter bytecode,
see the `garter` specification doc.