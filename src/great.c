#include <stdio.h>
#include "utils.h"
#include "mz/mz.h"
#include "elf/elf.h"

#define MAGIC_ELF 0x464C457F // "\x7FELF"
#define MAGIC_MZ 0x5A4D // "MZ"

void scan() {
	unsigned int s;
	if (_ddread(&s, 4)) {
		puts("ERROR: Could not read file");
		return;
	}

	switch (s) {
	case MAGIC_ELF:
		scan_elf();
		return;
	default:
		switch ((unsigned short)s) {
		case MAGIC_MZ:
			scan_mz();
			return;
		default:
			puts("ERROR: Unknown executable/library format");
		}
	}
}