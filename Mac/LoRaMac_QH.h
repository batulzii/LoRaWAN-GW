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
#ifndef __LORAMAC_QH_H__
#define __LORAMAC_QH_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
typedef enum typeOfData
{
		dataTypeJoin_Req							= 0x00,
		dataTypeJoin_Accept						= 0x01,
		dataType_DataUnconFirmUp			= 0x02,
		dataType_DataUnconFirmDown		= 0x03,
		dataType_DataConfirmUp				= 0x04,
		dataType_DataConfirmDown			= 0x05,
		dataType_FixReceiveMode				= 0x06,
		dataType_DataUnknow						= 0x07,	
}typeOfData_t;

/*!
 * LoRaMAC Status
 */
typedef enum eLoRaMacStatus
{
    /*!
     * Service started successfully
     */
    LORAMAC_STATUS_OK,
    /*!
     * Service not started - LoRaMAC is busy
     */
    LORAMAC_STATUS_BUSY,
    /*!
     * Service unknown
     */
    LORAMAC_STATUS_SERVICE_UNKNOWN,
    /*!
     * Service not started - invalid parameter
     */
    LORAMAC_STATUS_PARAMETER_INVALID,
    /*!
     * Service not started - invalid frequency
     */
    LORAMAC_STATUS_FREQUENCY_INVALID,
    /*!
     * Service not started - invalid datarate
     */
    LORAMAC_STATUS_DATARATE_INVALID,
    /*!
     * Service not started - invalid frequency and datarate
     */
    LORAMAC_STATUS_FREQ_AND_DR_INVALID,
    /*!
     * Service not started - the device is not in a LoRaWAN
     */
    LORAMAC_STATUS_NO_NETWORK_JOINED,
    /*!
     * Service not started - playload lenght error
     */
    LORAMAC_STATUS_LENGTH_ERROR,
    /*!
     * Service not started - playload lenght error
     */
    LORAMAC_STATUS_MAC_CMD_LENGTH_ERROR,
    /*!
     * Service not started - the device is switched off
     */
    LORAMAC_STATUS_DEVICE_OFF,
}LoRaMacStatus_t;

/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */
void LoRaMacInitialization( void );

/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */
LoRaMacStatus_t Send( typeOfData_t typeOfData, void *fBuffer, u16_t fBufferSize );

/**
 * @func   TypeOfDataNow
 * @brief  Check if transmission side received a respond from receiver side
 * @param  None
 * @retval TRUE if received, FALSE if not receive
 */
bool_t TypeOfDataNow(u8_t typeOfData, bool_t rw);

#endif // __LORAMAC_QH_H__
