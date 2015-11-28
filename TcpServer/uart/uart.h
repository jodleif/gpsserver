#ifndef UART_H
#define UART_H
extern int setup_uartconn(void);
int enable_debug_mode(int);
int set_default_options(int uart0_filestream);
int query_gga(int);
int set_vtg_message(int handle);
int set_message_freq(int handle);
extern int write_uart(int handle, unsigned char *message);
extern int read_uart(int uart0_filestream, unsigned char *rx_buffer, size_t bufflen);
extern void close_uartconn(int handle);
#endif //
