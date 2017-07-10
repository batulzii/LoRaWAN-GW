/******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author:	Quang Huy
 *
 * Last Changed By:  $Author:  Quang Huy
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     
								By Huy 		$Date: 2017-2-6 	11:45:00
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "board.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*!
 * Unique Devices IDs register set ( STM32L1xxx )
 */
#define         ID1                                 SIM->UIDMH
#define         ID2                                 SIM->UIDML
#define         ID3                                 SIM->UIDL
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
buffdat_t uartBuffTest;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
/*!
 * \brief Get the current battery level
 *
 * \retval value  battery level ( 0: very low, 254: fully charged )
 */
static void SX1276IoInit( void );
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void BoardInitMcu( void )
{
    SystemInit();
		SystemCoreClockUpdate();
		
		SysTick_Config(SystemCoreClock/1000);
	
		/*Khoi tao UART*/
		UART_Init(UART_DEBUG, BAUD9600, NO_PARITY, ONE_STOP_BIT);
		UART_RegBuffer(UART_DEBUG, BUFF_OVERRIDE, &uartBuffTest);
		UART_Open(UART_DEBUG);
		UART_SetRxIrq(UART_DEBUG, ENABLE);
		printf("Sys CLK: %d, Bus CLK: %d \r\n", SystemCoreClock, SystemBusClock);
		
		SPI_Init();
	
		i2c_init(I2C_DEBUG);
}

void BoardInitPeriph( void )
{
		/*Khoi tao GPIO*/
		GPIO_SetPinIsInput(BTN_CONFIG, 1);
		GPIO_SetPinIsOutput(LED_POWER);
		GPIO_SetPinIsOutput(LED_SEND);
		GPIO_SetPinIsOutput(LED_RECEIVE);
		GPIO_SetPinIsOutput(RELAY1);
		GPIO_SetPinIsOutput(RELAY2);
		GPIO_SetPinIsOutput(BUZZER);
	
		//Test LED 1, 2, 3
		TimerStart(0, 1, OnLedBoardInitPeriphEvent, NULL);
		TimerStart(100, 9, OnLedBoardInitPeriphEvent, NULL);
	
		/*Khoi tao pin SX1276*/
		SX1276IoInit();
}

void OnLedBoardInitPeriphEvent(void_p px)
{
		GPIO_PinHigh(LED_POWER);
		GPIO_PinToggle(LED_SEND);
		GPIO_PinToggle(LED_RECEIVE);	
}

void BoardGetUniqueId( uint8_t *id )
{
    id[7] = ((ID1+ID3)>>24)&0xFF;
    id[6] = ((ID1+ID3)>>16)&0xFF;
    id[5] = ((ID1+ID3)>>8)&0xFF;
    id[4] = ((ID1+ID3))&0xFF;
    id[3] = (ID2>>24)&0xFF;
    id[2] = (ID2>>16)&0xFF;
    id[1] = (ID2>>8)&0xFF;
    id[0] = (ID2)&0xFF;	
}

uint32_t BoardGetRandomSeed( void )
{
    return ID1 ^ ID2 ^ ID3;
}

uint8_t BoardGetBatteryLevel( void )
{
    return 1;
}

/*SX1276 on board*/
static void SX1276IoInit( void )
{
	GPIO_SetPinIsOutput(SX_RESET);
	GPIO_SetPinIsInput(SX_DIO0, 1);
}

void SX1276IoIrqInit( void )
{
	GPIO_SetPinIsInterrupt(SX_DIO0, EXTI_TRIGER_Rising_Edge);
}

uint8_t SX1276GetPaSelect( uint32_t channel )
{
    if( channel < RF_MID_BAND_THRESH )
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}
