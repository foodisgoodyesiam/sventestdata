#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistr.h>
#include <unictype.h>
#include <errno.h>
#include <string.h>

static const char* programName;

static FILE* myOpen(const char* path) {
    FILE* ret = fopen(path, "wt");
    if (__builtin_expect(ret==NULL, false)) {
        fprintf(stderr, "%s: could not open %s for writing.txt: %s\n", programName, path, strerror(errno));
        exit(1);
    }
    return ret;
}

int main(int argNum, const char** args) {
    programName = args[0];
    ucs4_t c = 0;
    FILE* out_all = myOpen("allutf8fortesting.txt");
    FILE* out_assigned = myOpen("assignedutf8fortesting.txt");
    char outBuf[16];
    unsigned numInvalid = 0;
    for (ucs4_t c = 1; c<160000; c++) {
        int numBytes = u8_uctomb(outBuf, c, 16);
        if (__builtin_expect(numBytes<1, false)) {
//            fprintf(stderr, "%s: could not convert code point %" PRIu32 " to multi-byte\n", programName, (uint32_t)c);
//            exit(1);
            numInvalid ++;
        } else {
            fwrite(outBuf, 1, numBytes, out_all);
            if (!uc_is_property_unassigned_code_value(c))
                fwrite(outBuf, 1, numBytes, out_assigned);
        }
    }
    fclose(out_all);
    fclose(out_assigned);
    if (numInvalid > 0)
        fprintf(stderr, "%s: %u code points that could not be converted to multi-byte\n", programName, numInvalid);
}
