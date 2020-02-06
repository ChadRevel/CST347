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

char UARTbuffer [50];
enum theDirection dirSize;


//////////////////////////////////////////////////
//lab 3 part

//create the 3 queue handles for the leds and the controllers
QueueHandle_t ledQ[3] = {0, 0, 0};
QueueHandle_t UARTQ = 0;
QueueHandle_t ledController[3] = {NULL, NULL, NULL};

//create the handles for the leds
TaskHandle_t LEDHandle[3] = {NULL, NULL, NULL};


//make message enum that is increase or decrease. So the queue can send anything across.

int main (void)
{
	// Initialize The Board
	prvMiscInitialisation();
	intitializeLEDDriver();
	initializeButtonDriver();
	
	/*
	The MainControl task will poll the status of all three switches (just as we did in lab 2). 
	The switches should be debounced with a three stage strategy: 
	Read the switch, Delay 10ms using a vTaskDelay, then read switch again to verify the change. 
	In addition you should implement a lockout feature in your driver only allowing a switch to perform a single action on a press. 
	There will be three of these tasks (only 1 active at a time). 
	Each of these task will send messages via a queue to its corresponding LED task. 
	The switches will perform the following function.
	*/
	
	QueueHandle_t * theQueueHandles = queueCreation();

	//create the queues for the handles
	ledQ[1] = xQueueCreate(5, sizeof(theDirection));
	ledQ[2] = xQueueCreate(5, sizeof(theDirection));
	ledQ[3] = xQueueCreate(5, sizeof(theDirection));
	UARTQ = xQueueCreate(5, sizeof(UARTBuffer));


	//create a data input for the parameters of the leds
	struct inputLEDTask ledParameter1 = {LED1, ledQ[1]};
	struct inputLEDTask ledParameter2 = {LED2, ledQ[2]};
	struct inputLEDTask ledParameter3 = {LED3, ledQ[3]};

	
	//create a data input for the parameters of the 3 main controls
	struct mainTaskInput mainControlLEDParameter1 = (ledQ[1], ledController1, UARTQ);
	struct mainTaskInput mainControlLEDParameter2 = (ledQ[2], ledController2, UARTQ);
	struct mainTaskInput mainControlLEDParameter3 = (ledQ[3], ledController3, UARTQ);
	
	
	//create the 3 tasks first, then suspend 2 of them before it actually starts by the start scheduler.
	//not sure if still need these and the 2 suspends.
	xTaskCreate(taskMainControl, "Main Control Take 1", configMINIMAL_STACK_SIZE, NULL, 1, &LED1H);
	xTaskCreate(taskMainControl, "Main Control Take 2", configMINIMAL_STACK_SIZE, NULL, 1, &LED2H);
	xTaskCreate(taskMainControl, "Main Control Take 3", configMINIMAL_STACK_SIZE, NULL, 1, &LED3H);
	
	//now suspend the latter 2 tasks
	vTaskSuspend(LED2H);
	vTaskSuspend(LED3H);
	
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

