#ifdef _WIN32
#include <Windows.h>
HANDLE f;
#else
#include <stdio.h>
FILE *f;
#endif

void scan(void);
