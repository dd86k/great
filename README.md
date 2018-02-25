
[//]: # (❌/✔️)

# great

`great` is a simple executable/library analyzer. It's pretty incomplete as of right now.

## Support table

What's _great_so far?

| | Header | Relocations | Sections | Symbols |
|---|---|---|---|---|
| MZ | ✔️ | ✔️ | N/A | N/A |
| NE | ❌ | ❌ | ❌ | ❌ |
| LE | ❌ | ❌ | ❌ | ❌ |
| LX | ❌ | ❌ | ❌ | ❌ |
| PE32 | ✔️ | ❌ | ✔️ | ❌ |
| PE32+ | ✔️ | ❌ | ✔️ | ❌ |
| ELF32 | ❌ | ❌ | ❌ | ❌ |
| ELF64 | ❌ | ❌ | ❌ | ❌ |

Other planned stuff:
- `-s` switch for Symbols
- a complete manpage

# Compiling

You __must__ pack structs by 1-byte. `/Zp` on cl and `-fpack-struct=1` on gcc platforms.