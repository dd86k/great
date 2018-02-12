#include <stdio.h>
#include "utils.h"
#include "mz/mz.h"

void scan() {
	unsigned int s;
	if (_ddread(&s, 4)) {
		puts("ERROR: Could not read file");
		return;
	}

	switch (s) {
	
	default:
		switch (s & 0xFFFF) {
		case 0x5A4D:
			scan_mz();
			break;
		default:
			puts("ERROR: Unknown executable/library format");
		}
	}
}