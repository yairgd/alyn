/*
 * =====================================================================================
 *
 *       Filename:  test1.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/25/2023 01:55:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <memory>
#include <chrono>
#include <vector>
#include <thread>
#include <memory>
#include <iostream>
#include<fstream>

#include "protocol-v1/ProtocolStateMachine.h"
#include "protocol-v1/UartProtocolParser.h"

#include "HandleUartMsg.h"
#include "game_api.h"
#include "SignalManager.h"

#include "utils/ThreadPool.h"
#include "utils/logger.h"

#include "IUart.h"

#ifdef _MSC_VER
#include <windows.h>
#define usleep(x) Sleep(x/1000)
#elif __GNUC__
#include <unistd.h>
#endif


#define UART_DEVICE "/dev/ttyACM0"
#define BYPASS "bypass\n\r"

void run_the_application() {
	auto threadPool= ThreadPool::Instance();
	auto uart = Hal::GetUart(UART_DEVICE);
	auto gameApi = std::make_shared<GameApi>(uart, std::make_shared<SignalManager>());				
	
	uart->Open();
	if (uart->IsOpen() == false) {
		logger_print(LOG_INFO,"cannot open uart %s",UART_DEVICE );				
		return;
	}

	char m_exit = false;

	threadPool->beginTask([uart,gameApi, &m_exit]()->void{
		auto handleUartMsg  =  std::make_shared<Simple::HandleUartMsg>(gameApi);		
		auto protocolStateMachine = std::make_shared<Simple::ProtocolStateMachine> (handleUartMsg);		
		auto uartProtocolParser = std::make_shared<Simple::UartProtocolParser> (uart, protocolStateMachine);


		std::mutex m;

		while (true) {
			m.lock();
			if (m_exit) {
				m.unlock();						
				return;
			}
			m.unlock();
			uartProtocolParser->Parse();
		}
	});

	threadPool->beginTask([uart, gameApi, &m_exit]()->void{
		// bypass the shell and move to direct uart comnication		
		uart->Send(( char *)BYPASS, sizeof (BYPASS) );
		usleep(100000);

		for (int i=0;i<100;i++) {
			gameApi->getDeviceInfo(); // works
		}
		gameApi->exitBypassMode();

		// exit from bypass mode		
	//	uart->Send(( char *)BYPASS, sizeof (BYPASS) );
	//	usleep(100000);

		m_exit = true;		
		return;

	});





	logger_print(LOG_DEBUG,"joining %ld threads at ThreadPool",threadPool->size() );


	threadPool->finish();
	threadPool->join();
}


int main(int argc, char *argv[]) {

#ifdef __GNUC__
	logger_init(0, LOG_LOCAL0);
	//	set_signal_handlers();
#endif
	logger_set_log_level(7);
	logger_print(LOG_DEBUG,"Begin to Run");

# if  CONFIG_SINGLE_INSTANCE
	// Open the lock file
	int fd = open("/var/run/myprogram.lock", O_CREAT | O_RDWR, 0644);
	if (fd < 0) {
		// Error opening the lock file
		return 1;
	}
	// Acquire an exclusive lock on the file
	if (flock(fd, LOCK_EX | LOCK_NB) < 0) {
		// Lock failed, another instance is running
		return 1;
	}
#endif

	run_the_application();


#if CONFIG_SINGLE_INSTANCE
	// Release the lock and close the file
	flock(fd, LOCK_UN);
	close(fd);
#endif
	return 0;
}

