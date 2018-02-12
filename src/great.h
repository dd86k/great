#ifdef _WIN32
#include <Windows.h>
HANDLE f;
#else
FILE *f;
#endif

void scan(void);
