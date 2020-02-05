////////////////////////////////////////////////////////////////////////////////
//File: myTasks.h
//Author:	Chad Revel
//This is the tasks header file of the third lab for CST 347
////////////////////////////////////////////////////////////////////////////////
#ifndef MYTASKS_H_
#define MYTASKS_H_

#include <asf.h>
#include <FreeRTOS.h>
#include "task.h"
#include "myButtons.h"
#include "ledDriver.h"
#include "myDefines.h"


void taskSystemControl(void * pvParamaters);
void taskHeartBeat (void * pvParamaters);
void taskLED(void * pvParameters);
QueueHandle_t queueCreation(void);
void mainControlTask(void);



#endif
