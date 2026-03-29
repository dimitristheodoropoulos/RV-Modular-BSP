#include "uart.h"

// Διευθύνσεις PLIC για QEMU Virt
#define PLIC_PRIORITY      (volatile unsigned int *)0x0c000028 
#define PLIC_MENABLE       (volatile unsigned int *)0x0c002000 
#define PLIC_MTHRESHOLD    (volatile unsigned int *)0x0c200000 
#define PLIC_MCLAIM        (volatile unsigned int *)0x0c200004 

// volatile Ring Buffer
volatile RingBuffer rx_fifo = { .head = 0, .tail = 0 };

void uart_init(void) {
    // 1. UART Hardware Setup (Enable FIFO & Interrupts)
    *(volatile unsigned char *)(UART_BASE + 2) = 0x01; 
    *(volatile unsigned char *)(UART_BASE + 1) = 0x01; 

    // 2. PLIC Setup για UART (ID 10)
    *PLIC_PRIORITY = 1;              
    *PLIC_MENABLE = (1 << 10);       
    *PLIC_MTHRESHOLD = 0;            
}

void uart_putc(char c) {
    // Περίμενε μέχρι ο UART να είναι έτοιμος να στείλει
    while (!(*(volatile char *)UART_LSR & (1 << 5)));
    *(volatile char *)UART_BASE = c;
}

void uart_print(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void uart_handle_irq(void) {
    unsigned int irq_id = *PLIC_MCLAIM;

    if (irq_id == 10) { 
        char c = *(volatile char *)UART_BASE;
        int next = (rx_fifo.head + 1) % RING_BUF_SIZE;
        if (next != rx_fifo.tail) {
            rx_fifo.buffer[rx_fifo.head] = c;
            rx_fifo.head = next;
        }
    }
    if (irq_id > 0) {
        *PLIC_MCLAIM = irq_id; // Acknowledge στον PLIC
    }
}

char uart_getc_irq(void) {
    // Spin μέχρι να έρθει κάτι στον buffer από το interrupt
    while (rx_fifo.head == rx_fifo.tail);
    
    char c = rx_fifo.buffer[rx_fifo.tail];
    rx_fifo.tail = (rx_fifo.tail + 1) % RING_BUF_SIZE;
    return c;
}

void uart_print_hex(unsigned long val) {
    char *hex = "0123456789ABCDEF";
    uart_print("0x");
    for (int i = 60; i >= 0; i -= 4) {
        uart_putc(hex[(val >> i) & 0xF]);
    }
}