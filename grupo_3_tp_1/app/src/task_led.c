/*
 * task_led.c
 *
 *  Created on: Jul 5, 2025
 *      Author: HP VICTUS
 */


#include "task_led.h"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>

extern osMessageQId Queue01Handle;
extern osMessageQId QueueUartHandle;  // 👈 Cola de UART

void task_led_app(void const *argument)
{
	osEvent evt;

	for (;;)
	{
		evt = osMessageGet(Queue01Handle, osWaitForever);

		if (evt.status == osEventMessage)
		{
			// Enciende el LED
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			osDelay(200);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

			// Mensaje UART constante
			const char *msg = "LED ENCENDIDO\r\n";
			osMessagePut(QueueUartHandle, (uint32_t)msg, 0);
		}
	}
}

