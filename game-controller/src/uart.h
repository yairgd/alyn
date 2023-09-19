/*
 * =====================================================================================
 *
 *       Filename:  uart.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/02/2023 10:03:21 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef UART_1H
#define UART_1H 
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

int  uart_send(uint8_t *c, int n) ;
void uart_init(void);
int  uart_receive_one_char(uint8_t *c);

#ifdef __cplusplus
}
#endif


#endif
