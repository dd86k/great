# great

`great` is a simple executable/library analyzer. It's pretty incomplete as of right now.

So far, the list of supported binaries are:
- MZ
- PE32/PE32+

Planned stuff:
- PE32
  - Symbols
  - other stuff
- ELF32/ELF64
- NE
- LE/LX

Other planned stuff:
- `-s` switch for Symbols
- a complete manpage

# Compiling

You __must__ pack structs by 1-byte. `/Zp` on cl and `-fpack-struct=1` on gcc platforms.

# Example

Example output for `great.exe`:
```
PE32+ Windows Console Executable for x86-64 machines, LARGE_ADDRESS_AWARE
DLL Characteristics: , DYNAMIC_BASE, NX_COMPAT, TERMINAL_SERVER_AWARE
Timestamp               : 5A80DE67h
Number of sections      : 5
Number of symbols       : 0
Symbol Table Pointer    : 0h
Optional Header Size    : 240
Linker Version          : 14.12
Size of Code            : 2C00h
Initialized Data Size   : 2C00h
Uninitialized Data Size : 0h
Entry Point Address     : 2F1Ch
Base of Code            : 1000h
Base of Data            : 7FFBh
Base of Image           : 140000000h
Section Alignment       : 4096
File Alignment          : 512
OS Version              : 6.0
Image Version           : 0.0
Subsystem Version       : 6.0
Win32 Version           : 0h
Size of Image           : 9000h
Size of Headers         : 400h
Checksum                : 0h
Stack Reserve Size      : 100000h
Stack Commit Size       : 1000h
Heap Reserve Size       : 100000h
Heap Commit Size        : 1000h
Loader Flags (Obsolete) : 0h
Number of RVA and Sizes : 16

Directories

Export Table          : 00000000  0
Import Table          : 00005660  160
Resource Table        : 00000000  0
Exception Table       : 00007000  528
Certificate Table     : 00000000  0
Base Relocation Table : 00008000  44
Debug Directory       : 00005140  28
Architecture Data     : 00000000  0
Global Pointer        : 00000000  0
TLS Table             : 00000000  0
Load Configuration    : 00005160  256
Bound Import          : 00000000  0
Import Address        : 00004000  456
Delay Import          : 00000000  0
CLR Header            : 00000000  0
Reserved              : 00000000  0

Sections

1 .text
  Virtual Address : 00001000  10904
  Raw Data        : 00000400  11264
  Relocations     : 00000000  0
  Line Numbers    : 00000000  0
  <, CNT_CODE, MEM_EXECUTE, MEM_READ>

2 .rdata
  Virtual Address : 00004000  7518
  Raw Data        : 00003000  7680
  Relocations     : 00000000  0
  Line Numbers    : 00000000  0
  <, INITIALIZED_DATA, MEM_READ>

3 .data
  Virtual Address : 00006000  1656
  Raw Data        : 00004E00  512
  Relocations     : 00000000  0
  Line Numbers    : 00000000  0
  <, INITIALIZED_DATA, MEM_READ, MEM_WRITE>

4 .pdata
  Virtual Address : 00007000  528
  Raw Data        : 00005000  1024
  Relocations     : 00000000  0
  Line Numbers    : 00000000  0
  <, INITIALIZED_DATA, MEM_READ>

5 .reloc
  Virtual Address : 00008000  44
  Raw Data        : 00005400  512
  Relocations     : 00000000  0
  Line Numbers    : 00000000  0
  <, INITIALIZED_DATA, MEM_DISCARDABLE, MEM_READ>

No symbols

```