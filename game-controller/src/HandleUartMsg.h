/*
 * =====================================================================================
 *
 *       Filename:  protocol.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2022 03:14:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H 
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_uart.h>

#include <string>
#include <memory>
#include <vector>

#include <cstring>
#include <type_traits>
#include <utility>


#include "protocol-v1/ProtoclDataStructures.h"
#include "protocol-v1/messages.h"
#include "protocol-v1/IHandleMsg.h"
#include "utils/logger.h"
#include "game.h"
#include "hal/IUart.h"

using namespace Simple;

extern "C" {
	int set_bypass(const struct shell *sh, shell_bypass_cb_t bypass);
}

class HandleUartMsg: public IHandleMsg {
	public:
		HandleUartMsg() = default;
		HandleUartMsg(std::shared_ptr<Hal::IUart> uart) : m_uart(uart)  {};
		void NewData(const MsgToParse &msg) override {
			switch (msg.cmd) {
				case CmdId::GetDeviceInfo:
					{

						Payload::DeviceInfoData data;
						DeviceInfoData deviceInfoData;
						data.hw_version[0] = 1;
						data.hw_version[1] = 2;
						data.sw_version[2] = 3;
						data.sw_version[0] = 4;
						data.sw_version[1] = 5;


						auto msg = deviceInfoData(data);
						int n = m_uart->Send(msg->buffer,msg->size()); 
						(void)n;

						break;
					}
				case CmdId::DataChunk:
					{
						Payload::DataChunk *di = (Payload::DataChunk*)(msg.data.buffer);	
						/* save data in memory */
						push_lua_code_to_memory(di->buffer, di->size, di->reset);
						// send ok message
						Ack ack;
						auto res_msg = ack();
						int n = m_uart->Send(res_msg->buffer,res_msg->size()); 
						(void)n;
						break;
					}
				case CmdId::GameStartCommand:
					{
						Payload::Command *di = (Payload::Command*)(msg.data.buffer);	
						struct game * g = game_get_by_index(di->params[0]);
						game_start(g);
						// send ok message
						Ack ack;
						auto res_msg = ack();
						int n = m_uart->Send(res_msg->buffer,res_msg->size()); 
						(void)n;
						break;
					}
				case CmdId::GameStopCommand:
					{
						game_stop();
						// send ok message
						Ack ack;
						auto res_msg = ack();
						int n = m_uart->Send(res_msg->buffer,res_msg->size()); 
						(void)n;
						break;
					}
				case CmdId::ExitBypassModeCommand:
					{
						Payload::Command *di = (Payload::Command*)(msg.data.buffer);	
						(void)di;
						const struct shell *sh = shell_backend_uart_get_ptr();
						set_bypass (sh, NULL);
						// send ok message
						Ack ack;
						auto res_msg = ack();
						int n = m_uart->Send(res_msg->buffer,res_msg->size()); 
						(void)n;
						break;

					}

				default:
					{
						// Reserved
						// AckNack
						// DeviceInfoData
					}
			}

		};
		void Wrongdata() override {
		};
	private:
		std::shared_ptr<Hal::IUart> m_uart;


};


#endif
