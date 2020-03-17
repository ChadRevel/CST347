////////////////////////////////////////////////////////////////////////////////
//File: myDrivers.h
//Author:	Chad Revel
//This is the drivers header file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////
#ifndef MYTASKS_H_
#define MYTASKS_H_


#include <asf.h>
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "myButtons.h"
#include "ledDriver.h"
#include "myDefines.h"

//create structs for the globals to be used when passing pvParameters
typedef struct ledStruct{QueueHandle_t ledQ; uint8_t ledNum;}ledStruct;


void taskSystemControl(void * pvParamaters);
void taskHeartBeat (void * pvParamaters);

#endif