/*
 * task_led.h
 *
 *  Created on: Jul 5, 2025
 *      Author: HP VICTUS
 */

#ifndef INC_TASK_LED_H_
#define INC_TASK_LED_H_

#include "cmsis_os.h"

/* Declaraciones de las tareas para los LEDs */
void task_led_red(void *argument);
void task_led_green(void *argument);
void task_led_blue(void *argument);

#endif /* INC_TASK_LED_H_ */
