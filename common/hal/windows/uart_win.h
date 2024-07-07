#pragma once
#include<windows.h>
#include<string>

#include <stdio.h>
#include <fileapi.h>
#include <sysinfoapi.h>

#include "IUart.h"

namespace Hal {
	class UartWin : public IUart {
		public:
			UartWin(std::string name, Settings&& settings) 
			{
				m_name = name;
				hComm = 0;
				m_settings = settings;
			};
			int Send(char* lpBuffer, int dNoOFBytestoWrite) override;
			int Open() override;
			void Clean() override {};
			int Recevice(char* lpBuffer, int nNumberOfBytesToRead)  override;
			void Close() override;

			bool IsOpen() override {
				return true;
			}
			/**
			 * @class OPEN_PACKET
			 * @brief private class to use for win32 uart access implementaion 
			 */
			struct OPEN_PACKET
			{
				PCSTR lpFileName;
				HANDLE hFile;
				DWORD error;
				DWORD dwDesiredAccess;
				DWORD dwShareMode;
				DWORD dwCreationDisposition;
				DWORD dwFlagsAndAttributes;
			};


		private:
			HANDLE hComm;
			CHAR name[64];
			std::string m_name;
	};

}
