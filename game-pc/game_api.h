#ifndef GAME_API_H
#define GAME_API_H 

#include <iostream>
#include <memory>
#include "SignalManager.h"
#include "IUart.h"
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
		GameApi(std::shared_ptr<Hal::IUart> uart, std::shared_ptr<SignalManager> signalManager)
			: m_uart(uart), m_signalManager(signalManager) {}

		void getDeviceInfo() {
			GetDeviceInfo getDeviceInfoMsg;
			auto msg = getDeviceInfoMsg();
			int n = m_uart->Send(msg->buffer, msg->size());
			(void)n;
			m_signalManager->wait(); // Use SignalManager to wait	
		}

		void luaStartGame(int id) {
			GameStartCommand cmd;
			auto msg = cmd(id);
			int n = m_uart->Send(msg->buffer, msg->size());
			(void)n;
			m_signalManager->wait(); // Use SignalManager to wait
		}
		void luaStopGame() {
			GameStopCommand cmd;
			auto msg = cmd();
			int n = m_uart->Send(msg->buffer, msg->size());
			(void)n;
			m_signalManager->wait(); // Use SignalManager to wait
		}

		void loadBuffer(char* buffer, size_t size) {
			DataChunk dataChunk;
			for (size_t i = 0; i < size; i += 200) {
				// first buffer, sign it with reset bit
				auto msg = dataChunk(&buffer[i], size - i > 200 ? 200 : size - i, /*rst*/ i == 0);
				int n = m_uart->Send(msg->buffer, msg->size());
				(void)n;
				m_signalManager->wait(); // Use SignalManager to wait
			}
		}

		void exitBypassMode() {
			ExitBypassModeCommand exitBypassModeCommand;
			auto msg = exitBypassModeCommand();
			int n = m_uart->Send(msg->buffer, msg->size());
			(void)n;
			m_signalManager->wait(); // Use SignalManager to wait
		}



		void signal() {
			m_signalManager->signal(); // Use SignalManager to signal
		}

		private:
		std::shared_ptr<Hal::IUart> m_uart;
		std::shared_ptr<SignalManager> m_signalManager;

	};

}

#endif // GAME_API_H

