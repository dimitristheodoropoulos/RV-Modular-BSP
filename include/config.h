#ifndef CONFIG_H
#define CONFIG_H

// System Constants
#define OS_NAME    "RISCV-Professional-OS"
#define OS_VERSION "1.2.0-stable"

// Hardware Map (QEMU Virt)
#define UART_ADDR  0x10000000
#define CLINT_ADDR 0x02000000
#define SYS_REBOOT 0x100000

// Verification Thresholds
#define SENSOR_MIN_VAL 0x100
#define SENSOR_MAX_VAL 0xF00

#endif