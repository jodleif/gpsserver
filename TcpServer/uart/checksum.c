#include "checksum.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char *checksum(unsigned char *str) {
        printf("strlen");
        size_t len = strlen(str);
        unsigned char *ptr = malloc(len + 1);
        strcpy(ptr, str);
        unsigned char *backup = ptr;
        printf("...done %i\n", len);
        const char flag = 0xF0;
        const char flag2 = 0x0F;
        unsigned char _checksum = *(ptr + 1);
        ptr = ptr + 2;
        for (; ptr < (backup + len - 5); ++ptr) {
                _checksum ^= *ptr;
        }
        ++ptr;
        *ptr = to_hex((_checksum & flag) >> 4);
        ++ptr;
        *ptr = to_hex((_checksum & flag2));
        return backup;
}

unsigned char to_hex(unsigned char ch) {
        if (ch >= 0 && ch <= 9) {
                return (ch + '0');
        }
        if (ch >= 10 && ch <= 15) {
                return (ch + 'A' - 10);
        }
        return 255;
}
