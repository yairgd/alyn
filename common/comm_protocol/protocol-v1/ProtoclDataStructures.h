/*
 * =====================================================================================
 *
 *       Filename:  messages.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/12/2022 12:25:00 PM/home/yair/alyn/project/game-controller/src/HandleUartMsg.h
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef ProtoclDataStructures_h
#define ProtoclDataStructures_h 

#pragma pack(push)
#pragma pack(1)


namespace Simple {
	// this commands are received are being send to MCU
	enum struct  CmdId:unsigned char   {
		Reserved=0,
		DeviceInfoData,
		GetDeviceInfo, 
		AckNack,
		DataChunk,
		GameStartCommand,
		GameStopCommand,
		ExitBypassModeCommand,

	};

	

	struct Payload {	
		struct EmptyStruct {
			//int empty;
		};

		struct DataChunk {
			unsigned short size : 8;
			unsigned short reset : 1;
			unsigned char buffer [253];
		};


		struct Command {
			enum struct  Id:unsigned char {
				GameStart,
				GameStop,
				ExitBypassMode,
			} id;
			int params[10];
		};		

		// this data structures are commands ID are being received from MCU
		struct DeviceInfoData {
			char sw_version[3];
			char hw_version[2];
			char mcu_serial_number[8];
		};


		struct AckNack  {
			char ack_nack;
		};

		template<int n>
			struct GenericParsingData {
				unsigned char buffer[n]; 
				int size;
			};
	};


};

#pragma pack(pop)
#endif
