#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "checksum.h"
#include "uart.h"

// Open the uart
int setup_uartconn() {
        int uart0_filestream = -1;
        uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NDELAY | O_NOCTTY);
        if (uart0_filestream == -1) {
                printf("Error - unable to open UART");
                return -1;
        }

        struct termios options;
        tcgetattr(uart0_filestream, &options);
        options.c_cflag = B4800 | CS8 | CREAD | CLOCAL;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart0_filestream, TCIOFLUSH);
        tcsetattr(uart0_filestream, TCSANOW, &options);
        return uart0_filestream;
}

int enable_debug_mode(int uart0_filestream) {
        unsigned char buff[] = "$PSRF105,1*3E\r\n";
        return write_uart(uart0_filestream, &buff[0]);
}

int set_default_options(int uart0_filestream) {
        unsigned char buff[] = "$PSRF100,1,4800,1,0*1A\r\n";
        return write_uart(uart0_filestream, &buff[0]);
}

int read_uart(int uart0_filestream, unsigned char *rx_buffer, size_t bufflen) {
        if (uart0_filestream != -1) {
                int rx_length = read(uart0_filestream, (void *)rx_buffer, bufflen);
                if (rx_length < 0) {
                        //      printf("Error during read: %i\n", rx_length);
                        return 0;
                } else if (rx_length == 0) {
                        printf("No data waiting");
                        return rx_length;
                } else {
                        rx_buffer[rx_length] = '\0';
                        return rx_length;
                }
        }
        return -1;
}

void close_uartconn(int handle) { close(handle); }

int query_gga(int handle) {
        unsigned char buff[] = "$PSRF103,00,01,00,01*25\r\n";
        return write_uart(handle, &buff[0]);
}

int set_vtg_message(int handle) {
        // return write_uart(handle, "$PSRF103,05,00,00,01*21\r\n");
        return 0;
}

int write_uart(int handle, unsigned char *message) {
        size_t len = strlen(message);
        unsigned char *fixed = checksum(message);
        printf("Sending.[%iu] bytes\nMSG: %s", len, fixed);
        if (handle != -1) {
                int count = write(handle, fixed, len);
                if (count < 0) {
                        free(fixed);
                        return -1;
                }
                free(fixed);
                return 0;
        }
        free(fixed);
        return -1;
}

int set_message_freq(int handle) {
        unsigned char *setgga = "$PSRF103,00,00,10,01*20\r\n";
        unsigned char *setgll = "$PSRF103,01,00,10,01*20\r\n";
        unsigned char *setgsa = "$PSRF103,02,00,10,01*20\r\n";
        unsigned char *setgsv = "$PSRF103,03,00,10,01*20\r\n";
        unsigned char *setrmc = "$PSRF103,04,00,10,01*20\r\n";
        unsigned char *setvtg = "$PSRF103,05,00,10,01*20\r\n";
        write_uart(handle, setgga);
        write_uart(handle, setgll);
        write_uart(handle, setgsa);
        sleep(1);
        write_uart(handle, setgsv);
        sleep(1);
        write_uart(handle, setrmc);
        write_uart(handle, setvtg);
        return 0;
}
