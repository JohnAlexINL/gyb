#define GPLV3Link "https://www.gnu.org/licenses/gpl-3.0.en.html"
#define cmd_elf "elf "
#define cmd_link "link"
#define cmd_make "make"
#define version "gyb linker version 0.0.0\n"
#define copyright "Copyright (c) 2025 Modula <https://Modula.dev>"
const char badhelp[] = "gyb given invalid input.\nrunning \e[0;32mgyb help\e[0m prints the help dialog\n";
const char badargs[] = "gyb %s given the wrong number of arguments\n";
const char badplatform[] = "gyb %s given invalid target platform\n";
const char notice[] = version copyright "\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, only under version 3 of this license.\n\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n";

const char help[] = version "\
\e[0;34mSYNOPSIS\e[0m\n\
\e[0;32m\
    gyb " cmd_link " <dest> [sources] \n\
    gyb " cmd_elf " <dest> <bytecode>\n\
    gyb " cmd_make " <platform> <dest> <bytecode>\n\
    gyb license \n\
\e[0;34mDESCRIPTION\e[0m\n\
\e[0;32m\
    converts garter relocatable objects into bytecode\n\
    or ELF objects for cross-compilation\n\
    \e[0;35m" cmd_link  "\e[0m\tcombine multiple objects into one bytecode object\n\
    \e[0;35m" cmd_elf   "\e[0m\tconvert a GYB object into an ELF object\n\
    \e[0;35m" cmd_make  "\e[0m\tproduce native platform-specific executables\n\
\e[0;34mLICENSE\e[0m\n\
\e[0;32m\
    GPLv3, \
\e[0;35m" GPLV3Link "\n\
\e[0m\
    Put briefly; you may redistribute and/or modify this software\n\
    under the terms of the GPLv3 License as published by \n\
    the Free Software Foundation if and only if \n\
    the license and our copyright notice is included:\n\
    \e[0;31m\"" copyright "\"\e[0m\n\0";
void printLicense() { printf("%s%s\n", notice, license); }