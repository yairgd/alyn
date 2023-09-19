/*
 * =====================================================================================
 *
 *       Filename:  IProtocolParser.h
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

#ifndef PROTOCL_PARSER_H
#define PROTOCL_PARSER_H 
#include <memory>
#include <thread>
#include <functional>
#include <vector>
#include <iostream>
#include <mutex>


#include "IUart.h"

#if 0
#ifdef __GNUC__
#include <unistd.h>
#elif _MSC_VER
#include <windows.h>
#endif
#endif

class IProtocolParser {
	public:
		IProtocolParser(std::shared_ptr<Hal::IUart> uart) : m_uart(uart)  {};
		virtual void StateTrack(unsigned char c)  = 0;
		std::shared_ptr<Hal::IUart> GetUart()  {
			return m_uart;
		}

		void Pushdata(unsigned char * data, int n) {
			for (int i = 0;i < n ; i++) {
				StateTrack(data[i]);

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
			
			int  n = m_uart->Recevice(reinterpret_cast<char*>(b),100);
			if (n > 0)
				Pushdata(b, n);
		}

	private:
		std::shared_ptr<Hal::IUart> m_uart = nullptr;
		unsigned char b[1024];


};
#endif

