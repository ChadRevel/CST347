////////////////////////////////////////////////////////////////////////////////
//File:	main.c
//Author:	Chad Revel
//This is the main file of the sixth lab for CST 347
////////////////////////////////////////////////////////////////////////////////

//system includes
#include <asf.h>

// FreeRTOS Includes
#include <FreeRTOS.h>
#include <FreeRTOS_CLI.h>
#include "queue.h"
#include "ledDriver.h"
#include "myButtons.h"
#include "myTasks.h"
#include "myDefines.h"
#include "Uartdrv.h"
#include "cliCommands.h"

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

//these are the lab 5 char *'s
const char* uartSW0 = "\r\nHello FreeRTOS World\r\n";
const char* uartEXT_SW1 = "\r\nCST 347 – RTOS\r\n";
const char* uartEXT_SW2 = "\r\nLab 05 – Interrupts in FreeRTOS\r\n";
const char* myName = "\r\nChad\r\n";


//create the handles for the controlling the tasks
TaskHandle_t ledHandle = NULL;
TaskHandle_t controlHandle = NULL;
TaskHandle_t txHandle = NULL; 
TaskHandle_t rxHandle = NULL; 
TaskHandle_t buttonHandle = NULL;

//lab 5 queues
QueueHandle_t theTXQ;
QueueHandle_t theRXQ;
QueueHandle_t theLEDQ;


//make the structs within the structs	
struct rxStruct controlLED;
struct rxStruct rxParams;


int main (void)
{
	// Initialize The Board
	prvMiscInitialisation();
	intitializeLEDDriver();
	initializeButtonDriver();
	initUART(EDBG_UART);
	
	const char* startText = "This is Lab6\r\n";
	UARTPutStr(EDBG_UART, startText, 0);

	//creating the queues for lab 5. the tx, rx, and led queues
	//the tx queue with a size of 50 bytes, and & depth of 20 messages
	//the rx queue with a size of 1 byte, and & depth of 20 messages
	//the led queue with a size of 1 bytes, and & depth of 5 messages
	theTXQ = xQueueCreate(20, sizeof(char[50]));
	theRXQ = xQueueCreate(20, sizeof(char));
	theLEDQ = xQueueCreate(5, sizeof(int));
	
	//lab 5 structs to pass the values
	rxParams.theRXQ = theRXQ;
	rxParams.theTXQ = theTXQ;


	//creating a task for the led0 to beat. Need to pass in a parameter of 0, to show that it's for led0
	xTaskCreate(taskHeartBeat, "LED0 Heart Beat", configMINIMAL_STACK_SIZE, (void *) 0, heartbeatPriority, NULL);
	//for lab 5, have the same heartbeat task, have the led task block on message arrival and toggle leds with number,
	//tx task, and rx task
	xTaskCreate(taskTX, "The TX Task", configMINIMAL_STACK_SIZE, (void *) theTXQ, txTaskPriority, &txHandle);
	xTaskCreate(taskRX, "The RX Task", configMINIMAL_STACK_SIZE, (void *) &rxParams, rxTaskPriority, &rxHandle);
	xTaskCreate(taskLED, "The LED Task", configMINIMAL_STACK_SIZE, (void *) theLEDQ, ledTaskPriority, &ledHandle);
	xTaskCreate(buttonTask, "Button Task", configMINIMAL_STACK_SIZE, (void *) theTXQ, txTaskPriority, &buttonHandle);
	
	//lab 6 register commands with cli
	xTaskStatsCommand;//was suppose to be registering the commands, but I don't know how cause there's no example on how to do so in the pdf or on canvas.
	
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
