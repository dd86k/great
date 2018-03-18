#ifdef _WIN32
#include <Windows.h>
#include <WinBase.h>
#include <tchar.h>
#include <wchar.h>
#elif __linux__
#include <glob.h>
#else // POSIX

#endif

#define VERSION "0.0.0"

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
		//"  -s    Show symbols\n"
		"\n  -h    Show help screen and exit\n"
		"  -v    Show version screen and exit"
	);
	exit(0);
}

void version() {
	puts(
		"great v" VERSION
#ifdef TIMESTAMP
		" (" TIMESTAMP ")"
#endif
		"\n"
		"MIT License: Copyright (c) 2018 dd86k\n"
		"Project page: <https://github.com/dd86k/great>\n"
		"\nSupport\n"
		"MZ\n"
		"  header, relocations"
		"PE32\n"
		"  headers, sections\n"
		"PE32+\n"
		"  headers, sections\n"
	);
	exit(0);
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
		case 'h': help(); return;
		case 'v': version(); return;
		//case 's': s_symbols = 1; break;
		case '-': _args = 0; break;
		}
	}
}

// CLI Options

#ifdef _WIN32
#define O_HELP L"help"
#define O_VERSION L"version"
void sb(wchar_t *a) {
	if (_strcmpw_l(a, O_HELP, sizeof(O_HELP)/2) == 0)
		help();
	if (_strcmpw_l(a, O_VERSION, sizeof(O_VERSION)/2) == 0)
		version();
#else
#define O_HELP "help"
#define O_VERSION "version"
void sb(char *a) {
	if (_strcmp_l(a, O_HELP, sizeof(O_HELP)) == 0)
		help();
	if (_strcmp_l(a, O_VERSION, sizeof(O_VERSION)) == 0)
		version();
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
				sb(argv[argc]+2); continue;
			} else if (argv[argc][0] == '-') { // short arguments
				sa(argv[argc]); continue;
			}
		}
#ifdef _WIN32
		unsigned int a = GetFileAttributesW(argv[argc]);
		if ((a & 0x10) == 0) { // NOT FILE_ATTRIBUTE_DIRECTORY
			f = CreateFileW(argv[argc],
				GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (!f) { //TODO: GetLastError (Windows)
				fprintf(stderr, "E: Cannot open file\n");
				return 2;
			}
			scan();
		} else {
			_wprintf_p(L"E: Invalid entry: %s\n", argv[argc]);
			return 1;
		}
#else // POSIX
#error Missing CLI (POSIX)
#endif
	} // while

	return 0;
}