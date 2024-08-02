/*
 * =====================================================================================
 *
 *       Filename:  IUart.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/21/22 14:10:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef IUART_H
#define IUART_H 
#include <memory>
#include <string>
 namespace Hal {
	 
	enum BaudSettings {BAUDRATE_115200 = 0,BAUDRATE_921600, BAUDRATE_230400, BAUDRATE_460800} ;

	struct Settings {
				BaudSettings baud;
				//enum baud {BAUDRATE_115200 = 0,BAUDRATE_921600, BAUDRATE_230400, BAUDRATE_460800} baud;
			};
	class IUart {
		
		public:
			
		public:
			virtual  int Open() = 0;		
			virtual  void Close() = 0;		
			virtual  void Clean() = 0;		
			virtual  bool  IsOpen() = 0;		
			virtual int Send(char *msg, int n) = 0;
			virtual int Recevice( char *msg, int n) = 0;

		protected:
			 Settings m_settings;


	};

	std::shared_ptr<IUart> GetUart(std::string str);

}
#endif
