////////////////////////////////////////////////////////////////////////////////
//File: myTasks.c
//Author:	Chad Revel
//This is the task file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#include <asf.h>
#include <FreeRTOS.h>
#include "myTasks.h"

//gonna need 2 delay variables. 1 with 500ms, and 1 with 1000ms. Still need the while loop for the toggle.
//Block for 500ms being xDelay, 1000ms for xDelay2
//xDelay2 is for the taskHeartbeat, so LED0 will flash every second
const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
const TickType_t xDelay2 = 1000 / portTICK_PERIOD_MS;
const TickType_t xDelay3 = 100 / portTICK_PERIOD_MS;

extern TaskHandle_t LEDHandle[3];


void taskSystemControl(void * pvParamaters)
{
    //set up the parameters
	struct ledStruct * controlParams = (struct ledStruct *) pvParamaters;
	QueueHandle_t ledQ = controlParams->ledQ;
//	uint8_t ledNum = controlParams->ledNum;
    uint32_t ledNum = (uint32_t) pvParamaters;                        


	while (true)
	{
		//the first case will be for sw1
		//it will create the task for each of the leds.
		if (readButton(SW1) == 1)
		{
			if (ledQ = LED1)
			{
				//create a task for led 1 to turn on
				toggleLED(LED1);
				
			}
			else if (ledQ = LED2)
			{
				//create a task for led 2 to turn on
				toggleLED(LED2);
			}
			else if (ledQ = LED3)
			{
				//create a task for led 3 to turn on
				toggleLED(LED3);
			}
				
		}

		//if sw 2 is pressed, then check each led from the highest led to the lowest, then delete the task
		else if (readButton(SW2) == 1)
		{
			if (ledQ = LED1)
			{
				vTaskDelete(LEDHandle[3]);
			}
			else if (ledQ = LED2)
			{
				vTaskDelete(LEDHandle[2]);
			}
			else if (ledQ = LED3)
			{
				vTaskDelete(LEDHandle[1]);
			}

		}

		//For the on board switch, if it's pressed, then suspend, or release the leds
		else if (readButton(SW0) == 1)
		{
			//if (ledQ = LED1)
			//{
				//vTaskSuspend(LEDHandle[1]);
			//}
			//else if (ledQ = LED2)
			//{
				//vTaskSuspend(LEDHandle[2]);
			//}
			//else if (ledQ = LED3)
			//{
				//vTaskSuspend(LEDHandle[3]);
			//}
			//else if (ledQ = LED1)
			//{
				//vTaskResume(LEDHandle[1]);
			//}
			//else if (LEDHandle[2])
			//{
				//vTaskResume(LEDHandle[2]);
			//}
			//else if (LEDHandle[3])
			//{
				//vTaskResume(LEDHandle[3]);
			//}		
		
		}
		//delay for 100ms after all 3 switches
		vTaskDelay(xDelay3);
	}
	

		//this is the heartbeat for LED 0 to happen once every second
		while (true)
		{
			//toggle the 3 leds to their respective delays
			switch(ledNum)
			{
				case 1:
				toggleLED(LED1);
				break;
				case 2:
				toggleLED(LED2);
				break;
				case 3:
				toggleLED(LED3);
				break;
				
				default:
				break;
			}
			//set the delay for the specific leds
			if (ledNum == 0)
			{
				vTaskDelay(xDelay2);
			}
			else
			{
				vTaskDelay(xDelay);
			}
		}

}
		
void taskHeartBeat (void * pvParamaters)		
{
	/*
	The heartbeat task will be responsible for toggling the onboard LED every second.
	This will give you a visual clue that the FreeRTOS system is still running.
	*/

	//this is the heartbeat for LED 0 to happen once every second
	while (true)
	{
		toggleLED(LED0);
		vTaskDelay(xDelay2);
	}
		
}
