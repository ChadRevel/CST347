////////////////////////////////////////////////////////////////////////////////
//File:	ledDriver.c
//Author:	Chad Revel
//This is the led driver file of the third lab for CST 347
////////////////////////////////////////////////////////////////////////////////
#include <asf.h>
#include <FreeRTOS.h>
#include "ledDriver.h"
#include "myDefines.h"


void intitializeLEDDriver(void)
{
	/*
	Responsible for initializing the LEDs,
	this should be called from the Hardware Init Function in main.
	It will setup the ports for the LEDs and set them to the OFF
	state.
	*/
	ioport_init();
	
	//sets up the LEDs to output on the io ports.
	ioport_set_pin_dir(EXT_LED1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(EXT_LED2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(EXT_LED3, IOPORT_DIR_OUTPUT);
	
	//set the leds to low, so they are inactive
	ioport_set_pin_level(EXT_LED1, 1);
	ioport_set_pin_level(EXT_LED2, 1);
	ioport_set_pin_level(EXT_LED3, 1);
	
	
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
	//if the led number is 1, then that will be for the external led #1
	if (uiLedNum == 1)
	{
		return ioport_get_pin_level(EXT_LED1);
	}
	//else if the led number is 2, then that will be for the external led #2
	else if (uiLedNum == 2)
	{
		return ioport_get_pin_level(EXT_LED2);
	}
	//else if the led number is 3, then that will be for the external led #3 
	else if (uiLedNum == 3)
	{
		return ioport_get_pin_level(EXT_LED3);
	}
	//else the ioport will be set for the on board led
	else
	{
		return ioport_get_pin_level(LED_0_PIN);
	}
	
}

uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue)
{
/*
Sets the LED to a specified value (ON/OFF)
*/
		//if the led number is 1, then set the pin level
		//to on for for the external led #1, and then return the readLED
		if (uiLedNum == 1)
		{
			ioport_set_pin_level(EXT_LED1, uiLedValue);
			return readLED(uiLedNum);			
		}
		//else if the led number is 2, then set the pin level
		//to on for for the external led #2, and then return the readLED
		else if (uiLedNum == 2)
		{
			ioport_set_pin_level(EXT_LED2, uiLedValue);
			return readLED(uiLedNum);			
		}
		//else if the led number is 3, then set the pin level
		//to on for for the external led #3, and then return the readLED
		else if (uiLedNum == 3)
		{
			ioport_set_pin_level(EXT_LED3, uiLedValue);
			return readLED(uiLedNum);	
		}
		//else set the on board led to on, then return the readLED
		else
		{
			ioport_set_pin_level(LED_0_PIN, uiLedValue);
			return readLED(uiLedNum);
		}
}

uint8_t toggleLED(uint8_t uiLedNum)
{
/*
Toggles the LED
If the led is on, then turn it off
If the led is off, then turn it on
*/
	if (readLED(uiLedNum) == 1)
	{
		return setLED(uiLedNum, 0);
		
	}
	else
		return setLED(uiLedNum, 1);
	
	
}
