/*
 * =====================================================================================
 *
 *       Filename:  GameApi.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/03/2023 10:47:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef GAME_API_H
#define GAME_API_H 

#include <iostream>           // std::cout
#include <thread>             // std::thread, std::this_thread::yield
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable




#include "IUart.h"
#include <memory>
#include "protocol-v1/messages.h"
#include "utils/logger.h"

#ifdef MSVC
#elif __GNUC__
#include <unistd.h>
#endif
using namespace Simple;
/**
 * @class GameApi
 * @brief collection of functions to manage the GameApi device
 */
class HandleUartMsg;
namespace Simple {
	class GameApi {
	friend class HandleUartMsg;

	public:
		GameApi(std::shared_ptr<Hal::IUart> uart) {
			m_uart = uart;
		};

		void getDeviceInfo() {
			GetDeviceInfo getDeviceInfoMsg;

			auto msg = getDeviceInfoMsg();
			int n = m_uart->Send(msg->buffer,msg->size()); 
			(void)n;
		}



		void luaStartGame(int id) {
			GameStartCommand cmd;
			auto msg = cmd(id);
			int n = m_uart->Send(msg->buffer,msg->size()); 
			(void)n;
		}

		void loadBuffer(char * buffer, size_t size) {
			DataChunk dataChunk;

			for (int i = 0; i < size; i+= 200) {

				// first buffer, sign it with reswet bit  
				auto msg = dataChunk(&buffer[i], size - i > 200 ? 200 : size -i , /*rst*/ i == 0);

				int n = m_uart->Send(msg->buffer,msg->size()); 
				(void)n;
				wait();
			}

		}

//	private:

		void wait() {
			done = false;
			std::unique_lock lk(m_mtx);
			cv.wait(lk, [this]{return done;});
		}

		void signal() {
			std::lock_guard lk(m_mtx);
			done = true;
			cv.notify_one();
		}

	private:
		std::shared_ptr<Hal::IUart> m_uart;
		int m_idx;
		std::mutex m_mtx;
		std::condition_variable cv;
		bool done = false;

	};
};



#endif
