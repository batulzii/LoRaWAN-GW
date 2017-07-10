/******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author:	ThangDH
 *
 * Last Changed By:  $Author:  Huy-Hoang$
 * Revision:         $Revision: 1.0.0.1 $
 * Last Changed:     
								By Huy 		$Date: 2017-2-6 	11:45:00
								By Hoang 	$Date: 2017-05-16 11:45:00
 *
 ******************************************************************************/
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "driver_common.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
u32_t const g_portclock[GPIO_PORT_COUNT] = GPIO_CLK;
u32_t const g_spiclock[SPI_COUNT] = SPI_CLK;
u32_t const g_uartclock[UART_COUNT] = UART_CLK;
u32_t const g_i2cclock[I2C_COUNT] =  I2C_CLK;
u32_t const g_tpmclock[TPM_COUNT] = TPM_CLK;

GPIO_Type * const g_gpiobase[GPIO_PORT_COUNT] = GPIO_BASES;
PORT_Type * const g_gpioport[GPIO_PORT_COUNT] = PORT_BASES;
SPI_Type * const g_spibase[SPI_COUNT] = SPI_BASES;
UART_Type * const g_uartbase[UART_COUNT] = { 0 , UART1, UART2};
I2C_Type * const g_i2cbase[I2C_COUNT] = I2C_BASES;
TPM_Type * const g_tpmbase[TPM_COUNT] = TPM_BASES;
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
void 
ENTER_CRITCAL(void) {
    __disable_irq();
}

void EXIT_CRITICAL(void) {
    __enable_irq();
}

void delay_ms(u32_t ms) {
    u32_t i;
    u32_t fms = (SystemCoreClock >> 12) * ms;
    for (i = 0; i < fms; i++) {}
}

void delay_us(u32_t us) {
    u32_t i;
    u32_t fms = (SystemCoreClock >> 22) * us;
    for (i = 0; i < fms; i++) {}
}

/* END FILE */
