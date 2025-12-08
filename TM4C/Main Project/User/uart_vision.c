#include "uart_vision.h"
#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include <string.h>

static vision_cmd_t g_cmd;
static char rx_buf[32];
static uint8_t idx = 0;

void UART_Vision_Init(void)
{
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0; // UART0
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; // GPIOA

    GPIO_PORTA_AFSEL_R |= (1<<0) | (1<<1);
    GPIO_PORTA_PCTL_R  |= (1<<0) | (1<<4);
    GPIO_PORTA_DEN_R   |= (1<<0) | (1<<1);

    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 27;   // 115200 @50MHz
    UART0_FBRD_R = 8;
    UART0_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
    UART0_CTL_R |= UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE;

    g_cmd.valid = 0;
}

static void parse_frame(void)
{
    int16_t x = 0, y = 0;

    // expected format: "x,y\n" e.g. "120,-200\n"
    if (sscanf(rx_buf, "%hd,%hd", &x, &y) == 2) {
        g_cmd.steer = x;
        g_cmd.speed = y;
        g_cmd.valid = 1;
    }

    memset(rx_buf, 0, sizeof(rx_buf));
    idx = 0;
}

void UART0_Handler(void)
{
    if (UART0_MIS_R & UART_MIS_RXMIS) {
        char c = UART0_DR_R;

        if (c == '\n') {
            parse_frame();
        } else if (idx < sizeof(rx_buf)-1) {
            rx_buf[idx++] = c;
        }

        UART0_ICR_R = UART_ICR_RXIC;
    }
}

uint8_t UART_Vision_GetCmd(vision_cmd_t *cmd)
{
    if (!cmd) return 0;
    *cmd = g_cmd;
    return g_cmd.valid;
}
