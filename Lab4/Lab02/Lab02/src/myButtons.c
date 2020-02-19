////////////////////////////////////////////////////////////////////////////////
//File: myButtons.h
//Author:	Chad Revel
//This is the button main file of the fourth lab for CST 347
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
	ioport_set_pin_dir(SW0, IOPORT_DIR_INPUT);
	
	//sets the pin mode for the external switches 1 and 2 to turn on pullup resistors and debounce them
	ioport_set_pin_mode(EXT_SW1, (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE));
	ioport_set_pin_mode(EXT_SW2, (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE));
	
	//sets the sensing mode for the external switches 1 and 2 for when they are pushed
	ioport_set_pin_sense_mode(EXT_SW1, (IOPORT_SENSE_RISING));
	ioport_set_pin_sense_mode(EXT_SW2, (IOPORT_SENSE_RISING));
}

uint8_t readButton(uint8_t uiButtonNum)
{
	//if the button number is 1, then it is for the external button #1
	if (uiButtonNum == 1)
	{
		return ioport_get_pin_level(EXT_SW1);
	}
	//else if it is 2, then it is for the external button #2
	else if (uiButtonNum == 2)
	{
		return ioport_get_pin_level(EXT_SW2);
	}
	//else it is the built in board button
	else
		return ioport_get_pin_level(BUTTON_0_PIN);

}