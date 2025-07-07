#include "main.h"
#include "cmsis_os.h"
#include "task_led.h"
#include "board.h"


extern SemaphoreHandle_t sem_led_green;

void task_led_green(void *argument)
{
	while (1)
	{
		if (xSemaphoreTake(sem_led_green, portMAX_DELAY) == pdTRUE)
		{
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
			vTaskDelay(pdMS_TO_TICKS(1000));
			HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
		}
	}
}
