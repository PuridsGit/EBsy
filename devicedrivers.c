#include "devicedrivers.h"
#include "oscore.h"

int initUART0()
{
	//power pins
	LPC_SC->PCONP |= (1<<3);
	//set devider for perifiral 
	LPC_SC->PCLKSEL = 4;
	
	//baut rate
	//unlock registers
	LPC_UART0->LCR |= (1<<7);
	//configure baud rate
	LPC_UART0->DLL = 195;
	LPC_UART0->DLM = 0;
	//LPC_UART0->LCR &= ~0x3;
	LPC_UART0->LCR |= 3;
	
	//fifo on
	LPC_UART0->FCR |= 1;
	//configure pins
	LPC_IOCON->P0_2 &=~(5);
	LPC_IOCON->P0_2 |= 1;
	LPC_IOCON->P0_3 &=~(5);
	LPC_IOCON->P0_3 |= 1;
	
	//activare interupts
	LPC_UART0->LCR &= ~(1<<7);
	LPC_UART0->IER |= 0x3;
	
	//register device
	char deviceName[10] = "Uart0";
	if(registerDevice(deviceName, 0) == -1)
		return 1;
	
	//TODO NVIC register aktivieren Doku S72 | vorlesung 8
	return 0;	
	
}

int writeUART0(uint8_t data)
{
	//wait till THR is empty
	while(!(LPC_UART0->LSR & (1<<6)));
	//write to Tx Fifo
	LPC_UART0->THR = data;
	return 0;
}

uint8_t readUART0(void)
{
	//wait till there is something to read
	while(!(LPC_UART0->LSR & 0x1));
	//read register and return it
	return LPC_UART0->RBR & 0xff;
}

int closeUART0()
{
	return 0;
}
