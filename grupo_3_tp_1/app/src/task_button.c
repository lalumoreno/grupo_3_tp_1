#include "cmsis_os.h"
#include "main.h"
#include "task_button.h"
#include "task_uart.h"
#include "board.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* Cola compartida con task_ui */
extern QueueHandle_t button_event_queue;

/* Parámetros */
#define TASK_PERIOD_MS_           (50)
#define BUTTON_PULSE_TIMEOUT_     (200)
#define BUTTON_SHORT_TIMEOUT_     (1000)
#define BUTTON_LONG_TIMEOUT_      (2000)

/* Estados internos del botón */
typedef enum {
	BUTTON_STATE_IDLE,
	BUTTON_STATE_PRESSED
} button_state_t;

typedef struct {
	uint32_t counter;
	button_state_t state;
} button_info_t;

typedef struct {
	button_type_t type;
	uint32_t duration;
} button_event_t;

static button_info_t button_info;

/* Inicializa el estado del botón */
static void button_init_(void)
{
	button_info.counter = 0;
	button_info.state = BUTTON_STATE_IDLE;
}

/* Clasifica la pulsación */
static button_event_t button_process_state_(bool is_pressed)
{
	button_event_t event = { BUTTON_TYPE_NONE, 0 };

	if (is_pressed)
	{
		uart_enviar_estado_boton();
		button_info.counter += TASK_PERIOD_MS_;
		button_info.state = BUTTON_STATE_PRESSED;
	}
	else
	{
		if (button_info.state == BUTTON_STATE_PRESSED)
		{
			// Se soltó el botón (flanco de bajada)
			event.duration = button_info.counter;

			if (button_info.counter >= BUTTON_LONG_TIMEOUT_){
				event.type = BUTTON_TYPE_LONG;
			} else if (button_info.counter >= BUTTON_SHORT_TIMEOUT_) {
				event.type = BUTTON_TYPE_SHORT;
			} else if (button_info.counter >= BUTTON_PULSE_TIMEOUT_) {
				event.type = BUTTON_TYPE_PULSE;
			}

			// Reset
			button_info.counter = 0;
			button_info.state = BUTTON_STATE_IDLE;
			uart_enviar_estado_boton();
		}
	}

	return event;
}

/* Tarea del botón (modo polling) */
void task_button(void* argument)
{
	(void) argument;

	button_init_();

	while (1)
	{
		GPIO_PinState state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
#ifdef _F429ZI_
		bool is_pressed = (state == GPIO_PIN_SET);
#else
		bool is_pressed = (state == GPIO_PIN_RESET);
#endif
		button_event_t event = button_process_state_(is_pressed);

		if (event.type != BUTTON_TYPE_NONE)
		{
			// Mostrar mensaje por UART

			char msg[64];
			sprintf(msg, "Boton: %s - Tiempo: %lu ms\r\n",
					(event.type == BUTTON_TYPE_LONG)  ? "LARGO" :
							(event.type == BUTTON_TYPE_SHORT) ? "CORTO" : "PULSO",
									event.duration);
			task_uart_app(msg);

			// Enviar tipo de evento a la cola
			xQueueSend(button_event_queue, &event.type, 0);
		}

		vTaskDelay(pdMS_TO_TICKS(TASK_PERIOD_MS_));
	}
}
