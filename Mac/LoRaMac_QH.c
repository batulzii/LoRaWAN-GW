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
#include "LoRaMacCrypto.h"
#include "LoRaMac_QH.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*!
 * Maximum PHY layer payload size
 */
#define LORAMAC_PHY_MAXPAYLOAD                      255
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/*!
 * Buffer containing the data to be sent or received.
 */
static uint8_t LoRaMacBuffer[LORAMAC_PHY_MAXPAYLOAD];

/*!
 * Length of packet in LoRaMacBuffer
 */
static uint16_t LoRaMacBufferPktLen = 0;

/*!
 * Device Address
 */
static uint32_t LoRaMacDevAddr;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
/*!
 * \brief LoRaMAC layer frame buffer initialization
 *
 * \param [IN] macHdr      MAC header field
 * \param [IN] fCtrl       MAC frame control field
 * \param [IN] fOpts       MAC commands buffer
 * \param [IN] fPort       MAC payload port
 * \param [IN] fBuffer     MAC data buffer to be sent
 * \param [IN] fBufferSize MAC data buffer size
 * \retval status          Status of the operation.
 */
static LoRaMacStatus_t PrepareFrame( typeOfData_t typeOfDataSend, void *fBuffer, u16_t fBufferSize );

/*!
 * \brief LoRaMAC layer prepared frame buffer transmission with channel specification
 *
 * \remark PrepareFrame must be called at least once before calling this
 *         function.
 *
 * \param [IN] channel     Channel parameters
 * \retval status          Status of the operation.
 */
static LoRaMacStatus_t SendFrameOnChannel( void );

/**
 * @func   TaskHandleJoin_Req
 * @brief  Handle Join-Accept when GW received a Join-request from End device
 * @param  Input: payload - Join request form End device
 *         Output: GW respond a Join-Accept for End device
 * @retval None
 */
static void TaskHandleJoin_Req(void_p payload);

/**
 * @func   TaskHandleDataConfirmUp
 * @brief  Handle data when GW received DataConfirmUp from End device
 * @param  Input: payload - Data from End device
 * @retval None
 */
static void TaskHandleDataConfirmUp(void_p payload);

/**
 * @func   TaskHandleDataUnconFirmUp
 * @brief  Handle DataUnconfirmUp was received from End device.
 * @param  Input: payload - DataUnconfirmUp from End device
 *         Ouput: GW know that End device received DataConfirmDown
 * @retval None
 */
static void TaskHandleDataUnconFirmUp(void_p payload);

/**
 * @func   TaskHandleTxDone
 * @brief  Handle when a transmission is done
 * @param  Input px - None
 * @retval None
 */
static void TaskHandleTxDone(void_p px);

/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */
static void RxWindowSetup( bool_t rxContinuous );
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */
void OnRadioRxDone( u8_t *payload, u16_t size, u16_t rssi, u8_t snr );

/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */
void OnRadioTxDone( void );
/******************************************************************************/
/*                            __WEAK						                              */
/******************************************************************************/
__weak void GpioBoardMain( u32_t dwPin, StatusPeripheral status)
{
		UNUSED(dwPin);
		UNUSED(status);
}

__weak void DebugMain(u32_t devAddr, DirectionData dir)
{
		UNUSED(devAddr);
		UNUSED(dir);
}

__weak void HandleDataConfirmUp(uint8_t* data)
{
		UNUSED(data);
}
/******************************************************************************/
static LoRaMacStatus_t PrepareFrame( typeOfData_t typeOfDataSend, void *fBuffer, uint16_t fBufferSize )
{
		const void* payload = fBuffer;
	
		LoRaMacBufferPktLen = 0;
	
		TypeOfDataNow(dataType_DataUnknow, WRITE);
		
		LoRaMacBuffer[LoRaMacBufferPktLen++] = typeOfDataSend;
	
		switch(typeOfDataSend)
		{
			case dataTypeJoin_Accept:
				{
						LoRaMacBuffer[LoRaMacBufferPktLen++] =	LoRaMacDevAddr & 0xFF;
						LoRaMacBuffer[LoRaMacBufferPktLen++] = ( LoRaMacDevAddr >> 8 ) & 0xFF;
						LoRaMacBuffer[LoRaMacBufferPktLen++] = ( LoRaMacDevAddr >> 16 ) & 0xFF;
						LoRaMacBuffer[LoRaMacBufferPktLen++] = ( LoRaMacDevAddr >> 24 ) & 0xFF;
						LoRaMacBuffer[LoRaMacBufferPktLen++] = LORAWAN_NETWORK_ID;
					
						memcpy1( LoRaMacBuffer + LoRaMacBufferPktLen, 0, 5 );
						LoRaMacBufferPktLen = 11;
				}
				break;
			case dataType_DataUnconFirmDown:
				{
						LoRaMacBuffer[LoRaMacBufferPktLen++] = LoRaMacDevAddr & 0xFF;
						LoRaMacBuffer[LoRaMacBufferPktLen++] = ( LoRaMacDevAddr >> 8 ) & 0xFF;
						LoRaMacBuffer[LoRaMacBufferPktLen++] = ( LoRaMacDevAddr >> 16 ) & 0xFF;
						LoRaMacBuffer[LoRaMacBufferPktLen++] = ( LoRaMacDevAddr >> 24 ) & 0xFF;
					
						memcpy1( LoRaMacBuffer + LoRaMacBufferPktLen, 0, 6 );
						LoRaMacBufferPktLen = 11;
				}
				break;
			case dataType_DataConfirmDown:
				{
						memcpy1( LoRaMacBuffer + LoRaMacBufferPktLen, payload, 6 );
						LoRaMacBufferPktLen = 7;
						memcpy1( LoRaMacBuffer + LoRaMacBufferPktLen, 0, 4 );
						LoRaMacBufferPktLen = 11;
					
						LoRaMacDevAddr = LoRaMacBuffer[1]&0xFF;
						LoRaMacDevAddr |= ( LoRaMacBuffer[2] << 8 );
						LoRaMacDevAddr |= ( LoRaMacBuffer[3] << 16 );
						LoRaMacDevAddr |= ( LoRaMacBuffer[4] << 24 );
				}
				break;
			default:
				return LORAMAC_STATUS_SERVICE_UNKNOWN;
		}
	
		return LORAMAC_STATUS_OK;
}

static LoRaMacStatus_t SendFrameOnChannel( )
{
		SX1276SetMaxPayloadLength(LoRaMacBufferPktLen);
		SX1276SetTxConfig( LORA_TX_OUTPUT_POWER, LORA_FDEV, 
													LORA_BANDWIDTH, LORA_DATARATE, LORA_CODINGRATE, 
													LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON, 
													LORA_CRC_ON, LORA_FREQ_HOP_ON, LORA_HOP_PERIOUS,
													LORA_IQ_INVERSION_ON, LORA_TIMEOUT );
		
		SX1276Send(LoRaMacBuffer, LoRaMacBufferPktLen);
		
		return LORAMAC_STATUS_OK;
}

static void TaskHandleJoin_Req(void_p payload)
{
		uint8_t pktLen = 1; //Do byte dau tien dung de xac dinh loai ban tin, address o byte thu 2 (chi so = 1)
	
		uint8_t* data = (uint8_t*)payload;
	
		LoRaMacDevAddr = data[pktLen++];
		LoRaMacDevAddr |= ( (uint32_t)data[pktLen++] << 8 );
		LoRaMacDevAddr |= ( (uint32_t)data[pktLen++] << 16 );
		LoRaMacDevAddr |= ( (uint32_t)data[pktLen++] << 24 );
		
		GpioBoardMain(LED_SEND, TOGGLE);
		Send(dataTypeJoin_Accept, 0, 0);
		GpioBoardMain(LED_SEND, TOGGLE);
}

static void TaskHandleDataConfirmUp(void_p payload)
{
		uint8_t LORAWAN_NETWORK_IDFromGW = 0;
		
		uint8_t pktLen = 1; //Do byte dau tien dung de xac dinh loai ban tin, address o byte thu 2 (chi so = 1)
	
		uint8_t* data = (uint8_t*)payload;
	
		LORAWAN_NETWORK_IDFromGW = data[4];
	
		if(LORAWAN_NETWORK_IDFromGW == LORAWAN_NETWORK_ID)
		{
				LoRaMacDevAddr = data[pktLen++];
				LoRaMacDevAddr |= ( (uint32_t)data[pktLen++] << 8 );
				LoRaMacDevAddr |= ( (uint32_t)data[pktLen++] << 16 );
				LoRaMacDevAddr |= ( (uint32_t)data[pktLen++] << 24 );
			
				GpioBoardMain(LED_RECEIVE, HIGH);
				Send(dataType_DataUnconFirmDown, 0, 0);
				
				HandleDataConfirmUp(data);
		}
}	

static void TaskHandleDataUnconFirmUp(void_p payload)
{
		uint32_t address = 0;
	
		uint8_t pktLen = 1; //Do byte dau tien dung de xac dinh loai ban tin, address o byte thu 2 (chi so = 1)
	
		uint8_t* data = (uint8_t*)payload;
	
		address = data[pktLen++];
		address |= ( (uint32_t)data[pktLen++] << 8 );
		address |= ( (uint32_t)data[pktLen++] << 16 );
		address |= ( (uint32_t)data[pktLen++] << 24 );
		if(address==LoRaMacDevAddr)
		{
				GpioBoardMain(LED_SEND, LOW);
				TypeOfDataNow(dataType_DataUnconFirmUp, WRITE);
			
				DebugMain(address, RESPOND);
		}
}

static void TaskHandleTxDone(void_p px)
{
		RxWindowSetup(TRUE);
		GpioBoardMain(LED_RECEIVE, LOW);
}

static void RxWindowSetup( bool_t rxContinuous )
{
		SX1276SetRxConfig( LORA_BANDWIDTH, LORA_DATARATE,
													LORA_CODINGRATE, LORA_BANDWIDTH_AFC,
													LORA_PREAMBLE_LENGTH, LORA_SYS_TIMEOUT,
													LORA_FIX_LENGTH_PAYLOAD_ON, LORA_PAYLOAD_LENGTH,
													LORA_CRC_ON, LORA_FREQ_HOP_ON,
													LORA_HOP_PERIOUS, LORA_IQ_INVERSION_ON, rxContinuous);
	
		SX1276SetRx();
}

void LoRaMacInitialization( void )
{
		SX1276Init();
		SX1276SetChannel(LORA_RF_FREQUENCY);
		
		RxWindowSetup(LORA_RX_CONTINUOUS);
}

LoRaMacStatus_t Send( typeOfData_t typeOfDataSend, void *fBuffer, uint16_t fBufferSize )
{
		LoRaMacStatus_t status = LORAMAC_STATUS_PARAMETER_INVALID;
	
		// Prepare the frame
    status = PrepareFrame( typeOfDataSend, fBuffer, fBufferSize );
		
		// Validate status
    if( status != LORAMAC_STATUS_OK )
    {
        return status;
    }
		
		status = SendFrameOnChannel();
		
		return status;
}

void OnRadioRxDone( u8_t *payload, u16_t size, u16_t rssi, u8_t snr )
{
		switch(payload[0])
		{
			case dataTypeJoin_Req:
				{
						TimerStart(0, 1, TaskHandleJoin_Req, payload);
				}
				break;
			case dataType_DataConfirmUp:
				{
						TimerStart(0, 1, TaskHandleDataConfirmUp, payload);
				}
				break;
			case dataType_DataUnconFirmUp:
				{
						TimerStart(0, 1, TaskHandleDataUnconFirmUp, payload);
				}
				break;
			default:
				break;
		}
}

void OnRadioTxDone( void )
{
		TimerStart(0, 1, TaskHandleTxDone, NULL);
}

bool_t TypeOfDataNow(u8_t typeOfData, bool_t rw)
{
	static u8_t t_OfData = dataType_DataUnknow;
	
	if(typeOfData == t_OfData) return TRUE;
	else if(rw == WRITE) t_OfData = typeOfData;
	
	return FALSE;
}
