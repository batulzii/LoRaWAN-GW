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
#include "LoRaMac_QH.h"
#include "board.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE                           64

#define TIME_WAITE_DATA_UNCONFIRMDOWN												2 	//Don vi: 0.2s
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/*!
 * User application data
 */
static u8 AppData[LORAWAN_APP_DATA_MAX_SIZE];

static uint8_t pktLen = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void prepareCMDToRelay(void_p px);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void GpioBoardMain(
		u32_t dwPin,
		StatusPeripheral status
);

void DebugMain(u32_t devAddr, DirectionData dir);

void HandleDataConfirmUp(uint8_t* data);

void sendCMDToRelay(u32_t maThietBi, u8_t relayIndex, u8_t status);
/******************************************************************************/
/*!
 * \brief   Prepares the payload of the frame
 */
static void PrepareTxFrame( int maThietBi, int relayIndex, bool status)
{
		pktLen = 0;
		u32_t addr = 0x00;
		switch(maThietBi)
		{
			case 1: addr = ADDR_DEVICE_1;
				break;
			case 2: addr = ADDR_DEVICE_2;
				break;
			case 3: addr = ADDR_DEVICE_3;
				break;
			default:
				break;
		}
		AppData[pktLen++] = (u8_t)addr&0xFF;
		AppData[pktLen++] = (u8_t)(addr>>8)&0xFF;
		AppData[pktLen++] = (u8_t)(addr>>16)&0xFF;
		AppData[pktLen++] = (u8_t)(addr>>24)&0xFF;
		
		switch(relayIndex)
		{
			case 1: AppData[pktLen++] = 0x02;
				break;
			case 2: AppData[pktLen++] = 0x03;
				break;
			default: AppData[pktLen++] = 0x00;
				break;
		}
		
		if(status) AppData[pktLen++] = 0x01;
		else AppData[pktLen++] = 0x00;
}

/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
static void SendFrame( void )
{
		Send(dataType_DataConfirmDown, AppData, pktLen);
	
    return;
}

void GpioBoardMain(u32_t dwPin, StatusPeripheral status)
{
		GpioBoard(dwPin, status);
}

void DebugMain(u32_t devAddr, DirectionData dir)
{
		if(USE_DEBUG == TRUE) NBCMDWriteToFlash(devAddr, dir);
}

void HandleDataConfirmUp(uint8_t* data)
{
		uint32_t devAddr = 0;
		uint8_t pktLenDataConfirmUp = 1; //Do byte dau tien dung de xac dinh loai ban tin, address o byte thu 2 (chi so = 1)
	
		devAddr = data[pktLenDataConfirmUp++];
		devAddr |= ( (uint32_t)data[pktLenDataConfirmUp++] << 8 );
		devAddr |= ( (uint32_t)data[pktLenDataConfirmUp++] << 16 );
		devAddr |= ( (uint32_t)data[pktLenDataConfirmUp++] << 24 );
	
		uint8_t temperature = data[pktLen++];
		pktLen++;
		uint8_t humidity = data[pktLen++];
		uint16_t lux = ( (uint16_t)data[pktLen++] << 8 );
		lux |= data[pktLen++]&0xFF;
	
		sendDataToPC(devAddr, temperature, humidity, lux);
}

void sendCMDToRelay(u32_t maThietBi, u8_t relayIndex, u8_t status)
{
		GpioBoardMain(LED_SEND, HIGH);
		GpioBoardMain(LED_RECEIVE, HIGH);

		PrepareTxFrame(maThietBi, relayIndex, status);
		SendFrame();
}

/**
 * Main application entry point.
 */
int main( void )
{
		BoardInitMcu();
		BoardInitPeriph();
		
		LoRaMacInitialization();
		
		TimerStart( 500, TIMER_FOREVER, prepareCMDToRelay, NULL);
	
		while( 1 )
		{
				processTimer();
		}
}

static void prepareCMDToRelay(void_p px)
{
		CSMAStart(2, 1, true);

		DebugMain(ADDR_DEVICE_2, CMD);
}

