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
#ifndef UART_P_H
#define UART_P_H 
#include <string>
#include "IUart.h"

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>


static  int _set_blocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		return errno;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 0;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
		return errno;
	return 0;
}




static  int _set_interface_attribs (int fd, int speed, int parity)
{
	struct termios tty;
	if (tcgetattr (fd, &tty) != 0)
	{
		//	error_message ("error %d from tcgetattr", errno);
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
							// disable IGNBRK for mismatched speed tests; otherwise receive break
							// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
					// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON); //tty is the name of the struct termios

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
					// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;
	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
		return errno;
	}
	return 0;
}

static double what_time_is_it1()
{
	struct timespec now = { 0 };
	clock_gettime(0, &now);
	return now.tv_sec + now.tv_nsec * 1e-9;
}



namespace Hal {

	class UartLinux: public IUart {
		public:
			UartLinux(std::string device, Settings  && settings ) {
				m_device = device;
				m_settings = settings;
			};
			int Open() override ;
			void Close() override ;
			int Send(char *lpBuffer, int dNoOFBytestoWrite)  override;
			int Recevice( char* lpBuffer, int nNumberOfBytesToRead) override ;
			void Clean() override ;
			bool IsOpen() override ;


		private:
			int m_fd;
			std::string m_device;
	};
}
#endif

