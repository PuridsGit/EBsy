#include <stdint.h>
#include <LPC177x_8x.h>
#include "devicedrivers.h"
#include "helper.h"
#include "uartapp.h"
//#include "system_LPC177x_8x.h"
#define MAX_PROCS	11
#define MAX_DEVICES 10

#define GPIO_POWER_ON (1<<15)
#define NULL_BIT	8


extern void ctxswi(uintptr_t* prev, uintptr_t* next);
extern void ctxsw(uintptr_t* prev, uintptr_t* next);
extern void firstContexti(uintptr_t pointer);


//number of stacks available
uint32_t stack[MAX_PROCS-1][32];

//shows how many stacks are in use starts at 1 because of first context
//uint32_t usedstacks[MAX_PROCS-1];


/*
struct which saves the processes
pid - processId
sp - stackpointer
flags - shows if process is active or not
*/
typedef struct MyProcesses{
	uint8_t pid;
	uintptr_t sp;
	int flags;
}process_type;

struct cdevsw 
{
	int flag;
	int (*d_open)();
	int (*d_close)();
	uint8_t (*d_read)();
	int (*d_write)(uint8_t);
	char deviceName[10];
}cdevsw[MAX_DEVICES];



//table containing all running processes
process_type proc_table[MAX_PROCS];

uint8_t cur_pid=0;

int registerDevice(char deviceName[10], int ConnectionType)
{
	int i;
	int done = 0;
	for(i = 0;i< MAX_DEVICES; i++)
	{
		if(cdevsw[i].flag == 0)
		{
			cdevsw[i].flag = 1;
			//Uart0
			if(ConnectionType == 0)
			{
				cdevsw[i].d_open = &initUART0;
				cdevsw[i].d_write = &writeUART0;
				cdevsw[i].d_read = &readUART0;
				cdevsw[i].d_close = &closeUART0;
				if(copyStrings(deviceName, cdevsw[i].deviceName) == 0)
					done = 1;
				break;
			}
		}
	}
	if(done == 1)
		return i;
	else
		return -1;
}

void unregisterDevice(int deviceNumber)
{
	cdevsw[deviceNumber].flag = 0;
}


//unix like fork
void fork()
{
	uint8_t prev_pid=cur_pid;
	int i = 0;
	while(1)
	{
		cur_pid++;
		//check if maximum allowed proceses aren't running
		//try from the start
		if(cur_pid==MAX_PROCS)
		{
			cur_pid = 0;
			continue;
		}
		//check if stack and pid are free
		if(/*usedstacks[cur_pid] == 0 && */proc_table[cur_pid].flags == 0)
		{
			//mark stack as used
			//usedstacks[cur_pid] = 1;
			//copy stack
			for (i = 0; i < 32; i++)
				stack[cur_pid][i]=stack[prev_pid][i];
			//enter new process in process table
			proc_table[cur_pid].pid = cur_pid;
			proc_table[cur_pid].sp = (uintptr_t)&stack[cur_pid][32]-16*4;
			proc_table[cur_pid].flags = 1;
			//context switch to new process
			ctxswi(&proc_table[prev_pid].sp, &proc_table[cur_pid].sp);
		}
		//try again with next id
	}
}


void kill(uint8_t pid)
{
//	usedstacks[pid] = 0;
	proc_table[pid].pid = 0;
	proc_table[pid].flags = 0;
}

//yield without interupt
void yield()
{
	uint8_t prev_pid=cur_pid;
	while(1)
	{
		cur_pid++;
		if(cur_pid==MAX_PROCS)
		{
			cur_pid=0;
			continue;
		}
		if(proc_table[cur_pid].flags == 1)
			ctxsw(&proc_table[prev_pid].sp, &proc_table[cur_pid].sp);
	}
}



//yield with interupt
void yieldi()
{
	uint8_t prev_pid=cur_pid;
	while(1)
	{
		cur_pid++;
		if(cur_pid==MAX_PROCS)
			cur_pid=1;
		if(proc_table[cur_pid].flags == 1)
			ctxswi(&proc_table[prev_pid].sp, &proc_table[cur_pid].sp);
	}
}

/** @brief Hauptinitialisierungsroutine */
void init(void)
{
	//tick_counter=0; 		//Initialisieren des globalen Systemtick Zählers.
	
	SystemInit();			//MCU-Bootstraping
	//SysTick_Config(100000);		//Konfiguration des SysTick-Interrupts für 1mS.
	
	LPC_SC->PCONP |= GPIO_POWER_ON; //GPIO-Ports einschalten
	LPC_GPIO0->DIR=0xff00;		//LED Pins als Ausgänge konfigurieren
	LPC_GPIO0->SET=0xff00;		//LED's ausschalten
	//Delay(1000);
}


void p1()
{
	volatile int i;
	while(1){
		//ledon();
		//yield();
		i++;
	};
}

void p2()
{
	volatile int i;
	while(1){
		//ledoff();
		//yield();
		i++;
	};

}
void idle()
{
	while(1) {
		yieldi();
	}
}

void init_proc_table()
{
	int i;
	//for(i = 0; i < 10; i++)
	//	usedstacks[i] = 0;
	for(i=0; i<MAX_PROCS; i++)
	{
		if(i < MAX_DEVICES)
			cdevsw[i].flag=0;
		proc_table[i].sp=(uintptr_t)&stack[i][32]-16*4;
		switch(i){
			case 1: 
				proc_table[i].pid=i;
				stack[i][30]= (uintptr_t)uartapplication;		
				stack[i][31]= 0x21000000; 						
				proc_table[i].flags=1;
				//usedstacks[i] = 1;
			break;
			case 2:  
				proc_table[i].pid=i;
				stack[i][30]= (uintptr_t)p2; 
				stack[i][31]= 0x21000000; 			
				proc_table[i].flags=1;
				//usedstacks[i] = 1;
			break;
			default: 
				proc_table[i].pid=0;
				stack[i][30]= (uintptr_t)idle;
				stack[i][31]= 0x21000000; 			
				proc_table[i].flags=0;
			break;
		}

	}
	cur_pid=1;
	SysTick_Config(2400000);
	firstContexti(proc_table[cur_pid].sp);
	//Wird nie erreicht------
}

void SysTick_Handler()
{
	//umschalten auf PSP
	yieldi();
}

/*void HardFault_Handler()
{
	idle();
}*/
