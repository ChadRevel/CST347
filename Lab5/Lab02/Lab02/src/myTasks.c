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

		//SW0
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
	struct ledStruct * controlParams = (struct ledStruct *) pvParameters;
	QueueHandle_t ledQ = controlParams->ledQ;
	QueueHandle_t uartQ = controlParams->uartQ;
	uint8_t ledNum = controlParams->ledNum;
	int defaultMS = 500;
	TickType_t xDelay = defaultMS / portTICK_PERIOD_MS;

	timeDelay getDelay;

	while(true)
	{

		switch(ledNum)
		{
			case LED1:
				toggleLED(ledNum);
				xQueueSendToBack(uartQ, uartBuffer1Block, 0);
				break;
			case LED2:
				toggleLED(ledNum);
				xQueueSendToBack(uartQ, uartBuffer2Block, 0);
				break;
			case LED3:
				toggleLED(ledNum);
				xQueueSendToBack(uartQ, uartBuffer3Block, 0);
				break;
			default:
				break;
		}
		vTaskDelay(xDelay);
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

void taskTX(void *pvParameters)
{
	/*
	TX Task – The TX task will block awaiting a TXQueue message. 
	The message will be a string of maximum 50 characters (NULL Terminated) 
	for display to the terminal. The TX task will call the UARTPutStr() function 
	to display the string to the UART. The TX task will be created with a task priority of 3.
	*/
	
	
}

void taskRX(void *pvParameters)
{
	/*
	RX Task – The RX task will be blocking waiting for a queue message from the UART ISR.
	When received it will process the message as defined below. After processing it will 
	then block waiting for the next character. The RX task will be created with a priority of 4.
	
	First it will convert the character to a string and send it to the TX task for echo.
	Next it will process the character to see if it is a control character
	The control characters are defined as
	'1’ – Toggle LED 1 and echo
	‘2’ – Toggle LED 2 and echo
	‘3’ – Toggle LED 3 and echo
	‘u’ – Display your name on its own line and echo of character
	All other characters have no effect (echo only)
	*/
	
	
}