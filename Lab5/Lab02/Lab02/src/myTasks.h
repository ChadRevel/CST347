////////////////////////////////////////////////////////////////////////////////
//File: myTasks.h
//Author:	Chad Revel
//This is the tasks header file of the fifth lab for CST 347
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
typedef struct rxStruct{QueueHandle_t theTXQ; QueueHandle_t theRXQ;}rxStruct;
//typedef struct ledStruct{QueueHandle_t ledQ; QueueHandle_t uartQ; uint8_t ledNum;}ledStruct;
	
//typedef struct queueStruct{QueueHandle_t theRXQ;}queueStruct;
	
//typedef enum currLED {LED1, LED2, LED3} currLED;
typedef enum timeDelay {INCREASE, DECREASE} timeDelay;


void taskSystemControl(void * pvParamaters);
void taskHeartBeat (void * pvParamaters);
void taskLED(void * pvParameters);
void taskUART (void* pvParameters);
void taskTX(void *pvParameters);
void taskRX(void *pvParameters);



#endif
