#ifndef oscore_H
#define oscore_H

#include <stdint.h>
#include <LPC177x_8x.h>


void ctxsw(uintptr_t*, uintptr_t*);
void ctxswi(uintptr_t*, uintptr_t*);
void firstContext(uintptr_t);
void firstContexti(uintptr_t);

extern int registerDevice(char[10], int);
extern void init_proc_table(void);

#endif
