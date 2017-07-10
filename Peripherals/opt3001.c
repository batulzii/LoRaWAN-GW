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
#include "opt3001.h"
 
u8 dataLUX[2];

u16 opt3001_pow(u8 heSoMu)//2
{
	u8 i;
	u16 kq=1;
	
	if(heSoMu==0) return 1;
	
	for(i=1; i<255; i++)
	{
		kq *= 2;
		if(i==heSoMu) return kq;
	}
	return 0;
}
 
u16 opt3001_LUX(void)
{
	u16 opt_LUX16bit, heso;
	u8 opt_LUX;
	u8 confiOpt[2];
	
	//Configure opt3001, chon che do single-shot
	confiOpt[0] = 0xCA;
	confiOpt[1] = 0x10;
	I2CWriteMultiRegisters(OPT3001_ADDR, 0x01, confiOpt, 2);
	
	I2CReadMultiRegisters(OPT3001_ADDR, REG_RESULT, dataLUX, 2);
	
	opt_LUX16bit = (dataLUX[0]<<8)+dataLUX[1];	
	
	//Tinh gia tri cuong do anh sang don vi LUX
	heso = opt3001_pow((opt_LUX16bit>>12)&0x000F); 
	opt_LUX = heso*0.01*(opt_LUX16bit&0x0FFF);
	
	return opt_LUX;
}
