////////////////////////////////////////////////////////////////////////////////
//File: myTasks.h
//Author:	Chad Revel
//This is the tasks header file of the sixth lab for CST 347
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

//void taskSystemControl(void * pvParamaters);
void taskHeartBeat (void * pvParamaters);
void taskLED(void * pvParameters);
void taskUART (void* pvParameters);
void taskTX(void *pvParameters);
void taskRX(void *pvParameters);
void buttonTask(void * pvParamaters);



#endif
