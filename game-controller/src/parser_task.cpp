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
#include "parser_task.h"

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
#include "zephyr/uart_shell.h"
#include <zephyr/shell/shell.h>


#include "protocol-v1/ProtocolStateMachine.h"
#include "protocol-v1/ProtoclDataStructures.h"
#include "HandleUartMsg.h"
#include "IUart.h"

//const struct device *const sl_uart1 ;//= DEVICE_DT_GET(UART_NODE2);
//#define UART_NODE2 DT_CHOSEN(zephyr_console)
//const struct device *const sl_uart2 = DEVICE_DT_GET(UART_NODE2);

/* size of stack area used by each thread */
//#define STACKSIZE  2048 

/* scheduling priority used by each thread */
//#define PRIORITY 7


static std::shared_ptr<IProtocolParser> parser = nullptr;

static  Simple::HandleUartMsg g_handleUartMsg;
static  Simple::ProtocolStateMachine g_protocolStateMachine;

#define USING_UART_SHELL
#ifdef USING_UART_SHELL
/**
 * Created  06/16/24
 * @brief   this functions process the message comming from the uart data. it is caled from the shell
 * when it switch into bypass mode. look for function: bypass_cb
 * @param   
 * @return  
 */
void proccess_char(const  struct shell * sh, uint8_t *data, size_t len) {
	if (parser == nullptr) {
		std::shared_ptr<Hal::IUart> uart = Hal::GetUartShell(sh);
	//	auto p  =  std::shared_ptr<Simple::HandleUartMsg>(new Simple::HandleUartMsg(uart) );
	//	parser = std::shared_ptr<Simple::ProtocolStateMachine> ( new Simple::ProtocolStateMachine  (nullptr, p));
	
		g_handleUartMsg = Simple::HandleUartMsg(uart);
	//	auto p =  std::shared_ptr<Simple::HandleUartMsg>(g_handleUartMsg);
		g_protocolStateMachine = Simple::ProtocolStateMachine(nullptr,std::shared_ptr<Simple::HandleUartMsg>(&g_handleUartMsg));

		parser =  std::shared_ptr<Simple::ProtocolStateMachine> ( &g_protocolStateMachine);


	} else {
		parser->Pushdata(data, len);
	}
	
}

#else


void parser_message_listenr(void *p1,void *p2, void *p3)
{
	printk("message_in_listenr\n");

	const  struct shell * sh =  ( (struct shell *) p1); 

	auto uart = Hal::GetUartShell(sh);
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


#endif
