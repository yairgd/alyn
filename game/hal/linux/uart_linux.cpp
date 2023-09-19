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
#include "uart_linux.h"
#include <string>

namespace Hal {
	int UartLinux::Open() {
		m_fd  = open (m_device.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
		if (m_fd < 0)
		{
			return -1 ;
		}
		switch (m_settings.baud) {
			case BaudSettings::BAUDRATE_115200: 
				{
					_set_interface_attribs (m_fd , B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
					break;
				}
			case BaudSettings::BAUDRATE_921600: 
				{
					_set_interface_attribs (m_fd , B921600, 0);  // set speed to 115,200 bps, 8n1 (no parity)
					break;
				}
			case BaudSettings::BAUDRATE_230400: 
				{
					_set_interface_attribs (m_fd , B230400, 0);  // set speed to 115,200 bps, 8n1 (no parity)
					break;
				}
			case BaudSettings::BAUDRATE_460800: 
				{
					_set_interface_attribs (m_fd , B460800, 0);  // set speed to 115,200 bps, 8n1 (no parity)
					break;
				}


		}
		_set_blocking (m_fd, 0);                // set no blocking
		tcflush(m_fd,TCIOFLUSH);
		return 0;
	}

	void UartLinux::Close()  {
		if (m_fd>0)
			close(m_fd);
		m_fd=-1;

	}

	int UartLinux::Send(char *lpBuffer, int dNoOFBytestoWrite)   {
		int size=1;
		int err=0;
	
		int n =  write(m_fd,lpBuffer,dNoOFBytestoWrite);
		if (n<0)
			return -1;



//tcdrain(m_fd); // Wait until transmission ends
//tcflush(m_fd, TCOFLUSH); // Clear write buffer
		       
		// wait for the outbuffer to clean;
		double t = what_time_is_it1();
		int b = 0;
		while (size && err==0  && !b) {
			err = ioctl( m_fd, FIONREAD, &size );
			b = what_time_is_it1() - t > 0.1 ;

		}
		if (b)
			Clean();


		if (err<0 ||size)
			return -1;
		return n;
	};



	int UartLinux::Recevice( char* lpBuffer, int nNumberOfBytesToRead)   {
		int n;
		struct pollfd fds[1];
		fds[0].fd = m_fd;
		fds[0].events = POLLIN;

		int ret = poll(fds, 1,  100);

		if (ret == -1) {
			perror ("poll");
			return -1;
		}

		if (!ret) {
			return 0;

		}

		if (fds[0].revents & POLLIN) {
			n = read(m_fd,lpBuffer,nNumberOfBytesToRead);	 

		}

		return n;
	}

	void UartLinux::Clean()  {
		int e = tcflush(m_fd, TCIFLUSH);

	}
	bool UartLinux::IsOpen()  {
		return m_fd > 0;
	}


	std::shared_ptr<IUart> GetUart(std::string com_port) {
		return std::make_shared<UartLinux> (com_port.c_str(), Settings({ BaudSettings::BAUDRATE_115200}));
	}

};
