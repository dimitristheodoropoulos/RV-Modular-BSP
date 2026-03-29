#include "uart.h"
#include "timer.h"
#include "config.h"

int str_equal(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { 
        s1++; 
        s2++; 
    }
    return *s1 == *s2;
}

void run_system_diagnostic(void);

void handle_command(char *cmd) {
    if (str_equal(cmd, "help")) {
        uart_print("Commands: help, status, test, read, crash, reboot\n");
    } 
    else if (str_equal(cmd, "status")) {
        uart_print(OS_NAME);
        uart_print(" v");
        uart_print(OS_VERSION);
        uart_print("\nUptime Ticks: ");
        uart_print_hex(get_time());
        uart_print("\n");
    }
    else if (str_equal(cmd, "test")) {
        run_system_diagnostic();
    }
    else if (str_equal(cmd, "read")) {
        uart_print("Acquiring Sensor Data (MEMS Sim)...\n");
        unsigned long data = get_time() % 0xFFF;
        uart_print("Value: ");
        uart_print_hex(data);
        if (data < SENSOR_MIN_VAL || data > SENSOR_MAX_VAL) {
            uart_print(" [WARN: Out of Bounds]\n");
        } else {
            uart_print(" [STATUS: Nominal]\n");
        }
    }
    else if (str_equal(cmd, "crash")) {
        uart_print("Inducing illegal state...\n");
        asm volatile(".word 0");
    }
    else if (str_equal(cmd, "reboot")) {
        uart_print("System Shutdown...\n");
        *(volatile int *)SYS_REBOOT = 0x5555;
    }
}

void kmain(void) {
    char buffer[64];
    int idx = 0;
    
    // Αρχικοποίηση UART και PLIC Interrupts
    uart_init();
    
    uart_print("\n--- ");
    uart_print(OS_NAME);
    uart_print(" Initialized (IRQ Mode) ---\n# ");

    while (1) {
        // Η uart_getc_irq περιμένει να γεμίσει ο Ring Buffer από το Interrupt
        char c = uart_getc_irq();
        
        // 1. Διαχείριση Enter (New Line)
        if (c == '\r' || c == '\n') {
            uart_print("\n");
            buffer[idx] = '\0';
            if (idx > 0) {
                handle_command(buffer);
            }
            idx = 0;
            uart_print("# ");
        } 
        // 2. Διαχείριση Backspace (ASCII 0x08 ή 0x7f)
        else if (c == 0x08 || c == 0x7f) {
            if (idx > 0) {
                idx--;
                // Σβήσιμο από την οθόνη του χρήστη:
                // Πήγαινε πίσω (\b), γράψε κενό (space), πήγαινε πάλι πίσω (\b)
                uart_putc('\b');
                uart_putc(' ');
                uart_putc('\b');
            }
        }
        // 3. Κανονικοί χαρακτήρες (μέχρι το όριο του buffer)
        else if (idx < 63) {
            buffer[idx++] = c;
            uart_putc(c); // Echo χαρακτήρα στην οθόνη
        }
    }
}