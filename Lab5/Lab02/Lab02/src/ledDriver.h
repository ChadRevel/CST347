////////////////////////////////////////////////////////////////////////////////
//File:	ledDriver.c
//Author:	Chad Revel
//This is the led driver main file of the fifth lab for CST 347
////////////////////////////////////////////////////////////////////////////////
#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

#include <asf.h>
#include <FreeRTOS.h>

void intitializeLEDDriver(void);
uint8_t readLED(uint8_t uiLedNum);
uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue);
uint8_t toggleLED(uint8_t uiLedNum);

#endif /* LEDDRIVER_H_ */