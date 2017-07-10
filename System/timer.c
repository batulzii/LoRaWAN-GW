/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  $Author: trungtq $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-17 11:45:00 (Tue, 17 May 2016) $
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "board.h"
#include "timer.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef TIMER_SYSTICK_DEBUG
#define DBG_TIMER_SEND_STR(x)         UART_DBG_SendStr(x)
#define DBG_TIMER_SEND_NUM(x)         UART_DBG_SendNum(x)
#define DBG_TIMER_SEND_HEX(x)         UART_DBG_SendHex(x)
#else
#define DBG_TIMER_SEND_STR(x)
#define DBG_TIMER_SEND_NUM(x)
#define DBG_TIMER_SEND_HEX(x)
#endif

typedef struct _TIMER_ {
    u32_t           milSecStart;
    u32_t           milSecTimeout;
    u8_t            repeats;
    timer_callback  callbackFunc;
    void_p          pCallbackData;
} TIMER_t, *TIMER_p;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static TIMER_t g_pTimerHandle[MAX_TIMER] = { NULL };
static volatile u32_t   g_wMilSecTickTimer = 0;
static volatile u32_t   g_dwMilSecDelayTimer = 0;
static volatile u32_t   g_wMilSecTimer = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static bool_t TimeExpired(u8_t byTimerId);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func
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
) {
    DBG_TIMER_SEND_STR("STAT timer\n");
   
    for (u8_t i = 0; i < MAX_TIMER; i++) 
    {
        if (g_pTimerHandle[i].callbackFunc == NULL)
        {
            g_pTimerHandle[i].callbackFunc = callback;
            g_pTimerHandle[i].repeats = byRepeats;
            g_pTimerHandle[i].pCallbackData = pcallbackData;
            g_pTimerHandle[i].milSecStart = GetMilSecTick();
            g_pTimerHandle[i].milSecTimeout = wMilSecTick;
            
            return i;
        }
    }
    
    return NO_TIMER;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TimerRestart(
    u8_t byTimerId,
    u32_t wMilSecTick,
    u8_t byRepeats
) {
    DBG_TIMER_SEND_STR("RST timer\n");
    
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL))
        return FALSE;
        
    g_pTimerHandle[byTimerId].repeats = byRepeats;
    g_pTimerHandle[byTimerId].milSecTimeout = wMilSecTick;
    g_pTimerHandle[byTimerId].milSecStart = GetMilSecTick();
    
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TimerCancel(
    u8_t byTimerId
) {
    DBG_TIMER_SEND_STR("CAN timer\n");
    
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL))
        return FALSE;
    
    g_pTimerHandle[byTimerId].callbackFunc = NULL;
    g_pTimerHandle[byTimerId].repeats = 0;
    g_pTimerHandle[byTimerId].milSecTimeout = 0;
    g_pTimerHandle[byTimerId].milSecStart = 0;
    
    return TRUE;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
GetMilSecTick() {
    return g_wMilSecTickTimer;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
DelayMs(
    u32_t dwMilSec
) {
    g_dwMilSecDelayTimer = dwMilSec;
    while (g_dwMilSecDelayTimer != 0);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
processTimer() {
    timer_callback callbackfunc;
    void_p pPrameter;
    
    for (u8_t i = 0; i < MAX_TIMER; i++) 
    {
        if ((g_pTimerHandle[i].callbackFunc != NULL) && TimeExpired(i))
        {
            callbackfunc = g_pTimerHandle[i].callbackFunc;
            pPrameter = g_pTimerHandle[i].pCallbackData;
            
            if ((g_pTimerHandle[i].repeats != TIMER_FOREVER) && \
                (g_pTimerHandle[i].repeats != 0)) 
                 g_pTimerHandle[i].repeats--;
            
            callbackfunc(pPrameter);
            
            if (g_pTimerHandle[i].repeats == 0)
                 g_pTimerHandle[i].callbackFunc = NULL;
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TimeExpired(
    u8_t byTimerId              /* IN: Timer identifier */
) {
    u32_t wMilSecTick;
    u32_t wDeltaMilSec = 0;
    
    ENTER_CRITCAL;
    wMilSecTick = GetMilSecTick(); 
    if ((byTimerId >= MAX_TIMER) || (g_pTimerHandle[byTimerId].callbackFunc == NULL)) 
    {
        EXIT_CRITICAL;
        return FALSE;
    }
      
    if (wMilSecTick >= g_pTimerHandle[byTimerId].milSecStart) 
    {
        wDeltaMilSec = wMilSecTick - g_pTimerHandle[byTimerId].milSecStart;
    } 
    else 
    {
        wDeltaMilSec = g_pTimerHandle[byTimerId].milSecStart - wMilSecTick;
        wDeltaMilSec = MAX_U32 - wDeltaMilSec;
    }
    
    if (wDeltaMilSec < g_pTimerHandle[byTimerId].milSecTimeout)
    {
        EXIT_CRITICAL;
        return FALSE;
    }
    
    g_pTimerHandle[byTimerId].milSecStart = wMilSecTick;
    
    EXIT_CRITICAL;

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void
SysTick_Handler() {
    g_wMilSecTickTimer++;
    if (g_dwMilSecDelayTimer != 0)
        g_dwMilSecDelayTimer--;
}
/* END FILE */
