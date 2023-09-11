/*
 * =====================================================================================
 *
 *       Filename:  parser_task.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/11/2023 07:54:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <string.h>
#include <memory>

#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/i2c.h>
#include "zephyr/uart_zephyr.h"


#include "protocol-v1/ProtocolStateMachine.h"
#include "protocol-v1/ProtoclDataStructures.h"
#include "HandleUartMsg.h"

//const struct device *const sl_uart1 ;//= DEVICE_DT_GET(UART_NODE2);
#define UART_NODE2 DT_CHOSEN(zephyr_console)
const struct device *const sl_uart2 = DEVICE_DT_GET(UART_NODE2);

/* size of stack area used by each thread */
#define STACKSIZE 8096

/* scheduling priority used by each thread */
#define PRIORITY 7



void parser_message_listenr(void *p1,void *p2, void *p3)
{
	printk("message_in_listenr\n");

	auto uart = Hal::GetUartZephyr(sl_uart2);
	uart->Open();
	auto p  =  std::shared_ptr<Simple::HandleUartMsg>(new Simple::HandleUartMsg(uart) );
	std::shared_ptr<IProtocolParser> parser = std::shared_ptr<Simple::ProtocolStateMachine> ( new Simple::ProtocolStateMachine  (uart, p));


#if 0
	auto login =  [] () ->void{

	};	
	login();
#endif

	while (1) {
		parser->Parse();
	}
}

K_THREAD_DEFINE(parser_message_listenr_id, STACKSIZE, parser_message_listenr, 0, NULL, NULL, PRIORITY, 0, 0);

