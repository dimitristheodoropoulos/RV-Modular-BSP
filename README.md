# RV-Modular-BSP: Professional RISC-V Bare-Metal Framework

A modular, hardware-agnostic Board Support Package (BSP) and minimal kernel developed for the **RISC-V 64-bit** architecture. This project demonstrates the complete system bring-up process, from the reset vector to a functional, asynchronous command-line interface (CLI).

## 🎯 Engineering Objectives

This framework was engineered to address four critical pillars of modern embedded systems:
1.  **Deterministic Execution:** Register-level control of system timing and hardware peripherals.
2.  **Asynchronous I/O:** Implementation of an **Interrupt-Driven** architecture to maximize CPU efficiency.
3.  **System Robustness:** A low-level **Trap Handling** mechanism to manage hardware exceptions and PLIC-based interrupts.
4.  **Hardware Verification:** A built-in automated diagnostic suite for IP/IC integrity verification (Self-Test/POST).

## 🚀 Core Technical Features

### 1. Advanced Interrupt Architecture
* **PLIC Integration:** Full configuration of the **Platform Level Interrupt Controller** (Priority, Enable, Threshold, Claim/Complete cycle).
* **Asynchronous UART Driver:** Transitioned from Polling to **Interrupt-Driven I/O** for the UART 16550.
* **Circular Buffer (Ring Buffer):** Implementation of a thread-safe (volatile) `rx_fifo` to handle high-speed serial data without loss.

### 2. Low-Level System Bring-up (Bootstrapping)
* **Custom Bootloader:** Handwritten Assembly (`boot.S`) managing Hart initialization, CSR configuration (MIE, MSTATUS, MTVEC), and Stack Pointer setup.
* **Context Management:** Explicit context saving/restoring in the Trap Vector to ensure safe transitions between the foreground shell and background IRQs.

### 3. Hardware Abstraction Layer (HAL)
* **Decoupled Architecture:** Strict separation between `src/arch` (RISC-V specific), `src/drivers` (Hardware IP), and `src/kernel` (Logic).
* **Memory-Mapped I/O (MMIO):** Custom drivers developed using direct register manipulation for UART and CLINT (Machine Timer).

### 4. Integrated Diagnostic & Verification
* **Verification Suite:** A dedicated `tester.c` module providing automated "Pass/Fail" diagnostics for system timers and memory integrity.
* **Robust Shell:** A CLI supporting backspace handling, input validation, and real-time command execution.

## 🛠 Project Structure


├── src/arch/riscv/   # Bootstrap (boot.S) and Trap Handlers (trap.c)
├── src/drivers/      # Bare-metal drivers (UART, Timer, PLIC logic)
├── src/kernel/       # Main logic, CLI Shell, and Diagnostic Suite
├── include/          # Professional HAL Headers and System Config
└── linker.ld         # Explicit memory layout and section definitions


## 💻 Building and Emulation

### Prerequisites
* RISC-V GNU Toolchain (`riscv64-linux-gnu-gcc`)
* QEMU (System RISC-V)

### Workflow

make clean    # Remove previous builds
make          # Cross-compile and generate binary image (os.bin)
make run      # Execute on QEMU Virt Machine


## 🔍 Verification Commands
* `test`: Executes the **Automated Hardware Verification** suite.
* `read`: Performs a **Simulated Data Acquisition** cycle with software bounds checking.
* `status`: Reports system uptime (via CLINT) and hardware metadata.
* `crash`: Induces a controlled illegal instruction to verify **Exception Handling**.
* `help`: Displays available system commands.



