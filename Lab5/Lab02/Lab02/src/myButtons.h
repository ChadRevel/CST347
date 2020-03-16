////////////////////////////////////////////////////////////////////////////////
//File: myButton.h
//Author:	Chad Revel
//This is the button header file of the fifth lab for CST 347
////////////////////////////////////////////////////////////////////////////////
#ifndef MYBUTTONS_H_
#define MYBUTTONS_H_

#include <asf.h>
#include <FreeRTOS.h>
#include <task.h>

//initializeButtonDriver and readButton
void initializeButtonDriver(void);
uint8_t readButton(uint8_t uiButtonNum);
void button_handler(uint32_t id, uint32_t mask);

#endif /* MYBUTTONS_H_ */
