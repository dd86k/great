#ifdef _WIN32
#include <Windows.h>
#include <WinBase.h>
#include <tchar.h>
#include <wchar.h>
#elif __linux__
#include <glob.h>
#else // POSIX

#endif

#define VERSION "0.0.0" // MAJOR.MINOR.REVISION

#include <stdio.h>
#include <stdlib.h>
#include "great.h"
#include "utils.h"
#include "settings.h"

void help() {
	puts(
		"Print exec/lib information\n"
		"  Usage: great FILE [OPTIONS]\n"
		"         great OPTIONS\n\n"
		"  -H    Show detailed header info\n"
		"  -R    Show relocations\n"
		"  -S    Show sections\n"
		"  -Y    Show symbols\n"
		"  -D    (PE32) Show directories\n"
		"  -A    Show all\n"
		"  -h, --help      Show help screen and exit\n"
		"  -v, --version   Show version screen and exit"
	);
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
		"  header, relocations\n"
		"PE32\n"
		"  header, sections\n"
		"PE32+\n"
		"  header, sections\n"
		"ELF32\n"
		"  header\n"
		"ELF64\n"
		"  header"
	);
}

char _args = 1;

// CLI Options

#ifdef _WIN32
#define MAIN int wmain(int argc, wchar_t **argv)
#define vchar wchar_t
#define O_HELP L"help"
#define O_VERSION L"version"
void arglong(wchar_t *a) {
	if (wcscmp(a, O_HELP) == 0)
		help();
	if (wcscmp(a, O_VERSION) == 0)
		version();
#else // POSIX
#define MAIN int main(int argc, char **argv)
#define vchar char
#define O_HELP "help"
#define O_VERSION "version"
void arglong(char *a) {
	if (strcmp(a, O_HELP) == 0)
		help();
	if (strcmp(a, O_VERSION) == 0)
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
				arglong(argv[argc] + 2); continue;
			} else if (argv[argc][0] == '-') { // short arguments
				vchar* a = argv[argc];
				while (*++a != 0) {
					switch (*a) {
					case 'h': help(); return 0;
					case 'v': version(); return 0;
					case 'S': ++setting_sections; break;
					case 'Y': ++setting_symbols; break;
					case 'D': ++setting_directories; break;
					case 'H': ++setting_header; break;
					case 'A':
						memset(&setting_header, 1, 5);
						break;
					case '-': --_args; break;
					default:
						printf("Unknown parameter: %c", *a);
						return 1;
					}
				}
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