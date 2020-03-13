////////////////////////////////////////////////////////////////////////////////
//File: myTasks.h
//Author:	Chad Revel
//This is the tasks header file of the fourth lab for CST 347
////////////////////////////////////////////////////////////////////////////////
#ifndef MYTASKS_H_
#define MYTASKS_H_

#include <asf.h>
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "Uartdrv.h"
#include "myButtons.h"
#include "ledDriver.h"
#include "myDefines.h"

//create structs for the globals
typedef struct controlStruct{QueueHandle_t ledQ[3]; QueueHandle_t uartQ;}controlStruct;
typedef struct ledStruct{QueueHandle_t ledQ; QueueHandle_t uartQ; uint8_t ledNum;}ledStruct;
	
//typedef enum currLED {LED1, LED2, LED3} currLED;
typedef enum timeDelay {INCREASE, DECREASE} timeDelay;


void taskSystemControl(void * pvParamaters);
void taskHeartBeat (void * pvParamaters);
void taskLED(void * pvParameters);
void taskUART (void* pvParameters);
void modifiedLEDTask(void *pvParameters);



#endif
