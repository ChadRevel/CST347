////////////////////////////////////////////////////////////////////////////////
//File: myTasks.c
//Author:	Chad Revel
//This is the task file of the third lab for CST 347
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
                                 
/*
EXT_SW1
This Switch will decrease the delay time of the corresponding LED task. 
It will accomplish this by sending a message to this task via the queue setup for this 
pair using the API function vQueueSendToBack() function.  
The message will tell the LED task to decrease its delay time.

EXT_SW2
This Switch will increase the delay time of the corresponding LED task. 
It will accomplish this by sending a message to this task via the queue setup for this pair. 
It will accomplish this by calling vQueueSendToBack() function.

SW0
This Switch will cause the next LED to be selected. 
It will accomplish this by resuming the next MainControl (vTaskResume) Task and then suspending (vTaskSuspend) itself. 
So if LED1 is the current MainControl Task then after the SUSPEND/RESUME LED2 will be the active MainControl Task.
During this operation a message will also be sent to the UART task to identify the new active LED. 
This message will be in the form of a char * and contain a message stating “LED N IS NOW ACTIVE”. 
Make sure that it includes the appropriate Line feeds to move cursor to the next line.

The polling of the switches will happen in sequence and actions will be taken accordingly. 
After all three switches have been polled, the task will perform a vTaskDelay() for 100ms. 
Error checking must be implemented to assure that the Message was sent to the appropriate queue with the vQueueSendtoBack().
*/



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
/*
The heartbeat task will be responsible for toggling the onboard LED every second.
This will give you a visual clue that the FreeRTOS system is still running.
*/	
	
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

void taskLED(void * pvParameters)
{
/*
The LED tasks will blink the corresponding LED on and off. 
It will do this at an initial rate of 500ms. 
It will also check its Queue to see if it has a message waiting (uxQueueMessagesWaiting). 
If there is a message waiting it should read the message (xQueueReceive) and perform the necessary action. 
The wait time to the xQueueReceive() should be set to 0. 
The action will be either to increase or decrease the delay time in increments of 50ms. 
This should be bounded to a MAX delay of 1000ms and a MIN delay of 200ms. 
There will be three of these tasks as well. 
The task should use your LED Driver from Lab 2.
*/	
	
}

QueueHandle_t queueCreation(void)
{
	/*
	You will create 4 queues for this application.  
	The queues should have a depth of 5.  
	For the LED queue's you should create a custom enum message.  
	The UART queue should be set to a size of char [50].  
	You will figure out a method to get the Queue handles to the appropriate tasks.
	*/
	
	
	
}

void mainControlTask(void)
{
	
	
}