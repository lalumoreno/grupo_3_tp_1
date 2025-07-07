#include "cmsis_os.h"
#include "task_button.h"
#include "task_uart.h"
#include "board.h"
#include "task_uart.h"

/* Semáforos individuales para activar cada LED */
extern SemaphoreHandle_t sem_led_red;
extern SemaphoreHandle_t sem_led_green;
extern SemaphoreHandle_t sem_led_blue;

/* Cola de eventos del botón (proviene de task_button) */
extern QueueHandle_t button_event_queue;

void task_ui(void *argument)
{
	button_type_t button_event;

	while (1)
	{
		/* Espera un evento de tipo button_type_t enviado por task_button */
		if (xQueueReceive(button_event_queue, &button_event, portMAX_DELAY) == pdTRUE)
		{

			switch (button_event)
			{
			case BUTTON_TYPE_PULSE:
				task_uart_app(" UI: boton tipo PULSE - activar LED rojo\r\n");
				xSemaphoreGive(sem_led_red);
				break;

			case BUTTON_TYPE_SHORT:
				task_uart_app(" UI: boton tipo SHORT - activar LED verde\r\n");
				xSemaphoreGive(sem_led_green);
				break;

			case BUTTON_TYPE_LONG:
				task_uart_app(" UI: boton tipo LONG - activar LED azul\r\n");
				xSemaphoreGive(sem_led_blue);
				break;

			default:
				task_uart_app(" UI: tipo de pulsacion no reconocido\r\n");
				break;
			}
		}
	}
}
