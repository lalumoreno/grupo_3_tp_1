#include "main.h"
#include "cmsis_os.h"
#include "task_button.h"
#include "task_ui.h"
#include "task_led.h"
#include "task_uart.h"
#include "board.h"

/* Definición de colas y semáforos */
QueueHandle_t button_event_queue;
QueueHandle_t uart_queue;  // NUEVO: cola para enviar mensajes UART

SemaphoreHandle_t sem_led_red;
SemaphoreHandle_t sem_led_green;
SemaphoreHandle_t sem_led_blue;

void app_init(void)
{
	/* Crear cola UART */
	uart_queue = xQueueCreate(5, sizeof(char[64]));
	configASSERT(uart_queue != NULL);
	if (uart_queue == NULL) {
		// Fallback directo si falla
		while (1);
	}

	/* Crear cola de eventos del botón */
	button_event_queue = xQueueCreate(1, sizeof(button_type_t));
	configASSERT(button_event_queue != NULL);
	if (button_event_queue == NULL) {
		task_uart_app("Error: no se pudo crear la cola de botón\r\n");
		while (1);
	}

	/* Crear semáforos para cada LED */
	sem_led_red = xSemaphoreCreateBinary();
	sem_led_green = xSemaphoreCreateBinary();
	sem_led_blue = xSemaphoreCreateBinary();

	configASSERT(sem_led_red != NULL);
	configASSERT(sem_led_green != NULL);
	configASSERT(sem_led_blue != NULL);

	/* Crear tareas del sistema */
	BaseType_t status;

	status = xTaskCreate(task_uart, "task_uart", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate(task_button, "task_button", 128, NULL, tskIDLE_PRIORITY + 2, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate(task_ui, "task_ui", 128, NULL, tskIDLE_PRIORITY + 2, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate(task_led_red, "task_led_red", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate(task_led_green, "task_led_green", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate(task_led_blue, "task_led_blue", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
	configASSERT(status == pdPASS);

	/* Enviar mensaje por UART */
	task_uart_app("App init\r\n");
#ifdef _F429ZI_
	task_uart_app("Board F429ZI\r\n");
#endif

}
