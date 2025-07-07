/*
 * task_button.h
 *
 *  Created on: Jul 5, 2025
 *      Author: HP VICTUS
 */

#ifndef INC_TASK_BUTTON_H_
#define INC_TASK_BUTTON_H_

#include "cmsis_os.h"

typedef enum
{
  BUTTON_TYPE_NONE = 0,
  BUTTON_TYPE_PULSE,
  BUTTON_TYPE_SHORT,
  BUTTON_TYPE_LONG,
} button_type_t;


void task_button(void *argument);
void EXTI_Callback_Button(void);  // para usar desde el callback de interrupción

#endif /* INC_TASK_BUTTON_H_ */
