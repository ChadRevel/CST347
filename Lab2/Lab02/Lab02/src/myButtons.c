////////////////////////////////////////////////////////////////////////////////
//File: myButtons.h
//Author:	Chad Revel
//This is the drivers header file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#include <asf.h>
#include <FreeRTOS.h>
#include "myButtons.h"
#include "myDefines.h"

void initializeButtonDriver(void)
{
	//set the pins to input
	ioport_set_pin_dir(EXT_SW1, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(EXT_SW2, IOPORT_DIR_INPUT);
	
	//ioport_set_pin_mode(PIN_NUM, MODE) 
	//and ioport_set_pin_sense_mode(PIN_NUM, SENSE).
	ioport_set_pin_mode(EXT_SW1, (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE));
	ioport_set_pin_mode(EXT_SW2, (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE));
	
	//need comments!
	ioport_set_pin_sense_mode(EXT_SW1, (IOPORT_SENSE_RISING));
	ioport_set_pin_sense_mode(EXT_SW1, (IOPORT_SENSE_RISING));
}

int readButton(uint8_t uiButtonNum)
{
	if (uiButtonNum == 1)
	{
		return ioport_get_pin_level(EXT_SW1);
	}
	else if (uiButtonNum == 2)
	{
		return ioport_get_pin_level(EXT_SW2);
	}
	else
		return ioport_get_pin_level(BUTTON_0_PIN);
	
		//if (uiLedNum == 1)
		//{
			//return ioport_get_pin_level(EXT_LED1);
		//}
		//else if (uiLedNum == 2)
		//{
			//return ioport_get_pin_level(EXT_LED2);
		//}
		//else if (uiLedNum == 3)
		//{
			//return ioport_get_pin_level(EXT_LED3);
		//}
		//else
		//{
			//return ioport_get_pin_level(LED_0_PIN);
		//}
	
}