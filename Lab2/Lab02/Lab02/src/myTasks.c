////////////////////////////////////////////////////////////////////////////////
//File: myTasks.c
//Author:	Chad Revel
//This is the main task file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#include <asf.h>
#include <FreeRTOS.h>
#include "myTasks.h"

//global variables for the tasks
int theTask = 0;
const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
const TickType_t xDelay2 = 1000 / portTICK_PERIOD_MS;
int ledNum;

void taskSystemControl(void * pvParamaters)
{
	
	//make TaskHandle_t LED0Handle = NULL; for the leds with the numbers
	//gonna need 2 delay variables. 1 with 500ms, and 1 with 1000ms. Still need the while loop for the toggle.
	//Block for 500ms being xDelay, 1000ms for xDelay2
	//xDelay2 is for the taskHeartbeat, so LED0 will flash every second

		
	while (true)
	{

			
		//The next button tasks will be when the buttons are pushed

		if ()
		{
			switch (theTask)
			{
				case 0:
				//this will be the first task for ext_sw1 is pressed,
				//then the led1 will flash
				if readButton(EXT_SW1, 1) // use own functions for the case statements
				{
					// Create a Task to Handle Button Press and Light LED (for led 1, sw 1)
					xTaskCreate(taskSystemControl, "My Button Task", configMINIMAL_STACK_SIZE, 1, 1, 1);
					vTaskDelay(xDelay2);
					theTask = 1;                                   
				}
				else if (ioport_get_pin_level(EXT_SW2, 1))
				{
					vTaskSuspend(taskSystemControl, "My Button Task", configMINIMAL_STACK_SIZE, 1, 1, 1);
					theTask = 0;
				}
				else
					vTaskSuspend(taskSystemControl, "My Button Task", configMINIMAL_STACK_SIZE, 1, 1, 1)
				break;
				case 1:
				//this will be the second task for ext_sw1, is pressed again,
				//then led2 will flash
				if (ioport_get_pin_level(EXT_SW1, 1))
				{
					xTaskCreate(taskSystemControl, "My Button Task", configMINIMAL_STACK_SIZE, 2, 1, 2);
					vTaskDelay(xDelay2);
					theTask = 2;
					
				}
				else if (ioport_get_pin_level(EXT_SW2, 1))
				{
					vTaskSuspend(taskSystemControl, "My Button Task", configMINIMAL_STACK_SIZE, 1, 1, 1);
					theTask = 1;					
					
				}
				break;
			
				case 2:
				//this will be the third task for ext_sw1 is pressed a final time,
				//this will make led3 to flash.
				//any further pushes do nothing
				break;
			
				case 3:
				//this will be the fourth task for ext_sw2 being pressed,
				//this will have the highest led flashing to stop.
				break;
			
				case 4:
				//this will be the fifth task for the ext_sw2 being pressed a second time,
				//this will have the second highest led to stop flashing
				break;
			
				case 5:
				//this will be the sixth task for the ext_sw2 being pressed another time,
				//this will have the final led to stop flashing
				//any further pushes do nothing
				break;
			
				case 6:
				//this will be for the on board switch.
				//this will freeze all the flashing leds
				//if fewer than 3 leds are flashing, then sw1 will have the opportunity
				//to cause any higher leds to start flashing.
				break;
			
				case 7:
				//this will be the second press of sw0.
				//this will unfreeze the frozen leds
				//this will be done by suspending all currently running led tasks
				//on the first button press.
				//the second press will resume all the created led tasks. use vTaskSuspend and vTaskResume.
				break;
				
				default:
				break;
			}
			
			
			
		}
	}

}
		
void taskHeartBeat (void * pvParamaters)		
{
	(uint32_t  ledNum = (uint32_t) pvParamaters);
	//if, else or case statement for the task
		 
			//This will be the heartbeat of LED0 to show that the board is still working
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
			vTaskDelay( xDelay2 );
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
			vTaskDelay( xDelay2 );
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
			//}
			
