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
#include "board.h"
#include "gpio.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
static IRQn_Type g_gpioIRQ[2] = { PORTA_IRQn, PORTC_PORTD_IRQn};
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
) {
     u32_t pin;
     u32_t port;

     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;

     SIM->SCGC5 |= g_portclock[port];
     g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(1);
     g_gpiobase[port]->PDDR |= 1 << pin;
}

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
) {
     u32_t pin;
     u32_t port;

     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;

     SIM->SCGC5 |= g_portclock[port];
     /* PIN AS GPIO */
     g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(1);
     /* PIN IS INPUT */
     g_gpiobase[port]->PDDR &= ~ (1 << pin);
     /* ENABLE PULL */
     g_gpioport[port]->PCR[pin] |= PORT_PCR_PE_MASK;
     /* SELECT PULL */
     if (pullUp)  g_gpioport[port]->PCR[pin] |= PORT_PCR_PS_MASK;
     else g_gpioport[port]->PCR[pin] &= ~PORT_PCR_PS_MASK;
	
}

/**
 * @func   GPIO_SetPinIsInterrupt
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
GPIO_SetPinIsInterrupt(
    u32_t dwPin,
    u8 exti_triger
) {
     u32_t pin;
     u32_t port;

     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

		 g_gpioport[port]->PCR[pin] |= PORT_PCR_IRQC(exti_triger);
	
		 if(g_gpioport[port]==PORTA) NVIC_EnableIRQ(g_gpioIRQ[0]);
		 else if((g_gpioport[port]==PORTC)|(g_gpioport[port]==PORTD)) NVIC_EnableIRQ(g_gpioIRQ[1]);
}

/**
 * @func   PORTC_PORTD_IRQHandler
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void PORTC_PORTD_IRQHandler()
{
	if(((PORTC->ISFR)&0x08)== 0x08)
	{	
			SX1276OnDio0Irq();
	}
	//Xoa co ngat
	PORTC->ISFR |= 0xFF;
}

/**
 * @func   PORTA_IRQHandler
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void PORTA_IRQHandler()
{
	
}

/**
 * @func   GPIO_SetPinDefault
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
GPIO_SetPinDefault(
    u32_t dwPin
) {
     u32_t pin;
     u32_t port;

     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;

     g_gpiobase[port]->PCOR = (1 << pin);
     g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(0);
}
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
) {
     u32_t pin;
     u32_t port;

     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return; 
     g_gpiobase[port]->PSOR = (1 << pin);
}

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
) {
     u32_t pin;
     u32_t port;

     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;
     g_gpiobase[port]->PCOR = (1 << pin);
}

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
) {
     u32_t pin;
     u32_t port;

     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

     if ((pin >= GPIO_PIN_COUNT) | (port >= GPIO_PORT_COUNT)) return;
     g_gpiobase[port]->PTOR = (1 << pin);
}
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
) {
     u32_t pin;
     u32_t port;
     bool_t bState = TRUE;    
 
     port = EXTRACT_PORT(dwPin);
     pin = EXTRACT_PIN(dwPin);

     if(g_gpiobase[port]->PDIR & (1 << pin)) bState = TRUE ;
     else bState = FALSE;

     return bState;
}


/* END FILE */
