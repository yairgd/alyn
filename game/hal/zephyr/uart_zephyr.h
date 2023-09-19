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
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>

namespace Hal {
	class UartZephyr: public IUart {
		public:
			UartZephyr(const struct device *dev ) {
				m_device = dev;
			};
			int Open() override ;
			void Close() override ;
			int Send(char *lpBuffer, int dNoOFBytestoWrite)  override;
			int Recevice( char* lpBuffer, int nNumberOfBytesToRead) override ;
			void Clean() override ;
			bool IsOpen() override ;


		private:
			int m_fd;
			const struct device * m_device;
	};

	std::shared_ptr<IUart> GetUartZephyr( const struct device *dev) ;
	
}
#endif

