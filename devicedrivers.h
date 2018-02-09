#ifndef devicedrivers_H
#define devicedrivers_H

#include <stdint.h>
#include <LPC177x_8x.h>

extern int initUART0(void);
extern int writeUART0(uint8_t data);
extern uint8_t readUART0(void);
extern int closeUART0(void);

#endif
