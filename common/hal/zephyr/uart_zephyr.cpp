/*
 * =====================================================================================
 *
 *       Filename:  uart_linux.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/14/2023 04:27:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "uart_zephyr.h"
#include "game-controller/src/uart.h"

namespace Hal {
	int UartZephyr::Open() {
		 uart_init();
		return 0;	
	}

	void UartZephyr::Close()  {
	

	}

	int UartZephyr::Send(char *lpBuffer, int dNoOFBytestoWrite)   {
		/*
		for (int i=0;i<dNoOFBytestoWrite;i++)
			uart_poll_out(m_device, lpBuffer[i] );	
			*/
		uart_send((uint8_t*)lpBuffer, dNoOFBytestoWrite) ;
		
		return dNoOFBytestoWrite ;	
	};



	int UartZephyr::Recevice( char* lpBuffer, int nNumberOfBytesToRead)   {
		  uart_receive_one_char((uint8_t *)lpBuffer);
		  return 1;
		
#if 0
		int ret = uart_poll_in(m_device, (unsigned char*)lpBuffer);


		if (ret < 0)
			return 0;
		return 1;
#endif	
		
	}

	void UartZephyr::Clean()  {
	}
	bool UartZephyr::IsOpen()  {
		return true;
	}


	std::shared_ptr<IUart> GetUartZephyr( const struct device *dev) {
		return std::shared_ptr< UartZephyr> (new UartZephyr ( dev));
	}

};
