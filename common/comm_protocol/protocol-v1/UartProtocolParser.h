/*
 * =====================================================================================
 *
 *       Filename:  UartProtocolParser.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/12/2022 07:10:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef UART_POROTOCL_PARSER_H
#define UART_POROTOCL_PARSER_H 
#include <memory>
#include <thread>
#include <functional>
#include <vector>
#include <iostream>
#include <mutex>

#include "common/hal/IUart.h"
#include "UartProtocolParser.h"
#include "ProtocolStateMachine.h"

namespace Simple {

	class UartProtocolParser  {
		public:
			UartProtocolParser(std::shared_ptr<Hal::IUart> uart,
					   std::shared_ptr<Simple::ProtocolStateMachine> protocolStateMachine)
				: m_protocolStateMachine(protocolStateMachine) , m_uart(uart)  {};
			std::shared_ptr<Hal::IUart> GetUart()  {
				return m_uart;
			}

			void Pushdata(unsigned char * data, int n) {
				for (int i = 0;i < n ; i++) {
					m_protocolStateMachine->StateTrack(data[i]);

				}
			}
			
			void Parse() {
				if (m_uart == nullptr)
					return;
				int res = 0; // ok
				if (!m_uart->IsOpen() ) {
					res = m_uart->Open();
					if(!res) {
						std::cout << "Failed to open uart"<<std::endl;	
						//TODO: consider exiting the function. Currently continue with optimistic approach
					}
				} 

				int  n = m_uart->Recevice(reinterpret_cast<char*>(b),sizeof(b));
				if (n > 0)
					Pushdata(b, n);
			}

		private:
			std::shared_ptr<Simple::ProtocolStateMachine> m_protocolStateMachine = nullptr;			
			std::shared_ptr<Hal::IUart> m_uart = nullptr;
			unsigned char b[256];

	};

}
#endif

