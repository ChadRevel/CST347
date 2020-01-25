////////////////////////////////////////////////////////////////////////////////
//File: myDrivers.h
//Author:	Chad Revel
//This is the drivers header file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////


#include <asf.h>
#include <FreeRTOS.h>
#include "task.h"
#include "myButtons.h"
#include "ledDriver.h"
#include "myDefines.h"


void taskSystemControl(void * pvParamaters);
void taskHeartBeat (void * pvParamaters);