/*
 * =====================================================================================
 *
 *       Filename:  uart_emul.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/16/2022 05:24:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef UART_EMUL_H
#define UART_EMUL_H
#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <mutex>
#include <thread>

#include "../IUart.h"


namespace Hal {
	/**
	 * @class UartEmulator
	 * @brief emulates uart signals and replace the real Uart during testing.
	 */
	class UartEmulator: public IUart {

		private:

			/**
			 * @class CycBuffer
			 * @brief cyclyc buffer to implement both emulated FIOFs : transmit and receive
			 */
			class CycBuffer{
				public:

					/**
					 * get avalibale character from the cyclic buffer
					 * @param pointer to read the characater into it
					 * @return True if charactaer is valid, False character is non valid
					 */
					bool get(char *c) {
						std::unique_lock lk(m);

						if (head == tail)
							return false; // fifo empty
						*c = data[tail++];
						tail %= data.size();
						return true;
					};
					bool put(char c) {
						std::unique_lock lk(m);

						if ( ((head+1) % data.size()) == tail)
							return false; // fifo full
						data[head++] = c;
						head %= data.size();
						return true;
					};
					bool is_empty() {
						std::unique_lock lk(m);

						return head == tail;
					}
				private:
					std::array<char,4096> data;			
					int head = 0;
					int tail = 0;
					std::mutex m;				
			};

			CycBuffer TransmmitFifo;
			CycBuffer ReceiveFifo;
			int rate;
		public:
			UartEmulator(Settings  && settings) {
				m_settings = settings;
				switch (settings.baud ) {
					case BaudSettings::BAUDRATE_115200:
						rate = 115200;
						break;
				}
			}

			void WriteToTransmitFifo(char *s, int n) {
				while (n--  && TransmmitFifo.put(*s++));
				std::this_thread::sleep_for(std::chrono::microseconds( (int)(1000000*9.0/rate)));
			}

			int Open() override{
				return 0;
			}

			void Close() {};
			void Clean() {};

			int Send(char *msg, int n) {
				int i;

				for ( i = 0;i < n && TransmmitFifo.put(msg[i]); i++);
				return i;

			};

			int Recevice( char *msg, int n) {
				int i;

				std::this_thread::yield();
				for ( i = 0;i < n && !TransmmitFifo.is_empty() &&  TransmmitFifo.get(&msg[i])  ; i++);
				return i;
			};
			bool IsOpen() override{
				return true;
			}



	};

}

#endif

