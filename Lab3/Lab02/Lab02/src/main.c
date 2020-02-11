////////////////////////////////////////////////////////////////////////////////
//File:	main.c
//Author:	Chad Revel
//This is the main file of the third lab for CST 347
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

char uartBuffer [50];



//////////////////////////////////////////////////
//lab 3 part

//create the 3 queue handles for controlling the queues
QueueHandle_t ledQ[3] = {NULL, NULL, NULL};
QueueHandle_t uartQ = NULL;


//create the handles for the controlling the tasks
TaskHandle_t ledHandle[3] = {NULL, NULL, NULL};
TaskHandle_t uartHandle = NULL; 
TaskHandle_t nextTask = NULL;



	
struct controlStruct controlLED1;
struct controlStruct controlLED2;
struct controlStruct controlLED3;
	
struct ledStruct LED1Struct;
struct ledStruct LED2Struct;
struct ledStruct LED3Struct;
	

int main (void)
{
	// Initialize The Board
	prvMiscInitialisation();
	intitializeLEDDriver();
	initializeButtonDriver();
	initUART(EDBG_UART);
	
	const char* startText = "/r/n/r/n/r/nThis is Lab3/r/n/r/n/r/n";
	UARTPutStr(EDBG_UART, startText, sizeof(startText));
	
	
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
	ledQ[1] = xQueueCreate(5, sizeof(currLED));
	ledQ[2] = xQueueCreate(5, sizeof(currLED));
	ledQ[3] = xQueueCreate(5, sizeof(currLED));
	uartQ = xQueueCreate(5, sizeof(char[50]));


	//create structs to be able to pass in the values around and not have everything set as a global variable
	controlLED1.ledQ = ledQ[1];
	controlLED1.uartQ = uartQ;
	controlLED1.ledHandle = ledHandle[1];
	controlLED1.nextTask = nextTask;
	controlLED1.ledNum = LED1;
	controlLED2.ledQ = ledQ[2];
	controlLED2.uartQ = uartQ;
	controlLED2.ledHandle = ledHandle[2];
	controlLED2.nextTask = nextTask;
	controlLED2.ledNum = LED2;
	controlLED3.ledQ = ledQ[3];
	controlLED3.uartQ = uartQ;
	controlLED3.ledHandle = ledHandle[3];
	controlLED3.nextTask = nextTask;	
	controlLED3.ledNum = LED3;
	
	//controlLED1 = {&ledQ[1], &uartQ, &ledHandle[1], nextTask, LED1};
	//controlLED2 = {&ledQ[2], &uartQ, &ledHandle[2], nextTask, LED2};
	//controlLED3 = {&ledQ[3], &uartQ, &ledHandle[3], nextTask, LED3};
	LED1Struct.ledQ = ledQ[1];
	LED1Struct.uartQ = uartQ;
	LED1Struct.ledNum = LED1;
	LED2Struct.ledQ = ledQ[2];
	LED2Struct.uartQ = uartQ;
	LED2Struct.ledNum = LED2;
	LED3Struct.ledQ = ledQ[3];
	LED3Struct.uartQ = uartQ;
	LED3Struct.ledNum = LED3;
	//LED1Struct = {&ledQ[1], &uartQ, LED1};
	//LED2Struct = {&ledQ[2], &uartQ, LED2};
	//LED3Struct = {&ledQ[3], &uartQ, LED3};
	

//need 3 led task creates as well

	//creating a task for the led0 to beat. Need to pass in a parameter of 0, to show that it's for led0
	xTaskCreate(taskHeartBeat, "LED0 Heart Beat", configMINIMAL_STACK_SIZE, (void *) 0, 1, NULL);
	
	xTaskCreate(taskUART, "Main UART Task", configMINIMAL_STACK_SIZE, &uartHandle, 1, NULL);
	
	//create the 3 tasks first, then suspend 2 of them before it actually starts by the start scheduler.
	//not sure if still need these and the 2 suspends.
	xTaskCreate(taskSystemControl, "Main Control Take 1 for LED1", configMINIMAL_STACK_SIZE, (void *) &controlLED1, 1, &ledHandle[1]);
	xTaskCreate(taskSystemControl, "Main Control Take 2 for LED2", configMINIMAL_STACK_SIZE, (void *) &controlLED2, 1, &ledHandle[2]);
	xTaskCreate(taskSystemControl, "Main Control Take 3 for LED3", configMINIMAL_STACK_SIZE, (void *) &controlLED3, 1, &ledHandle[3]);
	
	//now suspend the latter 2 tasks
	vTaskSuspend(ledHandle[1]);
	vTaskSuspend(ledHandle[2]);
	
	// Create a Task to Handle Button Press and Light LED
	xTaskCreate(taskSystemControl,                       // Function Called by task
	"My Button Task",                        // Task Name
	configMINIMAL_STACK_SIZE,                // Task Stack Size
	NULL,                                    // Any Parameters Passed to Task
	1,                                       // Task Priority
	NULL);                                   // Place to store Task Handle
	

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

