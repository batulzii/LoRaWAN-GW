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
#ifndef __CSMA_H__
#define __CSMA_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MAX_CSMA                  16
#define NO_CSMA                   0xFF

#define TIME_CSMA									200 	//ms
#define TIME_TO_CHECK_SEND				195		//ms
#define MAX_TIME_TO_RESEND				100		//ms
/*
MIN_TIME_TO_RESEND Co y nghia: Nhung con chua dc gia nhap mang co the vao mang
trong khoang thoi gian nay
*/
#define MIN_TIME_TO_RESEND				0		//ms

#define USE_CSMA									TRUE

typedef void (*csma_callback)(u32_t wAddrDev, u8_t byRelay, u8_t byStatus);
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
u8_t CSMAStart(
		u32_t wAddrDev,
		u8_t byRelay,
		u8_t byStatus
);
#endif // __CSMA_H__
