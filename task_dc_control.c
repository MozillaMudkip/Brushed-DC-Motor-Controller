#include "task_dc_control.h"


/******************************************************************************/
// Function Declarations
/******************************************************************************/
static void task_dc_control(void *param);
static BaseType_t cli_handler_motor(
    char *pcWriteBuffer,
    size_t xWriteBufferLen,
    const char *pcCommandString);


/******************************************************************************/
// Global Variables
/******************************************************************************/
/* Queue used to send commands to control the DC motor */
QueueHandle_t q_motor;


/* The CLI command definition for the motor command */
static const CLI_Command_Definition_t xMotor =
    {
        "motor",
        "\r\nmotor < forward | reverse | brake | off >\r\n",
        cli_handler_motor,
        1
    };


/******************************************************************************/
// Static Function Definitions
/******************************************************************************/


/* This task receives commands from the queue to control motor direction */
static void task_dc_control(void *param)
{
    motor_message_type_t message_type;


    /* Suppress warning for unused parameter */
    (void)param;


    /* Repeatedly running part of the task */
    for (;;)
    {
        // Check the Queue. If nothing is in the queue, block until a message is received
        xQueueReceive(q_motor, &message_type, portMAX_DELAY);


        switch (message_type)
        {
        case MOTOR_FORWARD:
            cyhal_gpio_write(PIN_MOTOR_FORWARD, 1);
            cyhal_gpio_write(PIN_MOTOR_REVERSE, 0);
            break;


        case MOTOR_REVERSE:
            cyhal_gpio_write(PIN_MOTOR_FORWARD, 0);
            cyhal_gpio_write(PIN_MOTOR_REVERSE, 1);
            break;


        case MOTOR_BRAKE:
            cyhal_gpio_write(PIN_MOTOR_FORWARD, 1);
            cyhal_gpio_write(PIN_MOTOR_REVERSE, 1);
            break;


        case MOTOR_OFF:
        default:
            cyhal_gpio_write(PIN_MOTOR_FORWARD, 0);
            cyhal_gpio_write(PIN_MOTOR_REVERSE, 0);
            break;
        }
    }
}


/* FreeRTOS CLI Handler for the 'motor' command */
static BaseType_t cli_handler_motor(
    char *pcWriteBuffer,
    size_t xWriteBufferLen,
    const char *pcCommandString)
{
    const char *pcParameter;
    motor_message_type_t motor_type;


    BaseType_t xParameterStringLength, xReturn;


    /* Remove compile-time warnings about unused parameters */
    (void)pcCommandString;
    (void)xWriteBufferLen;
    configASSERT(pcWriteBuffer);


    /* Obtain the parameter string */
    pcParameter = FreeRTOS_CLIGetParameter(
        pcCommandString,        /* The command string itself */
        1,                      /* Return the first parameter */
        &xParameterStringLength /* Store the parameter string length */
    );
    /* Sanity check something was returned */
    configASSERT(pcParameter);


    /* Copy ONLY the parameter to pcWriteBuffer */
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    strncat(pcWriteBuffer, pcParameter, xParameterStringLength);


    if (strcmp(pcWriteBuffer, "forward") == 0)
    {
        motor_type = MOTOR_FORWARD;
    }
    else if (strcmp(pcWriteBuffer, "reverse") == 0)
    {
        motor_type = MOTOR_REVERSE;
    }
    else if (strcmp(pcWriteBuffer, "brake") == 0)
    {
        motor_type = MOTOR_BRAKE;
    }
    else if (strcmp(pcWriteBuffer, "off") == 0)
    {
        motor_type = MOTOR_OFF;
    }
    else
    {
        /* Return a string indicating an invalid parameter */
        memset(pcWriteBuffer, 0x00, xWriteBufferLen);
        sprintf(pcWriteBuffer, "Error... invalid parameter\n\r");
        xReturn = pdFALSE;
        return xReturn;
    }


    // Send the message to the motor task
    xQueueSendToBack(q_motor, &motor_type, portMAX_DELAY);


    /* Indicate that the command has completed */
    xReturn = pdFALSE;


    return xReturn;
}


/******************************************************************************/
// Public Function Definitions
/******************************************************************************/
void task_dc_control_init(void)
{
    cy_rslt_t rslt;


    // Initialize the pins that control the motor
    rslt = cyhal_gpio_configure(PIN_MOTOR_FORWARD, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);


    rslt = cyhal_gpio_configure(PIN_MOTOR_REVERSE, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);


    /* Create the Queue used to control the DC motor */
    q_motor = xQueueCreate(1, sizeof(motor_message_type_t));
    configASSERT(q_motor != NULL);


    /* Register the CLI command */
    FreeRTOS_CLIRegisterCommand(&xMotor);


    /* Create the task that will control the DC motor */
    xTaskCreate(
        task_dc_control,
        "Task_DC_Control",
        configMINIMAL_STACK_SIZE + 100, // Increase stack size for safety
        NULL,
        configMAX_PRIORITIES - 6,
        NULL);
}
