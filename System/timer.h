/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: 
 *
 * Last Changed By:  $Author:  $
 * Revision:         $Revision:  $
 * Last Changed:     $Date: $
 *
 ******************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MAX_TIMER                   16
#define TIMER_ONE_TIME              0
#define TIMER_FOREVER               0xFF
#define NO_TIMER                    0xFF

typedef void (*timer_callback)(void_p);
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   TimerStart
 * @brief  None
 * @param  None
 * @retval None
 */

u8_t                            /* OUT: index of timer */
TimerStart(
    u32_t wMilSecTick,          /* IN: timer tick */
    u8_t byRepeats,             /* IN: number of repeater */
    timer_callback callback,    /* IN: callback function*/
    void_p pcallbackData        /* IN: parameters */
);

/**
 * @func   TimerRestart
 * @brief  None
 * @param  None
 * @retval None
 */

bool_t
TimerRestart(
    u8_t byTimerId,
    u32_t wMilSecTick,
    u8_t byRepeats
);
    
/**
 * @func   TimerCancel
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TimerCancel(
    u8_t byTimerId
);

/**
 * @func   GetMilSecTick
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
GetMilSecTick(void);

/**
 * @func   DelayMs
 * @brief  None
 * @param  None
 * @retval None
 */
void
DelayMs(
    u32_t dwMilSec
);

/**
 * @func   processTimer
 * @brief  None
 * @param  None
 * @retval None
 */
void
processTimer(void);

#endif /* _TIMER_H_ */
