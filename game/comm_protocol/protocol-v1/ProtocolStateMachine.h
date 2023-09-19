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

#ifndef PROTOCOL_StateMachine_H
#define PROTOCOL_StateMachine_H 
#include <string>
#include <memory>
#include <vector>

#include <cstring>
#include <type_traits>
#include <utility>


#include "IProtocolParser.h"
//#include "protocol-v1/HandleUartMsg.h"
#include "protocol-v1/ProtoclDataStructures.h"
#include "protocol-v1/SensorDataParser.h"
#include "protocol-v1/messages.h"
#include "protocol-v1/IHandleUartMsg.h"


namespace Simple {

	class ProtocolStateMachine: public IProtocolParser {

		public:
			ProtocolStateMachine(std::shared_ptr<Hal::IUart> uart, std::shared_ptr<Simple::IHandleUartMsg>  handleUartMsg):IProtocolParser(uart), 
			m_handleMsg(std::move(handleUartMsg)) {}

		private:
			void StateTrack(unsigned char c) override {
				switch (state) {
					case ReceiveState::Preamble:
						if (c == 0xaa) {
							state = MsgType;
							msg.crc = 0xaa;
							idx = 0;
							state = ReceiveState::MsgType;							
						} else {
							state = ReceiveState::Preamble;														
						}
						break;
					case ReceiveState::MsgType:
						msg.crc+=c;
						state = ReceiveState::Cmd;
						break;
					case ReceiveState::Cmd:
						msg.cmd =static_cast<Simple::CmdId>(c);
						msg.crc+=c;						
						state = ReceiveState::Length;
						length = 0;
						break;
					case ReceiveState::Length:
						msg.crc+=c;						
						length = reinterpret_cast<uint8_t>(c) - 1;
						msg.data.size = c;
						state = ReceiveState::Data;
						break;
					case ReceiveState::Data:
						msg.crc+=c;						
						msg.data.buffer[idx++] = c;
						// update crc with c
						if (length) {
							length--;
						} else {
							state = ReceiveState::CheckSum;	
						}
						break;
					case ReceiveState::CheckSum:
						msg.crc+=0xee;						
						if (c != msg.crc ) {
							state = ReceiveState::Preamble;
							m_handleMsg->Wrongdata();	
						} else{
							state = ReceiveState::Suffix;							
						}
						break;
					case ReceiveState::Suffix:
						if (c != 0xee) {
							m_handleMsg->Wrongdata();	
						} else {
							m_handleMsg->NewData(msg);

						}
						state = ReceiveState::Preamble;						
						break;
				}
			}

		private:
			enum ReceiveState {Preamble, MsgType, Cmd, Length, Data, CheckSum,  Suffix};
			ReceiveState state  = Preamble; 
			unsigned char length;
			int idx;
			MsgToParse msg;
			std::shared_ptr<Simple::IHandleUartMsg>  m_handleMsg ;

	};


}



#endif

