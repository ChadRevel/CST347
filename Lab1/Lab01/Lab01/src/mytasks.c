#include <asf.h>
#include <FreeRTOS.h>
#include "mytasks.h"
#include "task.h"


void myButtonTask(void * pvParamaters)
{
	/* Block for 500ms. */
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	while (true)
	{
		/* Is button pressed? */
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE)
		{

			/* Yes, so turn LED on. */
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
			vTaskDelay( xDelay );
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
			vTaskDelay( xDelay );	
			
		}
		else
		{
			/* No, so turn LED off. */
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}

