#include <stdio.h>
#include "../utils.h"
#include "elf.h"

/// Scan an ELF image
void scan_elf() {
	unsigned char e_ident[EI_NIDENT]; // Exclude magic
	_ddread(e_ident, sizeof(EI_NIDENT));
	unsigned char class = e_ident[EI_CLASS];

	struct Elf64_Ehdr h;
	switch (class) {
	case 1:
		struct Elf32_Ehdr e32;
		_ddread(&e32, sizeof(struct Elf32_Ehdr));
		h.e_type = e32.e_type;
		h.e_machine = e32.e_machine;
		h.e_version = e32.e_version;
		h.e_entry = e32.e_entry;
		h.e_phoff = e32.e_phoff;
		h.e_shoff = e32.e_shoff;
		h.e_flags = e32.e_flags;
		h.e_ehsize = e32.e_ehsize;
		h.e_phentsize = e32.e_phentsize;
		h.e_phnum = e32.e_phnum;
		h.e_shentsize = e32.e_shentsize;
		h.e_shnum = e32.e_shnum;
		h.e_shstrndx = e32.e_shstrndx;
		break;
	case 2:
		_ddread(&h, sizeof(struct Elf64_Ehdr));
		break;
	default:
		puts("ERROR: EI_CLASS?");
		return;
	}

	printf("ELF");
	switch (class) {
	case 1: printf("32 "); break;
	case 2: printf("64 "); break;
	default: printf("? "); break;
	}
	switch (e_ident[EI_DATA]) {
	case 1: printf("LE "); break;
	case 2: printf("BE "); break;
	default: printf("? "); break;
	}
	switch (e_ident[EI_OSABI]) {
	case 0x00: printf("System V"); break;
	case 0x01: printf("HP-UX"); break;
	case 0x02: printf("NetBSD"); break;
	case 0x03: printf("Linux"); break;
	case 0x06: printf("Solaris"); break; 
	case 0x07: printf("AIX"); break;
	case 0x08: printf("IRIX"); break;
	case 0x09: printf("FreeBSD"); break;
	case 0x0C: printf("OpenBSD"); break;
	case 0x0D: printf("OpenVMS"); break;
	case 0x0E: printf("NonStop Kernel"); break;
	case 0x0F: printf("AROS"); break;
	case 0x10: printf("Fenix OS"); break;
	case 0x11: printf("CloudABI"); break;
	case 0x53: printf("Sortix"); break;
	default:   printf("Unknown DECL"); break;
	}
	switch (h.e_type) {
	case ET_NONE:   printf(" (No filetype)"); break;
	case ET_REL:    printf(" Relocatable"); break;
	case ET_EXEC:   printf(" Executable"); break;
	case ET_DYN:    printf(" Shared object"); break;
	case ET_CORE:   printf(" Core"); break;
	case ET_LOPROC: printf(" Professor-specific (LO)"); break;
	case ET_HIPROC: printf(" Professor-specific (HI)"); break;
	default:        printf(" ?"); break;
	}
	printf(" for ");
	switch (h.e_machine) {
	case EM_NONE:    printf("no"); break;
	case EM_M32:     printf("AT&T WE 32100 (M32)"); break;
	case EM_SPARC:   printf("SPARC"); break;
	case EM_860:     printf("Intel 80860"); break;
	case EM_386:     printf("x86"); break;
	case EM_IA64:    printf("IA64"); break;
	case EM_AMD64:   printf("x86-64"); break;
	case EM_68K:     printf("Motorola 68000"); break;
	case EM_88K:     printf("Motorola 88000"); break;
	case EM_MIPS:    printf("MIPS RS3000"); break;
	case EM_POWERPC: printf("PowerPC"); break;
	case EM_ARM:     printf("ARM"); break;
	case EM_SUPERH:  printf("SuperH"); break;
	case EM_AARCH64: printf("ARM (64-bit)"); break;
	default:         printf("unknown system"); break;
	}
	puts(" machines");

	printf(
		"type     : %Xh\n"
		"machine  : %Xh\n"
		"version  : %Xh\n"
		"entry    : %llXh\n"
		"phoff    : %llXh\n"
		"shoff    : %llXh\n"
		"flags    : %Xh\n"
		"ehsize   : %Xh\n"
		"phentsize: %Xh\n"
		"phnum    : %d\n"
		"shentsize: %Xh\n"
		"shnum    : %d\n"
		"shstrndx : %Xh\n",
		h.e_type, h.e_machine, h.e_version,
		h.e_entry, h.e_phoff, h.e_shoff,
		h.e_flags, h.e_ehsize, h.e_phentsize,
		h.e_phnum, h.e_shentsize, h.e_shnum,
		h.e_shstrndx
	);

	if (h.e_shnum) {
		_ddseek(h.e_shoff, SEEK_SET);

		int i = 0;
		struct Elf64_Shdr s;
		do {
			switch (class) {
			case 1:
				struct Elf32_Shdr s32;
				_ddread(&s32, sizeof(struct Elf32_Shdr));
				s.sh_name = s32.sh_name;
				s.sh_type = s32.sh_type;
				s.sh_flags = s32.sh_flags;
				s.sh_addr = s32.sh_addr;
				s.sh_offset = s32.sh_offset;
				s.sh_size = s32.sh_size;
				s.sh_link = s32.sh_link;
				s.sh_info = s32.sh_info;
				s.sh_addralign = s32.sh_addralign;
				s.sh_entsize = s32.sh_entsize;
				break;
			case 2:
				_ddread(&s, sizeof(struct Elf64_Shdr));
				break;
			}
			printf(
				"  \n%d. \n"
				"  <",
				++i
			);
			switch (s.sh_type) {
			case SHT_PROGBITS: printf("SHT_PROGBITS"); break;
			case SHT_SYMTAB: printf("SHT_SYMTAB"); break;
			case SHT_STRTAB: printf("SHT_STRTAB"); break;
			case SHT_RELA: printf("SHT_RELA"); break;
			case SHT_HASH: printf("SHT_HASH"); break;
			case SHT_DYNAMIC: printf("SHT_DYNAMIC"); break;
			case SHT_NOTE: printf("SHT_NOTE"); break;
			case SHT_NOBITS: printf("SHT_NOBITS"); break;
			case SHT_REL: printf("SHT_REL"); break;
			case SHT_SHLIB: printf("SHT_SHLIB"); break;
			case SHT_DYNSYM: printf("SHT_DYNSYM"); break;
			case SHT_LOPROC: printf("SHT_LOPROC"); break;
			case SHT_HIPROC: printf("SHT_HIPROC"); break;
			case SHT_LOUSER: printf("SHT_LOUSER"); break;
			case SHT_HIUSER: printf("SHT_HIUSER"); break;
			default: printf("SHT_NULL"); break;
			}
			if (s.sh_flags & SHF_WRITE)
				printf(", SHF_WRITE");
			if (s.sh_flags & SHF_ALLOC)
				printf(", SHF_ALLOC");
			if (s.sh_flags & SHF_EXECINSTR)
				printf(", SHF_EXECINSTR");
			if (s.sh_flags & SHF_MASKPROC)
				printf(", SHF_MASKPROC");
			printf(">");
		} while (--h.e_shnum);
	}
}