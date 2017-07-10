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
#ifndef __DRIVER_COMMON_H__
#define __DRIVER_COMMON_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
#include "MKL16Z4.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define NO_PIN_CONFIG           0xFFFFFFFFU
#define SHIFT_PORT              8u
#define MAKE_PIN(r,p)           (((r) << SHIFT_PORT) | (p))
#define EXTRACT_PORT(v)         ((v) >> SHIFT_PORT)
#define EXTRACT_PIN(v)          ((v) & 0xFFU)
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
#define GPIO_PORT_COUNT          5
#define GPIO_PIN_COUNT           32
#define SPI_COUNT                2
#define UART_COUNT               3
#define I2C_COUNT                2
#define TPM_COUNT                3
#define GPIOA_IDX                0
#define GPIOB_IDX                1
#define GPIOC_IDX                2
#define GPIOD_IDX                3
#define GPIOE_IDX                4

#define SPI0_IDX                 0
#define SPI1_IDX                 1

#define I2C0_IDX								 0
#define I2C1_IDX								 1

#define UART0_IDX                0
#define UART1_IDX                1
#define UART2_IDX                2

#define TPM0_IDX                 0
#define TPM1_IDX                 1
#define TPM2_IDX                 2


#define SCB_SCR       ((SCB)->SCR)
#define PMC_REGSC     ((PMC)->REGSC)


  
#define LLWU_PE1      ((LLWU)->PE1)
#define LLWU_PE2      ((LLWU)->PE2)
#define LLWU_PE3      ((LLWU)->PE3)
#define LLWU_PE4      ((LLWU)->PE4)
#define LLWU_F2       ((LLWU)->F2)
#define LLWU_F1       ((LLWU)->F1)
#define LLWU_ME       ((LLWU)->ME)
#define LLWU_FILT1    ((LLWU)->FILT1)
#define LLWU_FILT2    ((LLWU)->FILT2)

#define SMC_PMPROT                  ((SMC)->PMPROT)
#define SMC_PMCTRL                  ((SMC)->PMCTRL)
#define SMC_PMCTRL                  ((SMC)->PMCTRL)
#define SMC_STOPCTRL                ((SMC)->STOPCTRL)
#define SMC_PMSTAT                  ((SMC)->PMSTAT)

#define GPIO_CLK  { SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, \
                    SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, \
                    SIM_SCGC5_PORTE_MASK }

#define UART_CLK  { SIM_SCGC4_UART0_MASK, SIM_SCGC4_UART1_MASK, \
                    SIM_SCGC4_UART2_MASK }

#define I2C_CLK   { SIM_SCGC4_I2C0_MASK, SIM_SCGC4_I2C1_MASK }

#define SPI_CLK   { SIM_SCGC4_SPI0_MASK, SIM_SCGC4_SPI1_MASK }

#define TPM_CLK   { SIM_SCGC6_TPM0_MASK, SIM_SCGC6_TPM1_MASK, \
                    SIM_SCGC6_TPM2_MASK }



/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern u32_t const g_portclock[GPIO_PORT_COUNT];
extern u32_t const g_spiclock[SPI_COUNT];
extern u32_t const g_uartclock[UART_COUNT];
extern u32_t const g_i2cclock[I2C_COUNT];
extern u32_t const g_tpmclock[TPM_COUNT];

extern GPIO_Type* const g_gpiobase[GPIO_PORT_COUNT];
extern PORT_Type* const g_gpioport[GPIO_PORT_COUNT];
extern UART_Type * const g_uartbase[UART_COUNT];
extern I2C_Type * const g_i2cbase[I2C_COUNT];
extern SPI_Type * const g_spibase[SPI_COUNT];
extern TPM_Type * const g_tpmbase[TPM_COUNT];

extern uint32_t SystemBusClock;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

void ENTER_CRITCAL(void);
void EXIT_CRITICAL(void);
void delay_ms(u32_t ms);
void delay_us(u32_t us);
#endif /* __DRIVER_COMMON_H__ */
