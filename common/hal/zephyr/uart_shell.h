/*
 * =====================================================================================
 *
 *       Filename:  Uart.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/21/22 14:12:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef UART_ZEPHYR_H
#define UART_ZEPHYR_H 
#include <string>
#include "IUart.h"
#include <memory>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/shell/shell.h>

namespace Hal {
	class UartShell: public IUart {
		public:
			UartShell(const struct shell * sh) {
				m_sh = sh;
			};
			int Open() override ;
			void Close() override ;
			int Send(char *lpBuffer, int dNoOFBytestoWrite)  override;
			int Recevice( char* lpBuffer, int nNumberOfBytesToRead) override ;
			void Clean() override ;
			bool IsOpen() override ;

		private:
			const struct shell * m_sh;
	};

	std::shared_ptr<IUart> GetUartShell( const struct shell * sh);
	
}
#endif

