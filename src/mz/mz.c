#include <stdio.h>
#include "../utils.h"
#include "mz.h"
#include "pe.h"
//#include "ne.h"
//#include "le.h"

void scan_mz() {
    struct mz_hdr h;

	if (_ddseek(0x3c, SEEK_SET)) goto _MZ;

	unsigned int p;
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
    _ddseek(0, SEEK_SET);
    _ddread(&h, sizeof(struct mz_hdr));

    printf("MZ executable for MS-DOS");
    if (h.e_ovno)
        printf(" (Overlay: %d)", h.e_ovno);

    printf(
        "\ncblp    : %Xh\n"
        "cp      : %Xh\n"
        "crlc    : %Xh\n"
        "cparh   : %Xh\n"
        "minalloc: %Xh\n"
        "maxalloc: %Xh\n"
        "ss      : %Xh\n"
        "sp      : %Xh\n"
        "csum    : %Xh\n"
        "ip      : %Xh\n"
        "cs      : %Xh\n"
        "lfarlc  : %Xh\n"
        "ovno    : %Xh\n"
        "lfanew  : %Xh\n",
        h.e_cblp, h.e_cp, h.e_crlc, h.e_cparh,
        h.e_minalloc, h.e_maxalloc, h.e_ss,
        h.e_sp, h.e_csum, h.e_ip, h.e_cs,
        h.e_lfarlc, h.e_ovno, h.e_lfanew
    );

    if (h.e_crlc) {
        short i = 0;
        struct mz_rlc r;
        _ddseek(h.e_lfarlc, SEEK_SET);
        puts("\nRelocations\n");
        do {
            _ddread(&r, sizeof(struct mz_rlc));
            printf(
                "%2d  Segment:%04X  Offset:%04X\n",
                ++i, r.seg, r.off
            );
        } while (--h.e_crlc);
    } else puts("\nNo relocations");
}