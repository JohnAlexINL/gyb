# gyb

`gyb` is the linker for the `m` compiler frontend
and the `garter` compiler toolchain.

## Object Format

This linker will implement its own object format

## Outputs

This linker can either:
- Take multiple `.gyo` files and merge them into one `.gyb`
- Take a `.gyb` file and translate it into an ELF `.o`
- Take a `.gyb` file and translate it into a static executable ELF

## Methodology

To make an ELF `.o` from a `.gyb` bytecode object,
we'll take template object and inject our code
into the appropriate sections of it,
translate and inject our symbol table into it,
and then output it back into a file.

## Purpose

The `m` compiler toolchain seeks to separate out
as many responsibilites from each process as possible.
Splitting the emitter from the linker allows the linker
to not worry about *how* the objects are prepared for linking,
and allows the emitter to only worry about the
single and very simply object format

# Implementation

## How does Relocation Work?

- Load and validate each object is relocatable
- Load all of the symbol tables and update their offsets
- Check to make sure there are no hanging `external` symbols;
    - If we have `external` symbols which haven't been resolved,
    we should give a linker warning to the console and
    set a flag so that the output object is
    still marked as needing relocation (i.e., magic `GYO\0`, not `GYB\0`)
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