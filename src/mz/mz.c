#include <stdio.h>
#include "../utils.h"
#include "mz.h"
#include "pe.h"
//#include "ne.h"
//#include "le.h"

void scan_mz() {
	if (_ddseek(0x3c, SEEK_SET)) goto _MZ;

	unsigned long int p;
	_ddread(&p, 4);

    if (p) {
        unsigned short sig;
        if (_ddseek(p, SEEK_SET)) goto _MZ;
        _ddread(&sig, 2);
        switch (sig) {
        case 0x4550: // "PE"
            scan_pe();
            return;
        /*case 0x454E: // "NE"
            scan_ne();
            return;
        case 0x454C: // "LE"
        case 0x584C: // "LX"
            scan_le(sig == 0x454C);
            return;*/
        }
    }

_MZ:
    struct mz_hdr h;
    _ddseek(0, SEEK_SET);
    _ddread(&h, sizeof(h));

    printf("MZ executable for MS-DOS");
    if (h.e_ovno)
        printf(" (Overlay: %d)", h.e_ovno);

    puts("");

    printf(
        "e_cblp    : %Xh\n"
        "e_cp      : %Xh\n"
        "e_crlc    : %Xh\n"
        "e_cparh   : %Xh\n"
        "e_minalloc: %Xh\n"
        "e_maxalloc: %Xh\n"
        "e_ss      : %Xh\n"
        "e_sp      : %Xh\n"
        "e_csum    : %Xh\n"
        "e_ip      : %Xh\n"
        "e_cs      : %Xh\n"
        "e_lfarlc  : %Xh\n"
        "e_ovno    : %Xh\n"
        "e_lfanew  : %lXh\n",
        h.e_cblp, h.e_cp, h.e_crlc, h.e_cparh,
        h.e_minalloc, h.e_maxalloc, h.e_ss,
        h.e_sp, h.e_csum, h.e_ip, h.e_cs,
        h.e_lfarlc, h.e_ovno, h.e_lfanew
    );
}