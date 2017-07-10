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
#include "LoRaMac_QH.h"
#include "csma.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef struct _CSMA_ {
		u32_t 					wAddrDev;
		u8_t 						byRelay;
		u8_t 						byStatus;
} CSMA_t, *CSMA_p;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static CSMA_t g_pCSMAHandle[MAX_CSMA] = { NULL };

static bool timeExpired = TRUE;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
//static csma_callback g_callback;
static u32_t g_AddrDev;
static u8_t g_Relay;
static u8_t g_Status;

static u8_t AddCSMAToBuff(
		u32_t wAddrDev,
		u8_t byRelay,
		u8_t byStatus
);

static bool_t CSMAExecute(void);

static void CSMACycle( void_p px );

static void CheckSend(void_p px);

static void Execute(void_p px);

static bool_t
CSMACancel(
    u8_t byCsmaId
);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                              __WEAK			                                  */
/******************************************************************************/
__weak void sendCMDToRelay(u32_t maThietBi, u8_t relayIndex, u8_t status)
{
		UNUSED(maThietBi);
		UNUSED(relayIndex);
		UNUSED(status);
}
/******************************************************************************/

static void CSMACycle(void_p px)
{
		timeExpired = TRUE;
	
		CSMAExecute();
}

/*
1. Add CMD to buffer
2. Check timeExpired?
	2.1. If no -> Step 4
	2.2. If yes -> Step 3
3. Try to execute CMD with CSMA/CA
4. timeExpired = FALSE;
*/
u8_t CSMAStart(
		u32_t wAddrDev,
		u8_t byRelay,
		u8_t byStatus
){
		u8_t CsmaId = AddCSMAToBuff(wAddrDev, byRelay, byStatus);
		
		if(timeExpired == TRUE)
		{
				CSMAExecute();
		}
    
    return CsmaId;
}

static bool_t CSMAExecute(void)
{
		for(u8_t i = 0; i < MAX_CSMA; i++)
		{
				if(g_pCSMAHandle[i].wAddrDev != 0)
				{
						timeExpired = FALSE;
				
						g_AddrDev  = g_pCSMAHandle[i].wAddrDev;
						g_Relay    = g_pCSMAHandle[i].byRelay;
						g_Status   = g_pCSMAHandle[i].byStatus;
					
						CSMACancel(i);
					
						Execute(NULL);
					
						return TRUE;
				}
		}
	
		return FALSE;
}

static void Execute(void_p px)
{
		sendCMDToRelay(g_AddrDev, g_Relay, g_Status);
			
		if(USE_CSMA) TimerStart(TIME_TO_CHECK_SEND, 1, CheckSend, NULL);
		else TimerStart(TIME_CSMA, 1, CSMACycle, NULL);
}

static void CheckSend(void_p px)
{
		if(TypeOfDataNow(dataType_DataUnconFirmUp, READ)) //Cho nay van bi ngu - Nho da nhan duoc phan hoi roi nhung truoc khi kiem tra, GW lai truyen di 1 ban tin khac thi se thanh sai
		{
				CSMACycle(NULL);
		}
		else
		{
				static u8_t resend = 0;
				if(resend < 2)
				{
						u16 timeToResend = random_number(MIN_TIME_TO_RESEND, MAX_TIME_TO_RESEND);
						TimerStart(timeToResend, 1, Execute, NULL);
						resend++;
				}
				else
				{
						CSMACycle(NULL);
						resend = 0;
				}
		}
}

static u8_t AddCSMAToBuff(
		u32_t wAddrDev,
		u8_t byRelay,
		u8_t byStatus
){
		for (int i = 0; i < MAX_CSMA; i++) 
    {
        if (g_pCSMAHandle[i].wAddrDev == 0)
        {
            g_pCSMAHandle[i].wAddrDev = wAddrDev;
						g_pCSMAHandle[i].byRelay = byRelay;
						g_pCSMAHandle[i].byStatus = byStatus;
						
            return i;
        }
    }
		
		return NO_CSMA;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
static bool_t
CSMACancel(
    u8_t byCsmaId
) {
    if ((byCsmaId >= MAX_CSMA) || (g_pCSMAHandle[byCsmaId].wAddrDev == 0))
        return FALSE;
    
    g_pCSMAHandle[byCsmaId].wAddrDev = 0;
    g_pCSMAHandle[byCsmaId].byRelay = 0;
    g_pCSMAHandle[byCsmaId].byStatus = 0;
    
    return TRUE;
}

