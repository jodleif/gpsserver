#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "uart.h"
#include "checksum.h"

// transmit_bytes("$ PSRF105,1,*3E\r\n");
int main(void) {
        printf("UART - READER\n");
        // unsigned char expr[] = "$PSRF100,1,4800,1,0*0C\r\n";
        // const unsigned char* ptr = checksum(&expr[0]);
        // printf("%s",ptr);

        constexpr size_t bufflen = 512u;
        int handle = setup_uartconn();
        unsigned char *buffer = (unsigned char *)malloc(bufflen);
        if (handle != -1) {
                set_message_freq(handle);
                for (int i = 0; i < 100; ++i) {
                        read_uart(handle, buffer, bufflen);
                        sleep(1);
                }
                close_uartconn(handle);
        }
        free(buffer);

        return 0;
}
