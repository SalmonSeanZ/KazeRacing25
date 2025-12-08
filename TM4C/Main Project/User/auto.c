#include "auto.h"
#include "pwm_output.h"
#include "uart_vision.h"

void Auto_Init(void)
{
    UART_Vision_Init();
}

void Auto_RunFrame(void)
{
    vision_cmd_t cmd;

    if (!UART_Vision_GetCmd(&cmd) || cmd.valid == 0) {
        PWM_Output_WriteFrame(1500, 1500);
        return;
    }

    int32_t pwm_speed = 1500 + cmd.speed * 0.5f;
    int32_t pwm_steer = 1500 + cmd.steer * 0.5f;

    if (pwm_speed > 2000) pwm_speed = 2000;
    if (pwm_speed < 1000) pwm_speed = 1000;
    if (pwm_steer > 2000) pwm_steer = 2000;
    if (pwm_steer < 1000) pwm_steer = 1000;

    PWM_Output_WriteFrame((uint16_t)pwm_speed, (uint16_t)pwm_steer);
}
