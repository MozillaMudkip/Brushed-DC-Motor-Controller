#ifndef __TASK_DC_CONTROL_H__
#define __TASK_DC_CONTROL_H__

#include "main.h"

#define PIN_MOTOR_FORWARD P9_0  // In1: Forward Control
#define PIN_MOTOR_REVERSE P9_2  // In2: Reverse Control

/* DC motor control message type */
typedef enum
{
    MOTOR_FORWARD = 0,
    MOTOR_REVERSE = 1,
    MOTOR_BRAKE = 2,
    MOTOR_OFF = 3
} motor_message_type_t;

extern QueueHandle_t q_motor;
void task_dc_control_init(void);

#endif
