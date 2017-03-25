#ifndef _BOLIB_H
#define _BOLIB_H
#include "stdio.h"
#include "sys.h"

//-----------------here below is the uart part--------------------------------
#define USART_REC_LEN 200					//max receive bytes
#define EN_USART1_RX  1						//enable usart1 to rx
#define EN_USART2_RX  1						//enable usart2 to rx

extern u8 USART1_RX_BUF[USART_REC_LEN];		//rx cache
extern u8 USART2_RX_BUF[USART_REC_LEN];
extern u16 USART1_RX_STA;					//rx state
extern u16 USART2_RX_STA;					//rx state


void uart_init(u32 port, u32 bound);
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void USART1_send_buf(char *buf);
#endif
