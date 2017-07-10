/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: TrungTQ
 *
 * Last Changed By:  ThangDH $
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     $Date: 2016-05-19 9:30:00 (Thu, 19 May 2016) $
 *
 ******************************************************************************/
#ifndef __UART_H__
#define __UART_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "buff.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#ifdef USE_DEBUG
#define DEBUG_I2C_CMD
#define DEDUG_I2C_DRV
#endif /* DEBUG */

#ifndef DRV_UART1_IDX
#define DRV_UART0_IDX           0
#define DRV_UART1_IDX           1
#define DRV_UART2_IDX           2
#endif /* DRV_UART1_IDX */

#define COUNT_UART              3

#ifdef USE_QUICK_BUFFER
#define TX_BUFFER_SIZE          128 /* must 2^N */
//#define RX_BUFFER_SIZE          128
#else /* USE_QUICK_BUFFER */
#define TX_BUFFER_SIZE          100
//#define RX_BUFFER_SIZE          100
#endif /* USE_QUICK_BUFFER */

#define UART_ARR                { DRV_UART0_TX, DRV_UART0_RX }, \
                                { DRV_UART1_TX, DRV_UART1_RX }, \
                                { DRV_UART2_TX, DRV_UART2_RX }

#define BAUD9600                9600
#define BAUD19200               14400
#define BAUD38400               38400
#define BAUD57600               57600
#define BAUD115200              115200
#define IS_BAUD(x)              (((x) == BAUD9600) || ((x) == BAUD19200) || \
                                 ((x) == BAUD38400) || ((x) == BAUD57600) || \
                                 ((x) == BAUD115200))

#define NO_PARITY               0
#define EVEN_PARITY             1
#define ODD_PARITY              2
#define IS_PARITY(x)            (((x) == NO_PARITY) || ((x) == EVEN_PARITY) || \
                                 ((x) == ODD_PARITY))

#define ONE_STOP_BIT             0
#define TWO_STOP_BIT             1
#define IS_STOP_BIT(x)           (((x) == ONE_STOP_BIT) || ((x) == TWO_STOP_BIT))

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
 * @func   RegisterBuffer
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void
UART_RegBuffer(
    u8_t byUartNumber,
    u8_t byOverRide,
    buffdat_p buf 
);

/**
 * @func   InitUart
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void
UART_Init(
    u8_t byUartNumber,
    u32_t dwBaudRate,
    u8_t byParity,
    u8_t byStopBit
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
UART_DeInit(
    u8_t byUartNumber
);

/**
 * @func   SetTransmitter
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void
UART_SetTransmitter(
    u8_t byUartNumber,
    bool_t bState   /* IN: TRUE: Enble */
                   /* FALSE: Disable */
);

/**
 * @func   SetReciver
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void
UART_SetReciver(
    u8_t byUartNumber,
    bool_t bState    /* IN: TRUE: Enble */
                     /* FALSE: Disable */
);

/**
 * @func   EnableIrqUart
 *
 * @brief  None  
 *
 * @param  None
 *
 * @retval None
 */
void
UART_SetRxIrq(
    u8_t byUartNumber,
    bool_t bState
);

/**
 * @func   UartOpen
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
UART_Open(
    u8_t byUartNumber
);

/**
 * @func   UartClose
 *
 * @brief  None
 *
 * @param  
 *
 * @retval None
 */
void
UART_Close(
    u8_t byUartNumber
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
u8_t
UART_GetChar(
    u8_t byUartNumber
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
UART_PutChar(
    u8_t byUartNumber,
    u8_t byData
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
UART_PutString(
    u8_t byUartNumber,
    char_p pStringOut
);

/**
 * @func   UartPutBuffer
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
UART_PutBuffer(
    u8_t byUartNumber,
    u8_p pbyDataOut,
    u16_t byLength
);

#ifdef USE_DEBUG
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
DEBUGPUTBUF(
    u8_p pbuff,
    u16_t wlength		
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
DEBUGPUTBUFCH(
    u8_p pbuff,
    u16_t wlength
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
binaryPrintf(
    u16_t wValue
);
#endif

#endif /* _UART_H_ */
