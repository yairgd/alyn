
# if _MSC_VER
#include <windows.h>
#include <stdio.h>
#include <fileapi.h>
#include <sysinfoapi.h>
#include <string>
#include "uart_win.h"
#include <memory>


ULONGLONG WINAPI OpenComPort(struct Hal::UartWin::OPEN_PACKET* op)
{
	HANDLE hFile = CreateFile(op->lpFileName, op->dwDesiredAccess, op->dwShareMode,
				  NULL, op->dwCreationDisposition, op->dwFlagsAndAttributes, NULL);

	op->error = hFile == INVALID_HANDLE_VALUE ? GetLastError() : NOERROR;
	op->hFile = hFile;

	return GetLastError();// for possible use GetExitCodeThread but use op->error better
}

int Hal::UartWin::Open() {

	HANDLE hThread;
	//m_name = "COM38";
	snprintf(name, 64, "\\\\.\\%s", m_name.c_str());

	struct OPEN_PACKET op = { name, INVALID_HANDLE_VALUE, ERROR_IO_PENDING,
		GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING, 0 };

	int fSuccess = 0;

	ULONGLONG ticks = GetTickCount64();

	if (hThread = CreateThread(NULL, 4096, (PTHREAD_START_ROUTINE)OpenComPort, &op, 0, NULL))
	{
		if (WaitForSingleObject(hThread, 100) == WAIT_TIMEOUT)//1 sec for example
		{
			CancelSynchronousIo(hThread);
			WaitForSingleObject(hThread, INFINITE);
		}
		CloseHandle(hThread);
	}
	else
	{
		op.error = GetLastError();
	}

	ticks = GetTickCount64() - ticks;


	if (op.hFile != INVALID_HANDLE_VALUE)
	{
		// CloseHandle(op.hFile);
		hComm = op.hFile;
		//  Initialize the DCB structure.
		DCB serialParams = { 0 };
		SecureZeroMemory(&serialParams, sizeof(DCB));
		serialParams.DCBlength = sizeof(DCB);
		//	serialParams.BaudRate = 921600; // CBR_115200;
		serialParams.ByteSize = 8;
		serialParams.StopBits = ONESTOPBIT;
		serialParams.Parity = NOPARITY;


		switch (m_settings.baud) {
			case IUart::Settings::baud::BAUDRATE_115200:
				{
					serialParams.BaudRate = CBR_115200;
					break;
				}
			case IUart::Settings::baud::BAUDRATE_921600:
				{
					serialParams.BaudRate = 921600;
					break;
				}
		}

		fSuccess = SetCommState(hComm, &serialParams);
		if (!fSuccess)
		{
			//  Handle the error.
			printf("GetCommState failed with error %d.\n", GetLastError());
			//return (-1);
		}

		COMMTIMEOUTS CommTimeOuts = {0};
		CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
		CommTimeOuts.ReadTotalTimeoutConstant = 100;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
		CommTimeOuts.WriteTotalTimeoutConstant = 100;

		if (!SetCommTimeouts(hComm, &CommTimeOuts)) {
			printf("invalid handle !!!\n");
			return -1;
		}
		return 0;
	}
	return -1;
}


void Hal::UartWin::Close() {
	CloseHandle(hComm);//Closing the Serial Port
}

int Hal::UartWin::Recevice(char* lpBuffer, int nNumberOfBytesToRead) {
	DWORD dNoOfBytesWritten = 0;
	int Status = 0;
	DWORD lpNumberOfBytesRead = 0;

	Status = ReadFile(
		hComm,
		lpBuffer,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		NULL
		);

	if (Status)
		return lpNumberOfBytesRead;

	return -1;
}

int Hal::UartWin::Send(char* lpBuffer, int dNoOFBytestoWrite) {
	DWORD dNoOfBytesWritten = 0;
	int Status;
	Status = WriteFile(hComm,        // Handle to the Serial port
			   lpBuffer,     // Data to be written to the port
			   dNoOFBytestoWrite,  //No of bytes to write
			   &dNoOfBytesWritten, //Bytes written
			   NULL);


	if (Status)
		return dNoOfBytesWritten;
	return -1;

}
 namespace Hal {
	std::shared_ptr<IUart> GetUart(std::string com_port) {
		return std::make_shared<UartWin>(com_port.c_str(), IUart::Settings({ IUart::Settings::baud::BAUDRATE_115200 }));
	}
}
#endif
