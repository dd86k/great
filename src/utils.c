#include "utils.h" // Includes ff.h, Windows.h
#include <stdio.h>
#include <string.h>

/**
 * Reads from the already opened f handle defined in ff.h.
 * buffer: Void pointer to buffer
 * size: size of buffer
 * Returns: Number of bytes read
 */
int _ddread(void *buffer, size_t size) {
#ifdef _WIN32
	unsigned long read;
	return ReadFile(f, buffer, size, &read, NULL) == 0;
#else // POSIX
	return fread(buffer, size, 1, f) <= 0;
#endif
}

/**
 * Fast seek the already opened file handle.
 * position: New position (<32-bit limit)
 * mode: File mode
 * Returns: 
 */
int _ddseek(long int position, int mode) {
#ifdef _WIN32
	return SetFilePointer(f, position, NULL, mode) == 0xFFFFFFFF;
#else
	return fseek(fp, position, mode);
#endif
}

#define _ddrewind(); _ddseek(0, SEEK_SET);

/**
 * Simple string compare with a defined length
 * s1: String 1
 * s2: String 2, usually a constant
 * l : Maximum length before giving up
 * Returns 0 if same, otherwise 1
 */
int _strcmp_l(char *s1, char *s2, size_t l) {
	while (--l) {
		if (*s1 != *s2) return 1;
		++s1; ++s2;
	}
	return 0;
}
#ifdef _WIN32
/**
 * Simple wide string compare with a defined length,
 * usually only used within CLI to compare arguments
 * s1: String 1
 * s2: String 2, usually a constant
 * l : Maximum length before giving up (DIVIDE BY 2 OR SUFFER)
 * Returns 0 if same, otherwise 1
 */
int _strcmpw_l(wchar_t *s1, wchar_t *s2, size_t l) {
	while (--l) {
		if (*s1 != *s2) return 1;
		++s1; ++s2;
	}
	return 0;
}
#endif

// slice and copy string
//void strslice(char *buffer, int length)

/**
 * Slice an ISO9660-like string (space-padded)
 * buffer: Entry buffer
 * target: Target string
 * max   : Slice end (excluded)
 */
void isoslice(char *buffer, char *target, size_t max) {
	if (buffer[0] == ' ') return;
	while (buffer[--max] == ' ' && max > 0);
	if (max) {
		memcpy(target, buffer, ++max);
	}
}

/**
 * Format a decimal binary number and add its appropriate suffix.
 * e.g. 24554 will result 23K
 * l: Value
 */
void _printfd(unsigned long long l) {
	float f = l;
	// Lazy code, sorry
	if (f >= GB) {
		printf("%.2fG", f / GB);
	} else if (f >= MB) {
		printf("%.2fM", f / MB);
	} else if (f >= KB) {
		printf("%.2fK", f / KB);
	} else
		printf("%lldB", l);
}

unsigned short bswap16(unsigned short s) {
	return (s & 0xff00) >> 8 | (s & 0xff) << 8;
}

unsigned int bswap32(unsigned int s) {
	return
		(s & 0x000000ff) << 24 |
		(s & 0x0000ff00) << 8 |
		(s & 0x00ff0000) >> 8 |
		(s & 0xff000000) >> 24;
}

unsigned long long bswap64(unsigned long long s) {
	unsigned long int* pi = (unsigned long int*)&s;
	unsigned long int a = // Hopefully it uses bswap (or XOR for clang?)
		(pi[0] & 0x000000ff) << 24 |
		(pi[0] & 0x0000ff00) << 8 |
		(pi[0] & 0x00ff0000) >> 8 |
		(pi[0] & 0xff000000) >> 24;
	pi[0] =
		(pi[1] & 0x000000ff) << 24 |
		(pi[1] & 0x0000ff00) << 8 |
		(pi[1] & 0x00ff0000) >> 8 |
		(pi[1] & 0xff000000) >> 24;
	pi[1] = a;
	return s;
}

void print_array(unsigned char *a, size_t s) {
	while (--s) {
		printf(" %02X", *a); ++a;
	}
	puts("");
}