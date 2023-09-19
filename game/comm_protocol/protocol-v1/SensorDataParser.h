/*
 * =====================================================================================
 *
 *       Filename:  SensorDataParser.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/21/2023 01:42:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef SENSOR_DATA_PARSER_H 
#define SENSOR_DATA_PARSER_H  
#include <array>
#include <stddef.h>
#include <memory>

namespace Simple {

	struct SensorDataParser {
		public:
			SensorDataParser(char DataSize, char DarkSize ) {
				m_DataSize = DataSize;
				m_DarkSize = DarkSize;
			}


			int  parse(const unsigned char *raw_data, int size) {
				int c=0;
				m_sysTick = *((int *)(raw_data));
				raw_data+=4;
				size-=4;  
				int i =0;
				int k=0;
				while (i < size ) {
					ir_data[k] = 0;
					if (c == 0 || c == 2) {
						// first two channels are dataSize	
						for (int j = 0; j < m_DarkSize; j++)
							ir_data[k] |= raw_data[i + j]<<(j*8);
						i+=m_DarkSize;

					} if (c == 1 || c==3) {
						// last two channels are darkSize	
						for (int j = 0; j < m_DataSize; j++)
							ir_data[k] |= raw_data[i + j]<<(j*8);
						i+=m_DataSize;

					}
					k++;					
					c++;
					c%=4;
				}
				return k;
			}

			int sysTick() {
				return m_sysTick;
			}
			int dataSize() { return  m_DataSize;}
			int darkSize() { return  m_DarkSize;}
			std::array<unsigned long, 128>  ir_data;

		private:
			int m_DataSize;
			int m_DarkSize;
			int m_sysTick;			


	};


}
#endif
