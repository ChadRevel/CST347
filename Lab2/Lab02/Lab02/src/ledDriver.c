////////////////////////////////////////////////////////////////////////////////
//File:	ledDriver.c
//Author:	Chad Revel
//This is the led driver file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////
#include <FreeRTOS.h>
#include "ledDriver.h"

void intitializeLEDDriver(void)
{
	/*
	Responsible for initializing the LEDs,
	this should be called from the Hardware Init Function in main.
	It will setup the ports for the LEDs and set them to the OFF
	state.
	*/
	
	
}

uint8_t readLED(uint8_t uiLedNum)
{
	/*
	returns the current state of the given LED
	uiLedNum Defined as:
	0 = Onboard LED
	1 = EXT_LED1
	2 = EXT_LED2
	3 = EXT_LED3
	*/
	
	
}

uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue)
{
/*
Sets the LED to a specified value (ON/OFF)
*/
		
}

uint8_t toggleLED(uint8_t uiLedNum)
{
/*
Toggles the LED
If on turns off
If off turns on
*/
	
	
}

/*
Next you will need to design a button driver as well.
It should be similar to the LED driver with functions
of initializeButtonDriver and readButton.
*/
