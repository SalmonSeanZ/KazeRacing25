#ifndef UART_VISION_H_
#define UART_VISION_H_

#include <stdint.h>

typedef struct {
    int16_t speed;  // y
    int16_t steer;  // x
    uint8_t valid;  // 1 = valid
} vision_cmd_t;

void UART_Vision_Init(void);
uint8_t UART_Vision_GetCmd(vision_cmd_t *cmd);

#endif
