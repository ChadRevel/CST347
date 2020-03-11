////////////////////////////////////////////////////////////////////////////////
//File:	main.c
//Author:	Chad Revel
//This is the main file of the fourth lab for CST 347
////////////////////////////////////////////////////////////////////////////////

//system includes
#include <asf.h>

// FreeRTOS Includes
#include <FreeRTOS.h>
#include "queue.h"
#include "ledDriver.h"
#include "myButtons.h"
#include "myTasks.h"
#include "myDefines.h"
#include "Uartdrv.h"


// My Includes

// Defines
#if( BOARD == SAM4E_XPLAINED_PRO )       // Used to place the heap
#define mainHEAP_START            _estack
#define mainRAM_LENGTH            0x00020000    /* 128 KB of internal SRAM. */
#define mainRAM_START             0x20000000    /* at 0x20000000 */
/* The SAM4E Xplained board has 2 banks of external SRAM, each one 512KB. */
#define mainEXTERNAL_SRAM_SIZE           ( 512ul * 1024ul )
#else
#error Define memory regions here.
#endif /* SAM4E_XPLAINED_PRO */


// Function Prototypes
static void prvMiscInitialisation( void );
static void prvInitialiseHeap( void );
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );

// Used to place the heap
extern char _estack;

	
const char* uartBuffer1D = "queue LED1 decrease\r\n";
const char* uartBuffer2D = "queue LED2 decrease\r\n";
const char* uartBuffer3D = "queue LED3 decrease\r\n";

const char* uartBuffer1I = "queue LED1 increase\r\n";
const char* uartBuffer2I = "queue LED2 increase\r\n";
const char* uartBuffer3I = "queue LED3 increase\r\n";

const char* uartBuffer1A = "LED 1 IS NOW ACTIVE\r\n";
const char* uartBuffer2A = "LED 2 IS NOW ACTIVE\r\n";
const char* uartBuffer3A = "LED 3 IS NOW ACTIVE\r\n";

//these are the lab 4 char *'s
const char* uartBufferMainStart = "Main Control is now starting.\r\n";
const char* uartBufferMainControl = "Main Control is running.\r\n";
const char* uartBufferMainBlock = "Main Control is now blocking.\r\n";

const char* uartBuffer1Start = "LED 1 IS NOW STARTING\r\n";
const char* uartBuffer2Start = "LED 2 IS NOW STARTING\r\n";
const char* uartBuffer3Start = "LED 3 IS NOW STARTING\r\n";

const char* uartBuffer1Block = "LED 1 IS NOW BLOCKING\r\n";
const char* uartBuffer2Block = "LED 2 IS NOW BLOCKING\r\n";
const char* uartBuffer3Block = "LED 3 IS NOW BLOCKING\r\n";



//////////////////////////////////////////////////
//lab 3 part

//create the 3 queue handles for controlling the queues
QueueHandle_t ledQ[3] = {NULL, NULL, NULL};
QueueHandle_t uartQ = NULL;


//create the handles for the controlling the tasks
TaskHandle_t ledHandle[3] = {NULL, NULL, NULL};
//TaskHandle_t controlHandle[3] = {NULL, NULL, NULL};
TaskHandle_t uartHandle = NULL; 



//make the structs within the structs	
struct controlStruct controlLED1;
struct controlStruct controlLED2;
struct controlStruct controlLED3;
	
struct ledStruct LED1Struct;
struct ledStruct LED2Struct;
struct ledStruct LED3Struct;
	
	
//lab 4 structs within a struct
struct mainControlStruct 
{
	//create structs to be able to pass in the values around and not have everything set as a global variable
	controlLED1.ledQ = ledQ[0];
	controlLED1.uartQ = uartQ;
	controlLED1.ledHandle = ledHandle[0];
	controlLED1.nextTask = &controlHandle[1];
	controlLED1.ledNum = LED1;
	controlLED2.ledQ = ledQ[1];
	controlLED2.uartQ = uartQ;
	controlLED2.ledHandle = ledHandle[1];
	controlLED2.nextTask = &controlHandle[2];
	controlLED2.ledNum = LED2;
	controlLED3.ledQ = ledQ[2];
	controlLED3.uartQ = uartQ;
	controlLED3.ledHandle = ledHandle[2];
	controlLED3.nextTask = &controlHandle[0];
	controlLED3.ledNum = LED3;
		
	LED1Struct.ledQ = ledQ[0];
	LED1Struct.uartQ = uartQ;
	LED1Struct.ledNum = LED1;
	LED2Struct.ledQ = ledQ[1];
	LED2Struct.uartQ = uartQ;
	LED2Struct.ledNum = LED2;
	LED3Struct.ledQ = ledQ[2];
	LED3Struct.uartQ = uartQ;
	LED3Struct.ledNum = LED3;

};	

struct mainControlHandle
{
	TaskHandle_t controlHandle[3] = {NULL, NULL, NULL};
};

int main (void)
{
	// Initialize The Board
	prvMiscInitialisation();
	intitializeLEDDriver();
	initializeButtonDriver();
	initUART(EDBG_UART);
	
	const char* startText = "This is Lab3\r\n";
	UARTPutStr(EDBG_UART, startText, 0);
	
	
	/*
	The MainControl task will poll the status of all three switches (just as we did in lab 2). 
	The switches should be debounced with a three stage strategy: 
	Read the switch, Delay 10ms using a vTaskDelay, then read switch again to verify the change. 
	In addition you should implement a lockout feature in your driver only allowing a switch to perform a single action on a press. 
	There will be three of these tasks (only 1 active at a time). 
	Each of these task will send messages via a queue to its corresponding LED task. 
	The switches will perform the following function.
	*/

	//create the queues for the handles
	ledQ[0] = xQueueCreate(5, sizeof(timeDelay));
	ledQ[1] = xQueueCreate(5, sizeof(timeDelay));
	ledQ[2] = xQueueCreate(5, sizeof(timeDelay));
	uartQ = xQueueCreate(5, sizeof(char[50]));


	////create structs to be able to pass in the values around and not have everything set as a global variable
	//controlLED1.ledQ = ledQ[0];
	//controlLED1.uartQ = uartQ;
	//controlLED1.ledHandle = ledHandle[0];
	//controlLED1.nextTask = &controlHandle[1];
	//controlLED1.ledNum = LED1;
	//controlLED2.ledQ = ledQ[1];
	//controlLED2.uartQ = uartQ;
	//controlLED2.ledHandle = ledHandle[1];
	//controlLED2.nextTask = &controlHandle[2];
	//controlLED2.ledNum = LED2;
	//controlLED3.ledQ = ledQ[2];
	//controlLED3.uartQ = uartQ;
	//controlLED3.ledHandle = ledHandle[2];
	//controlLED3.nextTask = &controlHandle[0];	
	//controlLED3.ledNum = LED3;
	//
	//LED1Struct.ledQ = ledQ[0];
	//LED1Struct.uartQ = uartQ;
	//LED1Struct.ledNum = LED1;
	//LED2Struct.ledQ = ledQ[1];
	//LED2Struct.uartQ = uartQ;
	//LED2Struct.ledNum = LED2;
	//LED3Struct.ledQ = ledQ[2];
	//LED3Struct.uartQ = uartQ;
	//LED3Struct.ledNum = LED3;


	//for lab 4's control settings
	

//need 3 led task creates as well

	//creating a task for the led0 to beat. Need to pass in a parameter of 0, to show that it's for led0
	xTaskCreate(taskHeartBeat, "LED0 Heart Beat", configMINIMAL_STACK_SIZE, (void *) 0, 1, NULL);
	
	xTaskCreate(taskUART, "Main UART Task", configMINIMAL_STACK_SIZE, &uartHandle, 1, NULL);
	
	xTaskCreate(taskLED, "LED 1 Task", configMINIMAL_STACK_SIZE, (void *) &LED1Struct, 1, &ledHandle[0]);
	xTaskCreate(taskLED, "LED 2 Task", configMINIMAL_STACK_SIZE, (void *) &LED2Struct, 1, &ledHandle[1]);
	xTaskCreate(taskLED, "LED 3 Task", configMINIMAL_STACK_SIZE, (void *) &LED3Struct, 1, &ledHandle[2]);
	//create the 3 tasks first, then suspend 2 of them before it actually starts by the start scheduler.

//for lab 4, we'll only need 1 create task and it will all be based off of sw0 to go through each led
	xTaskCreate(taskSystemControl, "Main Control in Main", configMINIMAL_STACK_SIZE, (void *) &mainControlStruct, 1, &mainControlHandle);

	//xTaskCreate(taskSystemControl, "Main Control in Main", configMINIMAL_STACK_SIZE, (void *) &controlLED1, 1, &controlHandle[0]);
	//xTaskCreate(taskSystemControl, "Main Control Task for LED2", configMINIMAL_STACK_SIZE, (void *) &controlLED2, 1, &controlHandle[1]);
	//xTaskCreate(taskSystemControl, "Main Control Task for LED3", configMINIMAL_STACK_SIZE, (void *) &controlLED3, 1, &controlHandle[2]);
	
	//now suspend the latter 2 tasks
	//vTaskSuspend(controlHandle[1]);
	//vTaskSuspend(controlHandle[2]);
	
	// Start The Scheduler
	vTaskStartScheduler();

	while(true) {}
}


static void prvInitialiseHeap( )
{
       uint32_t ulHeapSize;
       uint8_t *pucHeapStart, *pucHeapEnd;

       pucHeapStart = ( uint8_t * ) ( ( ( ( uint32_t ) &mainHEAP_START ) + 7 ) & ~0x07ul );
       pucHeapEnd = ( uint8_t * ) ( mainRAM_START + mainRAM_LENGTH );
       ulHeapSize = ( uint32_t ) ( ( uint32_t ) pucHeapEnd - ( uint32_t ) &mainHEAP_START );
       ulHeapSize &= ~0x07ul;
       ulHeapSize -= 1024;
       HeapRegion_t xHeapRegions[] = {
              { ( unsigned char *) pucHeapStart, ulHeapSize },
              { ( unsigned char *) SRAM_BASE_ADDRESS, mainEXTERNAL_SRAM_SIZE },
              { ( unsigned char *) SRAM_BASE_ADDRESS_2ND, mainEXTERNAL_SRAM_SIZE },
              { NULL, 0 }
       };

       vPortDefineHeapRegions( xHeapRegions );
}
static void prvMiscInitialisation( void )
{
       /* Initialize the SAM system */
       sysclk_init();
       board_init();
       prvInitialiseHeap();
       pmc_enable_periph_clk(ID_PIOA);
       pmc_enable_periph_clk(ID_PIOB);
}

void vAssertCalled( const char *pcFile, uint32_t ulLine )
{
       volatile uint32_t ulBlockVariable = 0UL;

       /* Setting ulBlockVariable to a non-zero value in the debugger will allow
       this function to be exited. */
       taskDISABLE_INTERRUPTS();
       {
              while( ulBlockVariable == 0UL )
              {
                     __asm volatile( "NOP" );
              }
       }
       taskENABLE_INTERRUPTS();
}

void vApplicationMallocFailedHook( void )
{
       /* Called if a call to pvPortMalloc() fails because there is insufficient
       free memory available in the FreeRTOS heap.  pvPortMalloc() is called
       internally by FreeRTOS API functions that create tasks, queues, software
       timers, and semaphores.  The size of the FreeRTOS heap is set by the
       configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
       vAssertCalled( __FILE__, __LINE__ );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
       ( void ) pcTaskName;
       ( void ) pxTask;

       /* Run time stack overflow checking is performed if
       configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
       function is called if a stack overflow is detected. */
       taskDISABLE_INTERRUPTS();

       for( ;; );
}

