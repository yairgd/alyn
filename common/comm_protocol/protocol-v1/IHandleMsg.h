/*
 * =====================================================================================
 *
 *       Filename:  IHandleMsg.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/27/2023 06:12:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef IHANDLE_MSG_h
#define IHANDLE_MSG_h 

#include "protocol-v1/messages.h"

namespace Simple {
	using ParsingData = Payload::GenericParsingData<1024>;
	using MsgToParse = Msg<ParsingData> ;

	class IHandleMsg  {
		public:

			virtual void NewData(const MsgToParse &msg) = 0;
			virtual void Wrongdata() = 0;

	};
}
#endif

