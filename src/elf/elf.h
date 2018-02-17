// http://www.sco.com/developers/gabi/latest/contents.html

// -4 because we already scanned for magic (4 bytes) beforehand

#define EI_NIDENT  12 // 16-4

#define EI_CLASS   0 // 4-4
#define EI_DATA    1 // 5-4
#define EI_OSABI   3 // 7-4

#define EV_NONE    0
#define EV_CURRENT 1

#define ET_NONE    0      // No file type
#define ET_REL     1      // Relocatable file
#define ET_EXEC    2      // Executable file
#define ET_DYN     3      // Shared object file
#define ET_CORE    4      // Core file
#define ET_LOPROC  0xff00 // Processor-specific
#define ET_HIPROC  0xffff // Processor-specific

#define EM_NONE        0  // No machine
#define EM_M32         1  // AT&T WE 32100
#define EM_SPARC       2  // SPARC
#define EM_386         3  // Intel Architecture
#define EM_68K         4  // Motorola 68000
#define EM_88K         5  // Motorola 88000
#define EM_860         7  // Intel 80860
#define EM_MIPS        8  // MIPS RS3000
#define EM_MIPS_RS4_BE 10 // MIPS RS4000 Big-Endian
#define EM_POWERPC 0x14 // PowerPC
#define EM_ARM     0x28 // ARM
#define EM_SUPERH  0xA2 // SuperH
#define EM_IA64    0x32 // Intel IA64
#define EM_AMD64   0x3E // x86-64
#define EM_AARCH64 0xB7 // 64-bit ARM

struct Elf32_Ehdr {
	//unsigned char e_ident[EI_NIDENT];
	unsigned short e_type;
	unsigned short e_machine;
	unsigned int e_version;
	unsigned int e_entry;
	unsigned int e_phoff;
	unsigned int e_shoff;
	unsigned int e_flags;
	unsigned short e_ehsize;
	unsigned short e_phentsize;
	unsigned short e_phnum;
	unsigned short e_shentsize;
	unsigned short e_shnum;
	unsigned short e_shstrndx;
};

struct Elf64_Ehdr {
	//unsigned char e_ident[EI_NIDENT];
	unsigned short e_type;
	unsigned short e_machine;
	unsigned int e_version;
	unsigned long long e_entry;
	unsigned long long e_phoff;
	unsigned long long e_shoff;
	unsigned int e_flags;
	unsigned short e_ehsize;
	unsigned short e_phentsize;
	unsigned short e_phnum;
	unsigned short e_shentsize;
	unsigned short e_shnum;
	unsigned short e_shstrndx;
};

struct Elf32_Shdr { // Section
	unsigned int sh_name;
	unsigned int sh_type;
	unsigned int sh_flags;
	unsigned int sh_addr;
	unsigned int sh_offset;
	unsigned int sh_size;
	unsigned int sh_link;
	unsigned int sh_info;
	unsigned int sh_addralign;
	unsigned int sh_entsize;
};

struct Elf64_Shdr { // Section
	unsigned int sh_name;
	unsigned int sh_type;
	unsigned long long sh_flags;
	unsigned long long sh_addr;
	unsigned long long sh_offset;
	unsigned long long sh_size;
	unsigned int sh_link;
	unsigned int sh_info;
	unsigned long long sh_addralign;
	unsigned long long sh_entsize;
};

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MASKPROC 0xf0000000

void scan_elf(void);