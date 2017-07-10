/******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author:	Huy
 *
 * Last Changed By:  $Author:  Huy$
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     
								By Huy 		$Date: 2017-2-6 	11:45:00
								By Hoang 	$Date: 2017-05-16 11:45:00
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "board.h"
#include "spi_gpio.h"

unsigned char TxBuf[32]="";

void SPI_Init()
{
	GPIO_SetPinIsOutput(NSS);
	GPIO_SetPinIsOutput(SCK);
	GPIO_SetPinIsOutput(MOSI);
	GPIO_SetPinIsInput(MISO, 1);
}

/****************************************************************************************************/
u8 SPI_RW(u8 Buff)
{
	u8 bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		if((Buff&0x80)==0x80) MOSI1;
		else MOSI0;
        delay_us(5);
		Buff = (Buff << 1);           // shift next bit into MSB..
		SCK1;                      // Set SCK high..
        delay_us(5);
		if(GPIO_PinReadInput(MISO)) Buff |= 0x01;
		SCK0;            		  // ..then set SCK low again
   	}
    return(Buff);           		  // return read uchar
}

/****************************************************************************************************/
u8 SPI_Read(u8 reg)
{
	u8 reg_val;

	NSS0;   	            // NSS low, initialize SPI communication...
	SPI_RW(reg&0x7F);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	NSS1;	                // NSS high, terminate SPI communication

	return(reg_val);        // return register value
}

/****************************************************************************************************/
u8 SPI_Write(u8 reg, u8 value)
{
	u8 status;

	NSS0;                   // NSS low, init SPI transaction
	status = SPI_RW(reg|0x80);      // select register
	SPI_RW(value);             // ..and write value to it..
	NSS1;                   // NSS high again

	return(status);            // return nRF24L01 status uchar
}

/****************************************************************************************************/
void SPI_Read_Buf(u8 reg, u8_p pBuf, u8 uchars)
{
	u8 uchar_ctr;
	
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++)
	{
		pBuf[uchar_ctr] = SPI_Read(reg); 
	}
	return;
}

/*****************************************************************************************************/
void SPI_Write_Buf(u8 reg, u8_p pBuf, char_t uchars)
{
	u8 uchar_ctr;
	
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++)
	{
		SPI_Write(reg, pBuf[uchar_ctr]);
	}
	return;
}
