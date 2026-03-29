#include "timer.h"

unsigned long get_time(void) {
    return *(volatile unsigned long *)CLINT_MTIME;
}

void sleep_ms(unsigned long ms) {
    unsigned long start = get_time();
    // Υπολογίζουμε πόσα ticks πρέπει να περάσουν
    unsigned long wait = ms * (TIMER_FREQ / 1000);
    
    while (get_time() - start < wait) {
        // Busy-wait loop (για τώρα)
    }
}