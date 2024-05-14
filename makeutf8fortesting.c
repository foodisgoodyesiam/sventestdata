#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistr.h>
#include <unictype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static const char* programName;

static FILE* myOpen(const char* path) {
    FILE* ret = fopen(path, "wt");
    if (__builtin_expect(ret==NULL, false)) {
        fprintf(stderr, "%s: could not open %s for writing.txt: %s\n", programName, path, strerror(errno));
        exit(1);
    }
    return ret;
}

// TODO: figure out how this stuff depends on endianness for utf-16 and utf-32

// I probably could make this "cleaner" with functional decomposition but it's so simple it doesn't matter...

int main(int argNum, const char** args) {
    programName = args[0];
    ucs4_t c = 0;

    // UTF-8
    FILE* out_all_utf8 = myOpen("allutf8fortesting.txt");
    FILE* out_assigned_utf8 = myOpen("assignedutf8fortesting.txt");
    char outBuf8[16];
    unsigned numInvalid = 0;
    for (ucs4_t c = 1; c<160000; c++) {
        int numBytes = u8_uctomb(outBuf8, c, 16);
        if (__builtin_expect(numBytes<1, false)) {
//            fprintf(stderr, "%s: could not convert code point %" PRIu32 " to multi-byte\n", programName, (uint32_t)c);
//            exit(1);
            numInvalid ++;
        } else {
            fwrite(outBuf8, 1, numBytes, out_all_utf8);
            if (!uc_is_property_unassigned_code_value(c))
                fwrite(outBuf8, 1, numBytes, out_assigned_utf8);
        }
    }
    fclose(out_all_utf8);
    fclose(out_assigned_utf8);

    // UTF16
    FILE* out_all_utf16 = myOpen("allutf16fortesting.txt");
    FILE* out_assigned_utf16 = myOpen("assignedutf16fortesting.txt");
    uint16_t outBuf16[16];
    for (ucs4_t c = 1; c<160000; c++) {
        int numUnits = u16_uctomb(outBuf16, c, 16);
        if (__builtin_expect(numUnits>=0, true)) {
            fwrite(outBuf16, 2, numUnits, out_all_utf16);
            if (!uc_is_property_unassigned_code_value(c))
                fwrite(outBuf16, 2, numUnits, out_assigned_utf16);
        }
    }
    fclose(out_all_utf16);
    fclose(out_assigned_utf16);

    // TODO utf-32

    // TODO endianness

    if (numInvalid > 0)
        fprintf(stderr, "%s: %u code points that could not be converted to multi-byte\n", programName, numInvalid);
}
