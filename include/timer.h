#ifndef TIMER_H
#define TIMER_H

#define CLINT_BASE       0x2000000
#define CLINT_MTIME      (CLINT_BASE + 0xBFF8)

// Στο QEMU virt, ο timer αυξάνεται κατά 10.000.000 κάθε δευτερόλεπτο
#define TIMER_FREQ       10000000

unsigned long get_time(void);
void sleep_ms(unsigned long ms);

#endif