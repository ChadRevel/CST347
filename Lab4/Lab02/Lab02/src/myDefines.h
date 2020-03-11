////////////////////////////////////////////////////////////////////////////////
//File: myDrivers.h
//Author:	Chad Revel
//This is the drivers header file of the fourth lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#ifndef MYDEFINES_H_
#define MYDEFINES_H_

#define EXT_LED1	PIO_PD28_IDX
#define EXT_LED2	PIO_PD17_IDX
#define EXT_LED3	PIO_PE1_IDX
#define EXT_SW1		PIO_PD26_IDX
#define EXT_SW2		PIO_PD30_IDX


//other variables to be used in myTasks
#define LED0	0
#define LED1	1
#define LED2	2
#define LED3	3
 
#define SW0		0
#define SW1		1
#define SW2		2

#define heartbeatPriority	1
#define uartControlPriority 2
#define mainControlPriority 3
#define led1ControlPriority 4
#define led2ControlPriority 4
#define led3ControlPriority 4

#endif