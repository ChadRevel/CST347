////////////////////////////////////////////////////////////////////////////////
//File: myTasks.c
//Author:	Chad Revel
//This is the task file of the sixth lab for CST 347
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

extern const char* uartSW0;
extern const char* uartEXT_SW1;
extern const char* uartEXT_SW2;
extern const char* myName;

uint8_t currLED = 0;
extern int currButton;

//this is the button task for sw0, ext_sw1, and ext_sw2
void buttonTask(void * pvParamaters)
{
	
	/*
	Button Task – We will be adding a button task to the system defined in part 1.
	The main purpose is to change from polling out buttons to interrupt processing.
	The task must keep track of a global variable for the state of the three buttons.
	It will block on an ulTaskNotifyTake( pdTRUE, portMAX_DELAY ); 
	until the notification is received from the ISR.
	It will then process the button state and do the following actions.
	
	SW0 – Send a message to the TX task as defined below
	“\r\nHello FreeRTOS World\r\n”
	EXT_SW1 – Send a message to the TX task as defined below
	“\r\nCST 347 – RTOS\r\n”
	EXT_SW2 – Send a message to the TX task as defined below

	“\r\nLab 05 – Interrupts in FreeRTOS\r\n”
	*/
	struct rxStruct * controlParams = (struct rxStruct *) pvParamaters;
	QueueHandle_t myTXQ = controlParams->theTXQ;
		
	while (true)
	{	
		if (readButton(SW1) == 1)
		{
			if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			else
			{
				//xQueueSendToBack(myTXQ, &stringBuffer, 0);
				SW_Debounce = 0;
				xQueueSendToBack(myTXQ, uartEXT_SW1, (TickType_t) 0);
			}
		}
		
		else if (readButton(SW2) == 1)
		{
			if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			else
			{
				SW_Debounce = 0;
				xQueueSendToBack(myTXQ, uartEXT_SW2, (TickType_t) 0);
			}
		}
		
		//SW0
		else if (readButton(SW0) == 1)
		{			
			if (SW_Debounce < maxSWDebounce) SW_Debounce++;
			else
			{
				SW_Debounce = 0;
				xQueueSendToBack(myTXQ, uartSW0, (TickType_t) 0);
			}
		}
		
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
	TX Task – The TX task will block awaiting a TXQueue message. 
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
	RX Task – The RX task will be blocking waiting for a queue message from the UART ISR.
	When received it will process the message as defined below. After processing it will 
	then block waiting for the next character. The RX task will be created with a
	priority of 4.
	
	First it will convert the character to a string and send it to the TX task for echo.
	Next it will process the character to see if it is a control character
	The control characters are defined as
	'1’ – Toggle LED 1 and echo
	‘2’ – Toggle LED 2 and echo
	‘3’ – Toggle LED 3 and echo
	‘u’ – Display your name on its own line and echo of character
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