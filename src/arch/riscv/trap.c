#include "uart.h"

void handle_trap(void) {
    unsigned long mcause;
    asm volatile("csrr %0, mcause" : "=r"(mcause));

    if (mcause & (1L << 63)) {
        // Interrupt bit 63 is set
        unsigned long id = mcause & 0xFF;
        if (id == 11) { // Machine External Interrupt (προέρχεται από PLIC)
            uart_handle_irq();
        }
    } else {
        // Exception logic
        uart_print("\n[EXCEPTION] ID: ");
        uart_print_hex(mcause);
        while(1);
    }
}