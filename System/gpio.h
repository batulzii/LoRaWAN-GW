/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date:  $
 *
 ******************************************************************************/
#ifndef __GPIO_H__
#define __GPIO_H__ 
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
#define EXTI_TRIGER_Logic0							8
#define EXTI_TRIGER_Logic1							12
#define EXTI_TRIGER_Rising_Edge					9
#define EXTI_TRIGER_Falling_Edge				10
#define EXTI_TRIGER_Rising_Falling_Edge	11
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 * @func   
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
GPIO_SetPinIsOutput(
    u32_t dwPin
);

/**
 * @func   
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
GPIO_SetPinIsInput(
    u32_t dwPin,
    bool_t pullUp
);

void
GPIO_SetPinIsInterrupt(
    u32_t dwPin,
    u8 exti_triger
);

void
GPIO_SetPinDefault(
    u32_t dwPin
);
/**
 * @func   
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
GPIO_PinHigh (
    u32_t dwPin
);

/**
 * @func   
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
GPIO_PinLow (
    u32_t dwPin
);

/**
 * @func   
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
GPIO_PinToggle(
    u32_t dwPin
);
/**
 * @func   
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
bool_t
GPIO_PinReadInput (
    u32_t dwPin
);

#endif /* END FILE */
