#ifdef _WIN32
#include <Windows.h>
#include <WinBase.h>
#include <tchar.h>
#include <wchar.h>
#elif __linux__
#include <glob.h>
#else // POSIX

#endif

#include <stdio.h>
#include <stdlib.h>
#include "great.h"
#include "utils.h"
#include "settings.h"

void help() {
	puts(
		"Print exec/lib information\n"
		"  Usage: great FILE [OPTIONS]\n"
		"         great OPTIONS\n"
		"\nOPTIONS\n"
		"  -s    Show symbols\n"
		"\n  -h    Show help screen and exit\n"
		"  -v    Show version screen and exit"
	);
}

void version() {
	puts(
		"great v0.0.0-0\n"
		"MIT License: Copyright (c) 2018 dd86k\n"
		"Project page: <https://github.com/dd86k/great>\n"
		"\nSupported binaries:\n"
		"- Executables:\n"
		"  - PE32\n"
		"  - PE32+\n"
	);
}

char _args = 1;

#ifdef _WIN32
#define MAIN int wmain(int argc, wchar_t **argv)
void sa(wchar_t *a) {
#else
#define MAIN int main(int argc, char **argv)
void sa(char *a) {
#endif
	while (*++a != 0) {
		switch (*a) {
		case 'h': help(); exit(0); return;
		case 'v': version(); exit(0); return;
		case 's': ShowSymbols = 1; break;
		case '-': _args = 0; break;
		}
	}
}

// CLI Options

#ifdef _WIN32
#define O_HELP L"help"
#define O_VERSION L"version"
#else
#define O_HELP "help"
#define O_VERSION "version"
#endif

#ifdef _WIN32
void sb(wchar_t *a) {
	if (_strcmpw_l(a, O_HELP, sizeof(O_HELP)/2) == 0) {
		help();
		exit(0);
	}
	if (_strcmpw_l(a, O_VERSION, sizeof(O_VERSION)/2) == 0) {
		version();
		exit(0);
	}
#else
void sb(char *a) {
	if (_strcmp_l(a, O_HELP, sizeof(O_HELP)) == 0) {
		help();
		exit(0);
	}
	if (_strcmp_l(a, O_VERSION, sizeof(O_VERSION)) == 0) {
		version();
		exit(0);
	}
#endif
}

MAIN {
	if (argc <= 1) {
		help();
		return 0;
	}
	while (--argc >= 1) {
		if (_args) {
			if (argv[argc][1] == '-') { // long arguments
				sb(argv[argc]+2);
				continue;
			} else if (argv[argc][0] == '-') { // short arguments
				sa(argv[argc]);
				continue;
			}
		}
#ifdef _WIN32
		unsigned int a = GetFileAttributesW(argv[argc]);
		if ((a & 0x10) == 0) { // NOT FILE_ATTRIBUTE_DIRECTORY
			f = CreateFileW(argv[argc],
				GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (!f) { //TODO: GetLastError (Windows)
				puts("There was an issue opening the file.");
				return 2;
			}
			scan();
		} else {
			wprintf(L"%s: Entry does not exist or a directory\n", argv[argc]);
			return 1;
		}
#elif __linux__
		glob_t globbuf;
		globbuf.gl_offs = 1;
		glob(argv[argc], GLOB_DOOFFS, NULL, &globbuf);
		if (globbuf.gl_pathc > 0) {
			f = fopen(argv[argc], "rb"); // maybe use _s?
			if (!f) {
				puts("There was an issue opening the file.");
				return 2;
			}
			scan();
		}
		globfree(&globbuf);
#else // POSIX

#endif
	} // while

	return 0;
}