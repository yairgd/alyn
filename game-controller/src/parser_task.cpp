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

#include "protocol-v1/UartProtocolParser.h"
#include "protocol-v1/ProtocolStateMachine.h"
#include "protocol-v1/ProtoclDataStructures.h"
#include "HandleUartMsg.h"
#include "IUart.h"


using namespace Simple;

static std::shared_ptr<Simple::UartProtocolParser> parser = nullptr;
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
		auto uart = Hal::GetUartShell(sh);
		auto handleUartMsg = std::make_shared<HandleUartMsg>(uart);
		auto g_protocolStateMachine = std::make_shared<Simple::ProtocolStateMachine> (handleUartMsg);		
		parser = std::make_shared<Simple::UartProtocolParser> (uart, g_protocolStateMachine);

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
