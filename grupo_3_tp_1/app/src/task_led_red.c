#include "main.h"
#include "cmsis_os.h"
#include "task_led.h"
#include "board.h"


extern SemaphoreHandle_t sem_led_red;

void task_led_red(void *argument)
{
	while (1)
	{
		if (xSemaphoreTake(sem_led_red, portMAX_DELAY) == pdTRUE)
		{
			HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
			vTaskDelay(pdMS_TO_TICKS(1000));
			HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
		}
	}
}
