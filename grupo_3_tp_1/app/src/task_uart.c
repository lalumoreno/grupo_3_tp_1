/*
 * task_uart.c
 *
 *  Created on: Jul 5, 2025
 *      Author: HP VICTUS
 */

#include "task_uart.h"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>  // por si usas sprintf o snprintf
#include "board.h"


extern UART_HandleTypeDef huart2;
extern QueueHandle_t uart_queue;  // Definida en app.c

#define UART_MSG_MAX_LEN 64

void task_uart(void *argument)
{
	char msg[UART_MSG_MAX_LEN];

	while (1)
	{
		if (xQueueReceive(uart_queue, &msg, portMAX_DELAY) == pdTRUE)
		{
			HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
		}
	}
}

/**
 * @brief Envía un mensaje por UART de forma segura a través de la cola
 * @param msg Cadena null-terminated (máx. 63 caracteres)
 */
void task_uart_app(const char *msg)
{
	if (msg == NULL) return;

	char buffer[UART_MSG_MAX_LEN];
	strncpy(buffer, msg, UART_MSG_MAX_LEN - 1);
	buffer[UART_MSG_MAX_LEN - 1] = '\0';  // Asegura terminación

	xQueueSend(uart_queue, &buffer, 0);  // 0 = no espera si está llena
}

void uart_enviar_estado_boton(void)
{
	GPIO_PinState state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	const char* mensaje;

	if (state == GPIO_PIN_RESET)  // Activo-bajo: presionado
		mensaje = "Estado del boton: PRESIONADO\r\n";
	else
		mensaje = "Estado del boton: LIBERADO\r\n";

	HAL_UART_Transmit(&huart2, (uint8_t*)mensaje, strlen(mensaje), HAL_MAX_DELAY);
}
