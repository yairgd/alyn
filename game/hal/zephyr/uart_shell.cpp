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
#include "uart_shell.h"
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_uart.h>

namespace Hal {
	Hal::UartShell uart(0);
	int UartShell::Open() {
		return 0;	
	}

	void UartShell::Close()  {
	

	}

	int UartShell::Send(char *lpBuffer, int dNoOFBytestoWrite)   {
		const struct shell * sh = shell_backend_uart_get_ptr();
		// taken from here:  zephyr/subsys/shell/shell_ops.c: 412
		size_t tmp_cnt;
		int err = sh->iface->api->write(sh->iface,
				((const uint8_t *) lpBuffer), dNoOFBytestoWrite,
				&tmp_cnt);

		return err <0 ? err : tmp_cnt ;	
	};



	int UartShell::Recevice( char* lpBuffer, int nNumberOfBytesToRead)   {
		  return 1;
	}

	void UartShell::Clean()  {
	}
	bool UartShell::IsOpen()  {
		return true;
	}


	std::shared_ptr<Hal::IUart> GetUartShell( const struct shell * sh) {
//		return std::shared_ptr<UartShell> (new UartShell (sh));
		uart = UartShell(sh);
		return std::shared_ptr<Hal::IUart>(&uart);
	}

};
