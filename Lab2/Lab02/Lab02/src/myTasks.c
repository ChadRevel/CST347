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

//the task handles for each of the leds
TaskHandle_t LEDHandle0 = NULL;
TaskHandle_t LEDHandle1 = NULL;
TaskHandle_t LEDHandle2 = NULL;
TaskHandle_t LEDHandle3 = NULL;


void taskSystemControl(void * pvParamaters)
{
//creating a task for the led0 to beat. Need to pass in a parameter of 0, to show that it's for led0
xTaskCreate(taskHeartBeat, "LED0 Heart Beat", configMINIMAL_STACK_SIZE, (void *) 0, 1, &LEDHandle0);
                                 


	while (true)
	{
		//the first case will be for sw1
		//it will create the task for each of the leds.
		if (readButton(SW1) == 1)
		{
			if (LEDHandle1 == NULL)
			{
				//create a task for led to start beating
				xTaskCreate(taskHeartBeat, "LED1 Heart Beat", configMINIMAL_STACK_SIZE, (void *) 1, 1, &LEDHandle1);
			}
			else if (LEDHandle2 == NULL)
			{
				//create a task for led to start beating
				xTaskCreate(taskHeartBeat, "LED2 Heart Beat", configMINIMAL_STACK_SIZE, (void *) 2, 1, &LEDHandle2);
			}
			else if (LEDHandle3 == NULL)
			{
				//create a task for led to start beating
				xTaskCreate(taskHeartBeat, "LED3 Heart Beat", configMINIMAL_STACK_SIZE, (void *) 3, 1, &LEDHandle3);					
			}
				
		}

		//if sw 2 is pressed, then check each led from the highest led to the lowest, then delete the task
		else if (readButton(SW2) == 1)
		{
			if (LEDHandle3 != NULL)
			{
				vTaskDelete(LEDHandle3);
			}
			else if (LEDHandle2 != NULL)
			{
				vTaskDelete(LEDHandle2);
			}
			else if (LEDHandle1 != NULL)
			{
				vTaskDelete(LEDHandle1);
			}

		}

		//For the on board switch, if it's pressed, then suspend, or release the leds
		else if (readButton(SW0) == 1)
		{
			if (LEDHandle1 != NULL)
			{
				vTaskSuspend(LEDHandle1);
			}
			else if (LEDHandle2 != NULL)
			{
				vTaskSuspend(LEDHandle2);
			}
			else if (LEDHandle3 != NULL)
			{
				vTaskSuspend(LEDHandle3);
			}
			else if (LEDHandle1 == NULL)
			{
				vTaskResume(LEDHandle1);
			}
			else if (LEDHandle2 == NULL)
			{
				vTaskResume(LEDHandle2);
			}
			else if (LEDHandle3 == NULL)
			{
				vTaskResume(LEDHandle3);
			}		
		
		}
		//delay for 100ms after all 3 switches
		vTaskDelay(xDelay3);
	}

}
		
void taskHeartBeat (void * pvParamaters)		
{
	//set up the LedNumber for the parameters
	uint32_t ledNum = (uint32_t) pvParamaters;
	//this is the heartbeat for LED 0 to happen once every second
	while (true)
	{
		//toggle the 4 leds to their respective delays
		switch(ledNum)
		{
			case 0:
				toggleLED(LED0);
			break;
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
