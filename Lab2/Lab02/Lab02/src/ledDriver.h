////////////////////////////////////////////////////////////////////////////////
//File:	ledDriver.c
//Author:	Chad Revel
//This is the led driver header file of the second lab for CST 347
////////////////////////////////////////////////////////////////////////////////

void intitializeLEDDriver(void);
uint8_t readLED(uint8_t uiLedNum);
uint8_t setLED(uint8_t uiLedNum, uint8_t uiLedValue);
uint8_t toggleLED(uint8_t uiLedNum);