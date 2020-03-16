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

extern const char* myName;
uint8_t currLED = 0;
//This is the main control for the task system
//void taskSystemControl(void * pvParamaters)
//{
	//struct rxStruct * controlParams = (struct rxStruct *) pvParamaters;
	//QueueHandle_t ledQueueParam[3];
	//ledQueueParam[0] = controlParams->ledQ[0];
	//ledQueueParam[1] = controlParams->ledQ[1];
	//ledQueueParam[2] = controlParams->ledQ[2];
	//QueueHandle_t uartQueueParam = controlParams->uartQ;
               //
	////declaration for either increasing or decreasing the speed			                     
	//enum timeDelay incDec;
//
	//while (true)
	//{
		//
		////first send out a message saying that the main control is starting
		//xQueueSendToBack(uartQueueParam, uartBufferMainStart, (TickType_t) 0);	
		//
		//
		//if (readButton(SW1) == 1)
		//{
			//if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			//else
			//{
				//SW_Debounce = 0;
				////send led1 back to the end of the queue
				//incDec = DECREASE;
				//xQueueSendToBack(ledQueueParam[currLED-1], (void *) &incDec, (TickType_t) 10);
			//}
		//}
//
		//else if (readButton(SW2) == 1)
		//{
			//if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			//else
			//{
				//SW_Debounce = 0;
				//incDec = INCREASE;
				////send led1 back to the end of the queue
				//xQueueSendToBack(ledQueueParam[currLED-1], (void *) &incDec, (TickType_t) 10);
			//}
		//}
//
		////SW0
		//else if (readButton(SW0) == 1)
		//{
			//if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			//else
			//{
				//SW_Debounce = 0;
				//switch(currLED)
				//{
					//case LED1:
						//currLED = LED2;
						//break;
					//case LED2:
						//currLED = LED3;
						//break;
					//case LED3:
						//currLED = LED1;
						//break;
					//default:
						//break;
				//}
				//
			//}
		//}
		////tell uart that the maincontrol is now blocking
		//xQueueSendToBack(uartQueueParam, uartBufferMainBlock, (TickType_t) 0);
		//
		////delay for 100ms after all 3 switches
		//vTaskDelay(xDelay3);
	//}
//
//}
	
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
	
	QueueHandle_t LEDParam = (QueueHandle_t) pvParameters;


	while(true)
	{
		xQueueReceive(LEDParam, &(currLED), portMAX_DELAY);
		toggleLED(currLED);
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
	struct rxStruct * controlParams = (struct rxStruct *) pvParameters;
	QueueHandle_t theRXQ = controlParams->theRXQ;
	
	char bufferString[50];
	/*
	TX Task � The TX task will block awaiting a TXQueue message. 
	The message will be a string of maximum 50 characters (NULL Terminated) 
	for display to the terminal. The TX task will call the UARTPutStr() function 
	to display the string to the UART. The TX task will be created with a task priority of 3.
	*/
	
	while (true)
	{
		//will first wait until the rx queue is sent into tx for processing
		//receive the rx message for echo, and place it into a buffer
		xQueueReceive(theRXQ, &bufferString, portMAX_DELAY);
		UARTPutStr(EDBG_UART, bufferString, sizeof(bufferString));
	}
	
}

void taskRX(void *pvParameters)
{
		struct rxStruct * controlParams = (struct rxStruct *) pvParameters;
		QueueHandle_t myRXQ = controlParams->theRXQ;
		QueueHandle_t myTXQ = controlParams->theTXQ;
		//QueueHandle_t theLEDQ = controlParams->theLEDQ;
		
		char rxBuffer = 0;
		char stringBuffer[50];

		int n = 0;
	/*
	RX Task � The RX task will be blocking waiting for a queue message from the UART ISR.
	When received it will process the message as defined below. After processing it will 
	then block waiting for the next character. The RX task will be created with a
	priority of 4.
	
	First it will convert the character to a string and send it to the TX task for echo.
	Next it will process the character to see if it is a control character
	The control characters are defined as
	'1� � Toggle LED 1 and echo
	�2� � Toggle LED 2 and echo
	�3� � Toggle LED 3 and echo
	�u� � Display your name on its own line and echo of character
	All other characters have no effect (echo only)
	*/

	while (true)
	{
		xQueueReceive(myRXQ, &rxBuffer, portMAX_DELAY);
		stringBuffer[0] = rxBuffer;
		
		//until we hit the end of the buffer, move through each char
		if (stringBuffer != NULL)
		{
			stringBuffer[n] = (char) rxBuffer;
			//if the char is 1, then toggle led1 and send echo to tx task
			if (stringBuffer[n] == '1')
			{
				toggleLED(LED1);
				xQueueSendToBack(myTXQ, &stringBuffer, 0);
				
			}
			//else if the char is 2, then toggle led2 and send echo to tx task
			else if (stringBuffer[n] == '2')
			{
				toggleLED(LED1);
				xQueueSendToBack(myTXQ, &stringBuffer, 0);
				
			}
			//else if the char is 3, then toggle led3 and send echo to tx task
			else if (stringBuffer[n] == '3')
			{
				toggleLED(LED1);
				xQueueSendToBack(myTXQ, &stringBuffer, 0);
							
			}
			//else if the char is u, then display "Chad" and send echo of char to tx task
			else if (stringBuffer[n] == 'u')
			{
				xQueueSendToBack(myTXQ, &stringBuffer, 0);
				UARTPutStr(EDBG_UART, myName, 0);
							
			}
			//else only echo to tx task and nothing else
			else
			{
				xQueueSendToBack(myTXQ, &stringBuffer, 0);
			}
			//step further into the string
			n++;
			
		}
		
		
	}
	
}