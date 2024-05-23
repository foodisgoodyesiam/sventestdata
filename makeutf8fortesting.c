#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistr.h>
#include <unictype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <endian.h>

static const char* programName;

static FILE* myOpen(const char* path) {
    FILE* ret = fopen(path, "wt");
    if (__builtin_expect(ret==NULL, false)) {
        fprintf(stderr, "%s: could not open %s for writing: %s\n", programName, path, strerror(errno));
        exit(1);
    }
    return ret;
}

// TODO: add proper error handling on the fwrite calls, maybe
// TODO: consider adding the byte order mark at the start of the UTF-16 and UTF-32

// I probably could make this "cleaner" with functional decomposition but it's so simple it doesn't matter...

#ifndef MAX_UNICODE_CODEPOINT
#   define MAX_UNICODE_CODEPOINT 160000U
#endif // MAX_UNICODE_CODEPOINT
#define UTF32_BUFFER_SIZE (MAX_UNICODE_CODEPOINT*2U)

int main(int argNum, const char** args) {
    programName = args[0];

    // UTF-8
    FILE* out_all = myOpen("all_utf8_fortesting.txt");
    FILE* out_assigned = myOpen("assigned_utf8_fortesting.txt");
    char outBuf8[16];
    unsigned numInvalid = 0;
    for (ucs4_t c = 1; c<MAX_UNICODE_CODEPOINT; c++) {
        const int numBytes = u8_uctomb(outBuf8, c, 16);
        if (__builtin_expect(numBytes<1, false)) {
//            fprintf(stderr, "%s: could not convert code point %" PRIu32 " to multi-byte\n", programName, (uint32_t)c);
//            exit(1);
            numInvalid ++;
        } else {
            fwrite(outBuf8, 1, numBytes, out_all);
            if (!uc_is_property_unassigned_code_value(c))
                fwrite(outBuf8, 1, numBytes, out_assigned);
        }
    }
    fclose(out_all);
    fclose(out_assigned);

    // UTF16LE
    out_all = myOpen("all_utf16le_fortesting.txt");
    out_assigned = myOpen("assigned_utf16le_fortesting.txt");
    uint16_t outBuf16[16];
    for (ucs4_t c = 1; c<MAX_UNICODE_CODEPOINT; c++) {
        const int numUnits = u16_uctomb(outBuf16, c, 16);
        if (__builtin_expect(numUnits>=0, true)) {
            for (int i=0; i<numUnits; i++)
                outBuf16[i] = htole16(outBuf16[i]);
            fwrite(outBuf16, 2, numUnits, out_all);
            if (!uc_is_property_unassigned_code_value(c))
                fwrite(outBuf16, 2, numUnits, out_assigned);
        }
    }
    fclose(out_all);
    fclose(out_assigned);

    // UTF16BE
    out_all = myOpen("all_utf16be_fortesting.txt");
    out_assigned = myOpen("assigned_utf16be_fortesting.txt");
    for (ucs4_t c = 1; c<MAX_UNICODE_CODEPOINT; c++) {
        const int numUnits = u16_uctomb(outBuf16, c, 16);
        if (__builtin_expect(numUnits>=0, true)) {
            for (int i=0; i<numUnits; i++)
                outBuf16[i] = htobe16(outBuf16[i]);
            fwrite(outBuf16, 2, numUnits, out_all);
            if (!uc_is_property_unassigned_code_value(c))
                fwrite(outBuf16, 2, numUnits, out_assigned);
        }
    }
    fclose(out_all);
    fclose(out_assigned);

    // UTF32LE, all
    size_t cur_index = 0;
    uint32_t outBuf32[UTF32_BUFFER_SIZE];
    for (ucs4_t c = 1; c<MAX_UNICODE_CODEPOINT && cur_index<UTF32_BUFFER_SIZE; c++) {
        // in principle, using libunistring for utf32 is unnecessary, I think. But whatever. Maybe this makes it future-proof if multibyte UTF-32 is introduced...
        // TODO: examine the generated assembly here. It probably simplifies this a lot
        const int numUnits = u32_uctomb(&outBuf32[cur_index], c, UTF32_BUFFER_SIZE-cur_index);
        cur_index+=numUnits;
    }
    for (size_t i=0; i<cur_index; i++)
        outBuf32[i] = htole32(outBuf32[i]);
    out_all = myOpen("all_utf32le_fortesting.txt");
    fwrite(outBuf32, 4, cur_index, out_all);
    fclose(out_all);

    // UTF32BE, all
    for (size_t i=0; i<cur_index; i++)
        outBuf32[i] = htobe32(outBuf32[i]);
    out_all = myOpen("all_utf32be_fortesting.txt");
    fwrite(outBuf32, 4, cur_index, out_all);
    fclose(out_all);

    // UTF32LE, assigned
    cur_index = 0;
    for (ucs4_t c = 1; c<MAX_UNICODE_CODEPOINT && cur_index<UTF32_BUFFER_SIZE; c++) {
        if (!uc_is_property_unassigned_code_value(c)) {
            const int numUnits = u32_uctomb(&outBuf32[cur_index], c, UTF32_BUFFER_SIZE-cur_index);
            cur_index += numUnits;
        }
    }
    for (size_t i=0; i<cur_index; i++)
        outBuf32[i] = htole32(outBuf32[i]);
    out_assigned = myOpen("assigned_utf32le_fortesting.txt");
    fwrite(outBuf32, 4, cur_index, out_assigned);
    fclose(out_assigned);

    // UTF32BE, assigned
    for (size_t i=0; i<cur_index; i++)
        outBuf32[i] = htobe32(outBuf32[i]);
    out_assigned = myOpen("assigned_utf32be_fotesting.txt");
    fwrite(outBuf32, 4, cur_index, out_assigned);
    fclose(out_assigned);

    if (numInvalid > 0)
        fprintf(stderr, "%s: %u code points that could not be converted to multi-byte\n", programName, numInvalid);
}
