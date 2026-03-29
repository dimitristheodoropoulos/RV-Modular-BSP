CC = riscv64-linux-gnu-gcc
OBJCOPY = riscv64-linux-gnu-objcopy

# Προσθέτουμε το -Iinclude για να βρίσκει τα .h αρχεία
CFLAGS = -march=rv64gc -mabi=lp64d -static -mcmodel=medany \
         -fno-stack-protector -nostdlib -nostartfiles \
         -Wl,--build-id=none -g -Iinclude

# Λίστα όλων των πηγαίων αρχείων - Προστέθηκε ο tester.c
SRCS = src/arch/riscv/boot.S \
       src/arch/riscv/trap.c \
       src/drivers/uart.c \
       src/drivers/timer.c \
       src/kernel/main.c \
	   src/kernel/tester.c

all: os.bin

os.elf: $(SRCS) linker.ld
	$(CC) $(CFLAGS) -Tlinker.ld $(SRCS) -o os.elf

os.bin: os.elf
	$(OBJCOPY) -O binary -S os.elf os.bin

run: os.bin
	qemu-system-riscv64 -M virt -display none -serial stdio -bios os.bin

clean:
	rm -f os.elf os.bin