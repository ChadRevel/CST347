////////////////////////////////////////////////////////////////////////////////
//File:	main.c
//Author:	Chad Revel
//This is the command line interface file of the sixth lab for CST 347
////////////////////////////////////////////////////////////////////////////////

#include "cliCommands.h"



/*lab 6 cli commands that will be needed:


start_led LED_int DELAY_ms – Start <LED_int> blinking using a toggle delay time of <DELAY_ms>.
If the specified LED is already blinking, the LED will not be affected but a user message
“LED already started” will be sent to the terminal.

stop_led LED_int – Stop <LED_int> from blinking. If the specified LED is not currently blinking,
the LED will not be affected but a user message “LED already stopped” will be sent to the terminal.

change_led LED_int DELAY_ms – Change <LED_int>’s blink delay time to <DELAY_ms>. If the specified
LED is not currently blinking, the LED will not be affected but a user message “LED is stopped” will
be sent to the terminal.
*/

//task-stats – Provide a listing tasks and task related information
// Note: You should check to make sure everything fits in the buffer you are sending back. This example does not...
portBASE_TYPE prvTaskStatsCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString)
{
	sprintf(pcWriteBuffer, taskListHdr);
	pcWriteBuffer += strlen(taskListHdr);
	//check if everything fits into the buffer
	//if (sizeof(taskListHdr) > pcWriteBuffer)
	//{
		//
	//}
	//else
	vTaskList(pcWriteBuffer);

	return pdFALSE;
}



