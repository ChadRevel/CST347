////////////////////////////////////////////////////////////////////////////////
//File: myButtons.h
//Author:	Chad Revel
//This is the button main file of the fifth lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#include <asf.h>
#include <FreeRTOS.h>
#include "myButtons.h"
#include "myDefines.h"

int currButton = 0;
extern TaskHandle_t buttonHandle;

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
	
	//lab 5 define calls
	//onboard switch 0
	pio_configure(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_TYPE, PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID, PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR, button_handler);
	pio_handler_set_priority(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_IRQn, 5);
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);
	
	//external switch 1
	pio_configure(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_TYPE, PIN_PUSHBUTTON_2_MASK, PIN_PUSHBUTTON_2_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_ID, PIN_PUSHBUTTON_2_MASK, PIN_PUSHBUTTON_2_ATTR, button_handler);
	pio_handler_set_priority(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_IRQn, 5);
	pio_enable_interrupt(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK);
	
	//external switch 2
	pio_configure(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_TYPE, PIN_PUSHBUTTON_3_MASK, PIN_PUSHBUTTON_3_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_ID, PIN_PUSHBUTTON_3_MASK, PIN_PUSHBUTTON_3_ATTR, button_handler);
	pio_handler_set_priority(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_IRQn, 5);
	pio_enable_interrupt(PIN_PUSHBUTTON_3_PIO, PIN_PUSHBUTTON_3_MASK);

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

//This is the second part of lab 5 for a button handler for the isr
void button_handler(uint32_t id, uint32_t mask)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if((PIN_PUSHBUTTON_1_ID == id) && (PIN_PUSHBUTTON_1_MASK == mask))
	{
		// Set a global variable to signify which button (IE onboard in this case)
		currButton = SW0;
	}
	else if((PIN_PUSHBUTTON_2_ID == id) && (PIN_PUSHBUTTON_2_MASK == mask))
	{
		// Set a global variable to signify external switch 1
		currButton = EXT_SW1;
	}
	else if((PIN_PUSHBUTTON_3_ID == id) && (PIN_PUSHBUTTON_3_MASK == mask))
	{
		// Set a global variable to signify external switch 2
		currButton = EXT_SW2;
	}
	// Then wake up the button Task to process.
	vTaskNotifyGiveFromISR(buttonHandle, &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}