/*
 * =====================================================================================
 *
 *       Filename:  protocol.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2022 03:14:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H 
#include <string>
#include <memory>
#include <vector>

#include <cstring>
#include <type_traits>
#include <utility>

#include "protocol-v1/ProtoclDataStructures.h"
#include "protocol-v1/messages.h"
#include "protocol-v1/IHandleMsg.h"
#include "utils/logger.h"

#include "game_api.h"

extern int gg;
namespace Simple {
	class HandleUartMsg: public IHandleMsg {
		public:
			HandleUartMsg(const std::shared_ptr<GameApi>  & gameApi = nullptr) : 
				m_gameApi(gameApi)
		{};
			void NewData(const MsgToParse &msg) override {
				switch (msg.cmd) {
					case CmdId::DeviceInfoData:
						{
							static int cnt = 0;
							Payload::DeviceInfoData *di = (Payload::DeviceInfoData*)(msg.data.buffer);
							logger_print(LOG_INFO,"Get device Info Commnand : %d  sw ver: %d.%d.%d, hw_ver: %d.%d" ,cnt, di->sw_version[2],
								     di->sw_version[1],
								     di->sw_version[0],
								     di->hw_version[1],
								     di->hw_version[0]
								    );
							gg=0;
							cnt++;
							break;
						}
					case CmdId::AckNack:
						{
							Payload::AckNack *di = (Payload::AckNack*)(msg.data.buffer);											  				 if (di->ack_nack) {
								logger_print(LOG_INFO,"Ack");
							} else {
								logger_print(LOG_INFO,"NotNack");
							}
							m_gameApi->signal();
						}

				}

			};
			void Wrongdata() override {
			};


		private:
			std::shared_ptr<GameApi> m_gameApi;


	};





}



#endif
