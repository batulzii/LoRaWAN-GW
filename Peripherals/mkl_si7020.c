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
#include "mkl_si7020.h"

u8 dataHumidity[2];
u8 dataTemperature[2];

u8 si7020_humi()
{
	u16 si_humid16bit;
	u8 si_humid;
	I2CReadMultiRegisters(SI7020_ADDR, cmd_meas_humid[0], dataHumidity, 2);
	
	si_humid16bit = (dataHumidity[0]<<8)+dataHumidity[1];	
	//Tinh gia tri nhiet do
	si_humid=(u8)((125*si_humid16bit)/65536 - 6);
	return si_humid;
}

u8 si7020_temperature()
{
	u16 si_temper16bit;
	u8 si_temper;
	I2CReadMultiRegisters(SI7020_ADDR, cmd_meas_temp[0], dataTemperature, 2);
	
	si_temper16bit = (dataTemperature[0]<<8)+dataTemperature[1];	
	//Tinh gia tri nhiet do
	si_temper=(u8)((175.72*si_temper16bit)/65536 - 46.85);
	return si_temper;
}
