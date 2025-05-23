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
#include <ostream>
#include <vector>
#include <thread>
#include <memory>
#include <iostream>
#include <fstream>

#include "protocol-v1/ProtocolStateMachine.h"
#include "protocol-v1/UartProtocolParser.h"

#include "utils/ThreadPool.h"
#include "utils/logger.h"
#include "game_api.h"
#include "HandleUartMsg.h"
#include "comm_protocol/protocol-v1/messages.h"

#include "IUart.h"
//#include "git.h"

#ifdef _MSC_VER
#include <windows.h>
static inline void  usleep(unsigned int x) {
	Sleep(x / 1000);
}
#elif __GNUC__
#include <unistd.h>
 //#include "signals.h"
#endif

#define UART_DEVICE "/dev/ttyACM0"
#define BYPASS "bypass\n\r"
static const uint8_t  QUIT_BYPASS [2]= {0x18 , 0x11};

char * read_file(char *filename, size_t & size)
{
  char * buffer;
  std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);
  if (!file.is_open()) {
	  std::cout<<"bad file name"<<std::endl;
	  exit(1);
  }
  size = file.tellg();
  file.seekg (0, std::ios::beg);
  buffer = new char [size];
  file.read (buffer, size);
  file.close();
  return buffer;
}

void run_the_application(int argc, char *argv[]) {
	auto threadPool= ThreadPool::Instance();
	auto uart = Hal::GetUart(UART_DEVICE);
	auto gameApi = std::make_shared<GameApi>(uart, std::make_shared<SignalManager>());				
	
	
	uart->Open();
	if (uart->IsOpen() == false) {
		logger_print(LOG_INFO,"cannot open uart %s",UART_DEVICE );				
		return;
	}

	char m_exit = false;
	size_t size;
	char * b = read_file(argv[1], size);

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

	threadPool->beginTask([uart, &m_exit, b,size,gameApi]()->void{
		// bypass the shell and move to direct uart comnication
		uart->Send(( char *)BYPASS, sizeof (BYPASS) );
		usleep(100000);

		// load and run the game
		gameApi->luaStopGame();
		gameApi->loadBuffer(b, size);
		gameApi->luaStartGame(0);	
		gameApi->exitBypassMode();


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

	run_the_application(argc, argv);


#if CONFIG_SINGLE_INSTANCE
	// Release the lock and close the file
	flock(fd, LOCK_UN);
	close(fd);
#endif
	return 0;
}

