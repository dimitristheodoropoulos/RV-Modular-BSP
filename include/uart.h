#ifndef UART_H
#define UART_H

#define UART_BASE 0x10000000
#define UART_LSR  (UART_BASE + 5)
#define RING_BUF_SIZE 128

typedef struct {
    char buffer[RING_BUF_SIZE];
    int head;
    int tail;
} RingBuffer;

void uart_init(void);
void uart_putc(char c);
void uart_print(const char *s);
char uart_getc_irq(void);
void uart_print_hex(unsigned long val);
void uart_handle_irq(void);

#endif