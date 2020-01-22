////////////////////////////////////////////////////////////////////////////////
//File: myTasks.c
//Author:	Chad Revel
//This is the main task file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#include <asf.h>
#include <FreeRTOS.h>
#include "task.h"

void myButtonTask(void * pvParamaters);

void myButtonTask(void * pvParamaters)
{
	
	if (taskControl)
	{
		switch (theTask)
		{
			case 0:
			//this will be the first task for ext_sw1 is pressed,
			//then the led1 will flash
				
			case 1:
			//this will be the second task for ext_sw1, is pressed again,
			//then led2 will flash
				
			case 2:
			//this will be the third task for ext_sw1 is pressed a final time,
			//this will make led3 to flash.
			//any further pushes do nothing
				
			case 3:
			//this will be the fourth task for ext_sw2 being pressed,
			//this will have the highest led flashing to stop.
				
			case 4:
			//this will be the fifth task for the ext_sw2 being pressed a second time,
			//this will have the second highest led to stop flashing
				
			case 5:
			//this will be the sixth task for the ext_sw2 being pressed another time,
			//this will have the final led to stop flashing
			//any further pushes do nothing
				
			case 6:
			//this will be for the on board switch.
			//this will freeze all the flashing leds
			//if fewer than 3 leds are flashing, then sw1 will have the opportunity
			//to cause any higher leds to start flashing.
				
			case 7:
			//this will be the second press of sw0.
			//this will unfreeze the frozen leds
			//this will be done by suspending all currently running led tasks
			//on the first button press.
			//the second press will resume all the created led tasks. use vTaskSuspend and vTaskResume.
				
				
				
				
		}
			
	}
		
		

	
	
	
	///////////////////////////////////////////////////////////////////////////////
	//this is from lab 1
	///* Block for 500ms. */
	//const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	//while (true)
	//{
		///* Is button pressed? */
		//if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE)
		//{
//
			///* Yes, so turn LED on. */
			//ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
			//vTaskDelay( xDelay );
			//ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
			//vTaskDelay( xDelay );
			//
		//}
		//else
		//{
			///* No, so turn LED off. */
			//ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		//}
	//}
}
