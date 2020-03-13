////////////////////////////////////////////////////////////////////////////////
//File: myTasks.c
//Author:	Chad Revel
//This is the task file of the fifth lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#include <asf.h>
#include <FreeRTOS.h>
#include "myTasks.h"


//gonna need 2 delay variables. 1 with 500ms, and 1 with 1000ms. Still need the while loop for the toggle.
//Block for 500ms being xDelay, 1000ms for xDelay2
//xDelay2 is for the taskHeartbeat, so LED0 will flash every second

const TickType_t xDelay2 = 1000 / portTICK_PERIOD_MS;
const TickType_t xDelay3 = 100 / portTICK_PERIOD_MS;

int SW_Debounce = 0;

extern const char* uartBuffer1D;
extern const char* uartBuffer2D;
extern const char* uartBuffer3D;

extern const char* uartBuffer1I;
extern const char* uartBuffer2I;
extern const char* uartBuffer3I; 

extern const char* uartBuffer1A;
extern const char* uartBuffer2A;
extern const char* uartBuffer3A;

extern const char* uartBufferMainStart;
extern const char* uartBufferMainBlock;

extern const char* uartBuffer1Start;
extern const char* uartBuffer2Start;
extern const char* uartBuffer3Start;

extern const char* uartBuffer1Block;
extern const char* uartBuffer2Block;
extern const char* uartBuffer3Block;

uint8_t currLED = LED1;

//This is the main control for the task system
void taskSystemControl(void * pvParamaters)
{
	//only suppose to be 1 MainControl task here for lab 4.
	/*
	“MainControl” Task – Poll button switches and perform the following actions. As with lab 3,
	the button switches need to be debounced and a lockout scheme should be implemented.
	All the hardware considerations from the previous labs apply. 
	*/
	
	/*
	OTHER MODIFICATIONS AND CONSIDERATIONS – “MainControl” will no longer send a message to the UART
	task announcing which LED is active based on SW0. Instead, it will send a message only to announce
	that it (“MainControl” task) is running. To do this, a vQueueSendToBack() message is sent to the UART queue
	at the top of the while(1) loop to announce “MainControl Starting” and right before it does the end-of-loop vTaskDelay()
	with “MainControl Blocking”.
	*/
	
	
	struct controlStruct * controlParams = (struct controlStruct *) pvParamaters;
	QueueHandle_t ledQueueParam[3];
	ledQueueParam[0] = controlParams->ledQ[0];
	ledQueueParam[1] = controlParams->ledQ[1];
	ledQueueParam[2] = controlParams->ledQ[2];
	QueueHandle_t uartQueueParam = controlParams->uartQ;
               
	//declaration for either increasing or decreasing the speed			                     
	enum timeDelay incDec;

	while (true)
	{
		
		//first send out a message saying that the main control is starting
		xQueueSendToBack(uartQueueParam, uartBufferMainStart, (TickType_t) 0);	
		
		
		//the first case will be for sw1
		//it will create the task for each of the leds.
		/*
		EXT_SW1
		This Switch will decrease the delay time of the corresponding LED task.
		It will accomplish this by sending a message to this task via the queue setup for this
		pair using the API function xQueueSendToBack() function.
		The message will tell the LED task to decrease its delay time.
		*/
		if (readButton(SW1) == 1)
		{
			if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			else
			{
				SW_Debounce = 0;
				//send led1 back to the end of the queue
				incDec = DECREASE;
				xQueueSendToBack(ledQueueParam[currLED-1], (void *) &incDec, (TickType_t) 10);
			}
		}

		//if sw 2 is pressed, then check each led from the highest led to the lowest, then delete the task
		/*
		EXT_SW2
		This Switch will increase the delay time of the corresponding LED task.
		It will accomplish this by sending a message to this task via the queue setup for this pair.
		It will accomplish this by calling xQueueSendToBack() function.
		*/
		else if (readButton(SW2) == 1)
		{
			if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			else
			{
				SW_Debounce = 0;
				incDec = INCREASE;
				//send led1 back to the end of the queue
				xQueueSendToBack(ledQueueParam[currLED-1], (void *) &incDec, (TickType_t) 10);
			}
		}

		//For the on board switch, if it's pressed, then suspend, or release the leds
		/*
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
		

		else if (readButton(SW0) == 1)
		{
			if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			else
			{
				SW_Debounce = 0;
				switch(currLED)
				{
					case LED1:
						currLED = LED2;
						break;
					case LED2:
						currLED = LED3;
						break;
					case LED3:
						currLED = LED1;
						break;
					default:
						break;
				}
				
			}
		}
		//tell uart that the maincontrol is now blocking
		xQueueSendToBack(uartQueueParam, uartBufferMainBlock, (TickType_t) 0);
		
		//delay for 100ms after all 3 switches
		vTaskDelay(xDelay3);
	}

}
	
//this is the heartbeat task to have led 0 blink		
void taskHeartBeat (void * pvParamaters)		
{
/*
The heartbeat task will be responsible for toggling the on-board LED every second.
This will give you a visual clue that the FreeRTOS system is still running.
*/	

	//this is the heartbeat for LED 0 to happen once every second
	while (true)
	{
		toggleLED(LED0);
		vTaskDelay(xDelay2);

	}
		
}


void taskLED(void * pvParameters)
{

//from lab 4
/*
LED Task – The LED task will function in the same way as it did in Lab 3.
This means the task will use uxQueueMessagesWaiting() and xQueueReceive() to receive timing change messages.
The one change here is that the LED task will now announce that it is active versus “MainControl” task as in lab 3. As in “MainControl”,
at the top of the while(1) and right before the vTaskDelay(), LED task will call vQueueSendtoBack() to announce “LED N Starting” and “LED N Blocking”.
*/

	struct ledStruct * controlParams = (struct ledStruct *) pvParameters;
	QueueHandle_t ledQ = controlParams->ledQ;
	QueueHandle_t uartQ = controlParams->uartQ;
	uint8_t ledNum = controlParams->ledNum;
	int defaultMS = 500;
	TickType_t xDelay = defaultMS / portTICK_PERIOD_MS;

	timeDelay getDelay;

	while(true)
	{
		//the switch statement to send message back that the led N has started depending on the led number.
		switch(ledNum)
		{
			case LED1:
				xQueueSendToBack(uartQ, uartBuffer1Start, 0);
				break;
			case LED2:
				xQueueSendToBack(uartQ, uartBuffer2Start, 0);
				break;
			case LED3:
				xQueueSendToBack(uartQ, uartBuffer3Start, 0);
				break;
			default:
				break;	
		}
		toggleLED(ledNum);

		
		//this will go through the queue for each of the leds and either increase or decrease the delay

		if(uxQueueMessagesWaiting(ledQ))
		{
			//if the queue receive of queue handle[i] goes into &getDelay and it's == to pbTrue, increase or decrease
			if (xQueueReceive(ledQ, &getDelay, 0))
			{

				if(getDelay == DECREASE)
				{
					if (ledNum == LED1)
					{
						xQueueSendToBack(uartQ, uartBuffer1D, (TickType_t) 0);
					}
					else if (ledNum == LED2)
					{
						xQueueSendToBack(uartQ, uartBuffer2D, (TickType_t) 0);
					}
					else if (ledNum == LED3)
					{
						xQueueSendToBack(uartQ, uartBuffer3D, (TickType_t) 0);
					}
					xDelay = (defaultMS - 50) / portTICK_PERIOD_MS;
					if(xDelay < 200)
					{
						xDelay = 200;
					}
				}

				else if (getDelay == INCREASE)
				{
					if (ledNum == LED1)
					{
						xQueueSendToBack(uartQ, uartBuffer1I, (TickType_t) 0);
					}
					else if (ledNum == LED2)
					{
						xQueueSendToBack(uartQ, uartBuffer2I, (TickType_t) 0);
					}
					else if (ledNum == LED3)
					{
						xQueueSendToBack(uartQ, uartBuffer3I, (TickType_t) 0);
					}
					xDelay = (defaultMS + 50) / portTICK_PERIOD_MS;
					if(xDelay == 1000)
					{
						xDelay = 1000;
					}
				}
			}
		}
		
		switch(ledNum)
		{
			case LED1:
				xQueueSendToBack(uartQ, uartBuffer1Block, 0);
				break;
			case LED2:
				xQueueSendToBack(uartQ, uartBuffer2Block, 0);
				break;
			case LED3:
				xQueueSendToBack(uartQ, uartBuffer3Block, 0);
				break;
			default:
				break;
		}
		vTaskDelay(xDelay);
	}
}

void modifiedLEDTask(void *pvParameters)
{
	struct ledStruct * controlParams = (struct ledStruct *) pvParameters;
	QueueHandle_t ledQ = controlParams->ledQ;
	QueueHandle_t uartQ = controlParams->uartQ;
	uint8_t ledNum = controlParams->ledNum;
	int defaultMS = 500;
	TickType_t xDelay = defaultMS / portTICK_PERIOD_MS;
	
	portTickType xStartTime;

    while (1)

    {
        /* Note the time before entering the while loop.  xTaskGetTickCount()
        is a FreeRTOS API function. */

        xStartTime = xTaskGetTickCount();

        /* Loop until pxTaskParameters->xToggleRate ticks have */

        while ((xTaskGetTickCount() - xStartTime) < 200);


		 /* Toggle the LED */
		 toggleLED(ledNum);

	}
	
	
}

void taskUART(void *pvParameters)
{
/*
The UART task will block until a message is ready for it. To accomplish this blocking
set the last parameter of the xQueueReceive() to portMAX_DELAY. It will then get the message
and write it out on the UART using your UARTPutStr(). You will need to write a very rudimentary
UART driver for your program. The filenames for the driver will be uartdrv.c and uartdrv.h.
This should include three functions. The functions should be initUART(), UARTPutC() and UARTPutStr().
The initUART() function will initialize the UART. The UARTPutC() will print a single byte (char) to the UART.
The UARTPutStr() function will use the UARTPutC() to write a complete string to the UART.
*/	

QueueHandle_t uartTempQueue = (QueueHandle_t) pvParameters;

char tempUART[50];

	while(true)
	{
		if(uxQueueMessagesWaiting(uartTempQueue))
		{
			xQueueReceive(uartTempQueue, &tempUART, portMAX_DELAY);
			UARTPutStr(EDBG_UART, tempUART, sizeof(tempUART));
		}
	}
}
