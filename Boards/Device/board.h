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
#ifndef __BOARD_H__
#define __BOARD_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "buff.h"
#include "driver_common.h"
#include "typedefs.h"
#include "utilities.h"
#include "comissioning.h"
#include "gpio.h"
#include "uart.h"
#include "flash.h"
#include "timer.h"
#include "spi_gpio.h"
#include "gpio_board.h"
#include "debug.h"
#include "csma.h"
#include "sx1276_QH.h"
#include "mkl_si7020.h"
#include "opt3001.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/* Define pin GPIO */
#define LED_POWER                    	MAKE_PIN(GPIOA_IDX, 18u)
#define LED_SEND                    	MAKE_PIN(GPIOA_IDX, 19u)
#define LED_RECEIVE                   MAKE_PIN(GPIOC_IDX, 1u)

#define RELAY1                    		MAKE_PIN(GPIOA_IDX, 1u)
#define RELAY2                    		MAKE_PIN(GPIOA_IDX, 2u)

/*Define pin Button*/
#define BTN_CONFIG										MAKE_PIN(GPIOC_IDX, 5u)

/**/
#define BUZZER												MAKE_PIN(GPIOC_IDX, 2u)

/*Define pin SX1276*/
#define SX_RESET											MAKE_PIN(GPIOC_IDX, 4u)
#define SX_DIO0												MAKE_PIN(GPIOC_IDX, 3u)

/* Define pin uart */
#define DRV_UART0_TX            			MAKE_PIN(GPIOD_IDX, 7u)
#define DRV_UART0_RX           				MAKE_PIN(GPIOD_IDX, 6u)            

#define DRV_UART1_TX            			MAKE_PIN(GPIOE_IDX, 0u)
#define DRV_UART1_RX            			MAKE_PIN(GPIOE_IDX, 1u)

#define DRV_UART2_TX            			MAKE_PIN(GPIOD_IDX, 5u)
#define DRV_UART2_RX            			MAKE_PIN(GPIOD_IDX, 4u)

/*Define pin I2C*/
#define DRV_I2C0_SCL									MAKE_PIN(GPIOB_IDX, 0u)
#define DRV_I2C0_SDA									MAKE_PIN(GPIOB_IDX, 1u)

#define DRV_I2C1_SCL									MAKE_PIN(GPIOE_IDX, 1u)
#define DRV_I2C1_SDA									MAKE_PIN(GPIOE_IDX, 0u)


/*Define pin SPI*/
#define DRV_SPI0_NSS									MAKE_PIN(GPIOE_IDX, 16u)
#define DRV_SPI0_SCK									MAKE_PIN(GPIOE_IDX, 17u)
#define DRV_SPI0_MOSI									MAKE_PIN(GPIOE_IDX, 18u)
#define DRV_SPI0_MISO									MAKE_PIN(GPIOE_IDX, 19u)

/* Debug */
#define UART_DEBUG              			1
#define I2C_DEBUG											0
#define SPI_DEBUG											0
#define TIMER_0												0
#define TIMER_1												1

/*!
 * Generic definition
 */
#ifndef SUCCESS
#define SUCCESS                       1
#endif

#ifndef FAIL
#define FAIL                          0
#endif

#ifndef USE_DEBUG
#define USE_DEBUG											TRUE
#endif	//USE_DEBUG

#define RF_MID_BAND_THRESH             525000000

#define timeCycle											1 //seconds
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
/*!
 * \brief Initializes the target board peripherals.
 */
void BoardInitMcu( void );

void BoardInitPeriph( void );

/*!
 * \brief 
 */
void BoardGetUniqueId( uint8_t *id );

/*!
 * Returns a pseudo random seed generated using the MCU Unique ID
 *
 * \retval seed Generated pseudo random seed
 */
uint32_t BoardGetRandomSeed( void );

/*!
 * \brief Get the current battery level
 *
 * \retval value  battery level ( 0: very low, 254: fully charged )
 */
uint8_t BoardGetBatteryLevel( void );

/*!
 * \brief Get the current battery level
 *
 * \retval value  battery level ( 0: very low, 254: fully charged )
 */
void SX1276IoIrqInit( void );

/*!
 * \brief Get the current battery level
 *
 * \retval value  battery level ( 0: very low, 254: fully charged )
 */
uint8_t SX1276GetPaSelect( uint32_t channel );

/*!
 * \brief Change LED status on Boards
 *
 * \retval None
 */
void OnLedBoardInitPeriphEvent(void_p px);

#endif // __BOARD_H__
