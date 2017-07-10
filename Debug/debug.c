/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: Quang Huy
 *
 * Last Changed By:  $Author: Quang Huy
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "board.h"
#include "debug.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void WriteToFlash(u32_t addrFlash, PositionFlash position);
	
static void ReadNbCMDFromFlash(uint32_t addrFlash);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                            __WEAK						                              */
/******************************************************************************/

/******************************************************************************/
void NBCMDWriteToFlash(u32_t devAddr, DirectionData dir)
{
		u32_t addrFlash = 0x00;
		PositionFlash position = MSB;
	
		switch(devAddr)
		{
			case ADDR_DEVICE_1:	addrFlash = ADDR_FLASH_DEVICE_1;
				break;
			case ADDR_DEVICE_2:	addrFlash = ADDR_FLASH_DEVICE_2;
				break;
			case ADDR_DEVICE_3:	addrFlash = ADDR_FLASH_DEVICE_3;
				break;
			default:
				break;
		}
		
		if(dir == CMD) position = MSB;
		else position = LSB;
		
		if(addrFlash != 0x00) WriteToFlash(addrFlash, position);
}

static void WriteToFlash(u32_t addrFlash, PositionFlash position)
{
		uint32_t nbCMD = 0;
		u8 nbCMDFlash[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	
		FLASH_ReadNbyte(addrFlash, nbCMDFlash, 8);
		
		if(position == MSB)
		{
				nbCMD = (uint32_t)( (nbCMDFlash[0]<<24) |
										(nbCMDFlash[1]<<16) | 
										(nbCMDFlash[2]<<8) | nbCMDFlash[3] );
				
				nbCMD++;
			
				nbCMDFlash[0] = (nbCMD>>24)&0xFF;
				nbCMDFlash[1] = (nbCMD>>16)&0xFF;
				nbCMDFlash[2] = (nbCMD>>8)&0xFF;
				nbCMDFlash[3] =  nbCMD&0xFF;
		}
		else //position = LSB
		{
				nbCMD = (uint32_t)( (nbCMDFlash[4]<<24) |
										(nbCMDFlash[5]<<16) | 
										(nbCMDFlash[6]<<8) | nbCMDFlash[7] );
			
				nbCMD++;
			
				nbCMDFlash[4] = (nbCMD>>24)&0xFF;
				nbCMDFlash[5] = (nbCMD>>16)&0xFF;
				nbCMDFlash[6] = (nbCMD>>8)&0xFF;
				nbCMDFlash[7] =  nbCMD&0xFF;
				
		}		
		FLASH_EraseNPage(addrFlash, 8);
		FLASH_WriteNbyte(addrFlash, nbCMDFlash, 8);
}

void sendNbCMDToPC(void_p px)
{
		//ReadNbCMDFromFlash(ADDR_FLASH_DEVICE_1);
		
		ReadNbCMDFromFlash(ADDR_FLASH_DEVICE_2);
	
		//ReadNbCMDFromFlash(ADDR_FLASH_DEVICE_3);
}

void resetNbCMDInFlash(void_p px)
{
		FLASH_EraseNPage(ADDR_FLASH_DEVICE_1, 8);
		UART_PutString(UART_DEBUG, "Earse ADDR_FLASH_DEVICE_1 success! \r\n");
		FLASH_EraseNPage(ADDR_FLASH_DEVICE_2, 8);
		UART_PutString(UART_DEBUG, "Earse ADDR_FLASH_DEVICE_2 success! \r\n");
		FLASH_EraseNPage(ADDR_FLASH_DEVICE_3, 8);
		UART_PutString(UART_DEBUG, "Earse ADDR_FLASH_DEVICE_3 success! \r\n");
}

void sendDataToPC(u32_t addr, u8_t temperature, u8_t humidity, u16_t lux)
{
		int temperature_chuc = (int)temperature/10;
		int temperature_dvi = (int)(temperature%10);
		
		int humidity_chuc = (int)humidity/10;
		int humidity_dvi = (int)(humidity%10);
		
		int lux_nghin = (int)lux/1000;
		int lux_tram = (int)(lux-lux_nghin*1000)/100;
		int lux_chuc = (int)(lux-lux_nghin*1000-lux_tram*100)/10;
		int lux_dvi = (int)(lux%10);
		
		UART_PutChar(UART_DEBUG, 'B');
		UART_PutChar(UART_DEBUG, 'D');
		UART_PutChar(UART_DEBUG, '*');

		UART_PutChar(UART_DEBUG, ((addr>>24)&0xFF)+48);
		UART_PutChar(UART_DEBUG, ((addr>>16)&0xFF)+48);
		UART_PutChar(UART_DEBUG, ((addr>>8)&0xFF)+48);
		UART_PutChar(UART_DEBUG, (addr&0xFF)+48);
		UART_PutChar(UART_DEBUG, '*');
		
		//Temperature
		UART_PutChar(UART_DEBUG, (temperature_chuc+48));
		UART_PutChar(UART_DEBUG, (temperature_dvi+48));
		UART_PutChar(UART_DEBUG, '*');
		
		//humidity
		UART_PutChar(UART_DEBUG, (humidity_chuc+48));
		UART_PutChar(UART_DEBUG, (humidity_dvi+48));
		UART_PutChar(UART_DEBUG, '*');
		
		//lux
		UART_PutChar(UART_DEBUG, (lux_nghin+48));
		UART_PutChar(UART_DEBUG, (lux_tram+48));
		UART_PutChar(UART_DEBUG, (lux_chuc+48));
		UART_PutChar(UART_DEBUG, (lux_dvi+48));
		UART_PutChar(UART_DEBUG, '*');
		
		UART_PutChar(UART_DEBUG, 'K');
		UART_PutChar(UART_DEBUG, 'T');
}

static void ReadNbCMDFromFlash(uint32_t addrFlash)
{
		u8 nbCMDReadFromFlash[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	
		FLASH_ReadNbyte(addrFlash, nbCMDReadFromFlash, 8);
	
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[0]);
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[1]);
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[2]);
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[3]);
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[4]);
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[5]);
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[6]);
		UART_PutChar(UART_DEBUG, nbCMDReadFromFlash[7]);
		
		UART_PutString(UART_DEBUG, "\r\n");
}
