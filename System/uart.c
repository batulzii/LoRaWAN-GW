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
 * Last Changed By:  $Author:  Huy$
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
#include "uart.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define UART_TIME_OUT               1000000
typedef struct _uart_ {
    u32 tx;
    u32 rx;
} uart_t, *uart_p;
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static void_p g_pUartBuffer[UART_COUNT] = { NULL };
static uart_t const g_uartArray[UART_COUNT] = { UART_ARR };
/*#define UART_ARR              { DRV_UART0_TX, DRV_UART0_RX }, \
                                { DRV_UART1_TX, DRV_UART1_RX }, \
                                { DRV_UART2_TX, DRV_UART2_RX }*/
static IRQn_Type g_uartIRQ[UART_COUNT] = { UART0_IRQn, UART1_IRQn, UART2_IRQn};
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void UART_DRIVER_IRQHandler(u8_t byUartNumber);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/**
 1. Ham nay dung de lam gi, tac dong toi he thong nhu the nao?
 */
void
UART_RegBuffer(
    u8_t byUartNumber,
    u8_t byOverRide,
    buffdat_p buf 
) 
{
    bufClear(buf);

    if (byOverRide == ENABLE) 
        buf->type = BUFF_OVERRIDE;
    else if (byOverRide == DISABLE)
        buf->type = BUFF_NOT_OVERRIDE;  
 
    g_pUartBuffer[byUartNumber] = (buffdat_p) buf;   
}

/**
 * @func   InitUart
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
/*
Cac cau hoi trong ham init()
1. Tai sao co cthuc tinh: dwSBR = (SystemBusClock >> 3) / dwBaudRate;?
2. Tai sao g_uartArray[byUartNumber].tx lai lay duoc DRV_UART0_TX;?
*/
void
UART_Init(
    u8_t byUartNumber,
    u32 dwBaudRate,
    u8 byParity,
    u8 byStopBit
) 
{
    u32_t dwSBR = 0;
    u32_t port;
    u32_t pin;

    if ((byUartNumber >= COUNT_UART) || !IS_STOP_BIT(byStopBit) || \
        !IS_BAUD(dwBaudRate) || !IS_PARITY(byParity))
        return ;

    /* UART Enable module clock */
    SIM->SCGC4 |= g_uartclock[byUartNumber];

    if (byUartNumber == 0) 
    {
        SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
				dwSBR = (SystemCoreClock)/(16*dwBaudRate);
			
        /* Config TX pin */    
        port = EXTRACT_PORT(g_uartArray[byUartNumber].tx);
        pin = EXTRACT_PIN(g_uartArray[byUartNumber].tx);
    
        SIM->SCGC5 |= g_portclock[port];
        g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(3);
    
        /* Config RX pin */
        port = EXTRACT_PORT(g_uartArray[byUartNumber].rx);
        pin = EXTRACT_PIN(g_uartArray[byUartNumber].rx);
    
        SIM->SCGC5 |= g_portclock[port];
        g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(3);

        /* Set Baud rate*/
        UART0->BDL = dwSBR & 0xFF;
        UART0->BDH = (dwSBR >> 8) & 0xFF;
        /* Set Mode 8 bit */
        UART0->C1 &= ~UART0_C1_M_MASK;
        /* Parity */
        if (NO_PARITY == byParity)
            UART0->C1 &= ~UART0_C1_PE_MASK;
        else 
        {
            UART0->C1 |= UART0_C1_PE_MASK;

            if (EVEN_PARITY == byParity)
                UART0->C1 &= ~UART0_C1_PT_MASK;
            else if (ODD_PARITY == byParity)
                UART0->C1 |= UART0_C1_PT_MASK;
        }
        /* Stopbit */
        if (ONE_STOP_BIT == byStopBit)
            UART0->BDH &= ~ UART0_BDH_SBNS_MASK;
        else if (TWO_STOP_BIT == byStopBit)
            UART0->BDH |= UART0_BDH_SBNS_MASK;
        /* Enable UART RX interrup */
//        UART0->C2 |= UART0_C2_RIE_MASK;
    }
    else 
    {
        dwSBR = (SystemBusClock)/(16*dwBaudRate);
        /* Config TX pin */    
        port = EXTRACT_PORT(g_uartArray[byUartNumber].tx);
        pin = EXTRACT_PIN(g_uartArray[byUartNumber].tx);
    
        SIM->SCGC5 |= g_portclock[port];
        g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(3);
    
        /* Config RX pin */
        port = EXTRACT_PORT(g_uartArray[byUartNumber].rx);
        pin = EXTRACT_PIN(g_uartArray[byUartNumber].rx);
    
        SIM->SCGC5 |= g_portclock[port];
        g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(3);

        /* Set UART baud rate */
        g_uartbase[byUartNumber]->BDL = dwSBR & 0xFF;
        g_uartbase[byUartNumber]->BDH = (dwSBR >> 8) & 0xFF; 
        /* Set Mode 8 bit */
        g_uartbase[byUartNumber]->C1 &= ~UART_C1_M_MASK;

        if (NO_PARITY == byParity)
            g_uartbase[byUartNumber]->C1 &= ~UART_C1_PE_MASK;
        else 
        {
            g_uartbase[byUartNumber]->C1 |= UART_C1_PE_MASK;
            if (EVEN_PARITY == byParity)
                g_uartbase[byUartNumber]->C1 &= ~UART_C1_PT_MASK;
            else if (ODD_PARITY == byParity)
                 g_uartbase[byUartNumber]->C1 |= UART_C1_PT_MASK;
        }

        if (ONE_STOP_BIT == byStopBit) 
        {
            g_uartbase[byUartNumber]->BDH &= ~ UART_BDH_SBNS_MASK;
        }
        else if (TWO_STOP_BIT == byStopBit) 
        {
             g_uartbase[byUartNumber]->BDH |= UART_BDH_SBNS_MASK;
        }
        /* Enable UART RX interrup */
//        g_uartbase[byUartNumber]->C2 |= UART_C2_RIE_MASK;
    }
    
    NVIC_EnableIRQ(g_uartIRQ[byUartNumber]);
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
UART_DeInit(
    u8_t byUartNumber
)
{
    u32_t port;
    u32_t pin;

    port = EXTRACT_PORT(g_uartArray[byUartNumber].tx);
    pin = EXTRACT_PIN(g_uartArray[byUartNumber].tx);
    g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(0);

    port = EXTRACT_PORT(g_uartArray[byUartNumber].rx);
    pin = EXTRACT_PIN(g_uartArray[byUartNumber].rx);
    g_gpioport[port]->PCR[pin] = PORT_PCR_MUX(0);

    /* UART unclock gate */
    SIM->SCGC4 &= ~g_uartclock[byUartNumber];
}

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
)
{
    if (byUartNumber >= COUNT_UART)
        return ;

    else if (byUartNumber == 0) 
    {
        if (bState)
            UART0->C2 |= UART0_C2_TE_MASK;
        else
            UART0->C2 &= ~UART0_C2_TE_MASK;
    }
    else 
    {
        if (bState)
            g_uartbase[byUartNumber]->C2 |= UART_C2_TE_MASK;
        else
            g_uartbase[byUartNumber]->C2 &= ~UART_C2_TE_MASK;
    }
}

/**
 * @func   SetReciver
 *
 * @brief  None
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
) 
{
    if (byUartNumber >= COUNT_UART)
        return ;

    else if (byUartNumber == 0) 
    {
        if (bState)
            UART0->C2 |= UART0_C2_RE_MASK;
        else
            UART0->C2 &= ~UART0_C2_RE_MASK;
    }
    else 
    {
        if (bState)
            g_uartbase[byUartNumber]->C2 |= UART_C2_RE_MASK;
        else
            g_uartbase[byUartNumber]->C2 &= ~UART_C2_RE_MASK;
    }

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
UART_SetRxIrq(
    u8_t byUartNumber,
    bool_t bState
) 
{
    if (byUartNumber >= COUNT_UART)
        return ;

    else if (byUartNumber == 0) 
    {
        if (bState)
            UART0->C2 |= UART0_C2_RIE_MASK;
        else
            UART0->C2 &= ~UART0_C2_RIE_MASK;
    }
    else 
    {
        if (bState)
            g_uartbase[byUartNumber]->C2 |= UART_C2_RIE_MASK;
        else
            g_uartbase[byUartNumber]->C2 &= ~UART_C2_RIE_MASK;
    }
}

/**
 * @func   UartOpen
 *
 * @brief  None
 *
 * @param  Enable Transmit and reeciver
 *
 * @retval None
 */
void
UART_Open(
    u8_t byUartNumber
) 
{
    if (byUartNumber >= COUNT_UART)  return ;

    UART_SetTransmitter(byUartNumber, TRUE);
    UART_SetReciver(byUartNumber, TRUE);
}

/**
 * @func   UartClose
 *
 * @brief  None
 *
 * @param     Disable Transmit and reeciver
 *
 * @retval None
 */
void
UART_Close(
    u8_t byUartNumber
) 
{
    if (byUartNumber >= COUNT_UART)
        return ;

    UART_SetTransmitter(byUartNumber, FALSE);
    UART_SetReciver(byUartNumber, FALSE);
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
u8_t
UART_GetChar(
    u8_t byUartNumber
) 
{
    if (byUartNumber == 0) 
    {
        while(!(UART0->S1 & UART0_S1_RDRF_MASK));
        /* Read the character */
        return UART0->D;
    }

    while(!(g_uartbase[byUartNumber]->S1 & UART_S1_RDRF_MASK));
    /* Read the character */
    return g_uartbase[byUartNumber]->D;	
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
UART_PutChar(
    u8_t byUartNumber,
    u8_t byData
) 
{
    if (byUartNumber >= COUNT_UART)  return ;
   
    if (byUartNumber == 0) 
    {
        /* Wait until space is available in the FIFO */
        while(!(UART0->S1 & UART0_S1_TDRE_MASK));
        /* Send the character */
        UART0->D = byData;

        while(!(UART0->S1 & UART0_S1_TC_MASK));//wait for transmission complete

        return ;
    }

    /* Wait until space is available in the FIFO */
    while(!(g_uartbase[byUartNumber]->S1 & UART_S1_TDRE_MASK));
    /* Send the character */
    g_uartbase[byUartNumber]->D = byData;

    while(!(g_uartbase[byUartNumber]->S1 & UART_S1_TC_MASK));
}

/**
 * @func   UartPutBuffer
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
/*Chua hieu ham nay dung de lam gi? Nhin thi giong ham putString*/
void
UART_PutBuffer(
    u8_t byUartNumber,
    u8_p pbyDataOut,
    u16_t byLength
) 
{
    u16_t i;

    for (i = 0; i < byLength; i++)
        UART_PutChar(byUartNumber, pbyDataOut[i]);
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
UART_PutString(
    u8_t byUartNumber,
    char_p pStringOut
) 
{

    char_p pNextChar = pStringOut;
    while (*pNextChar != NULL) 
    {
        UART_PutChar(byUartNumber, *pNextChar);
        pNextChar++;
    }
}

 /**
 * @func   UART0_IRQHandler
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
UART0_IRQHandler()
{
    UART_DRIVER_IRQHandler(UART0_IDX); 
}

 /**
 * @func   UART2_IRQHandler
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
UART2_IRQHandler()
{
    UART_DRIVER_IRQHandler(UART2_IDX); 
}

 /**
 * @func   UART2_IRQHandler
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
void
UART1_IRQHandler()
{
    ENTER_CRITCAL;
    /* Process Reciever */
    if ((g_uartbase[1]->C2 & UART_C2_RIE_MASK) && \
        (g_uartbase[1]->S1 & UART_S1_RDRF_MASK)) 
    {
				u8_t byReceiverData = UART_GetChar(1); 
				switch(byReceiverData)
				{
					case 'r': TimerStart(0, 1, sendNbCMDToPC, NULL);
						break;
					case 'e': TimerStart(0, 1, resetNbCMDInFlash, NULL);
						break;
					default:
						break;
				}
    }
    EXIT_CRITICAL;
}

/**
 * @func   UART_DRIVER_IRQHandler
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
/*
Chua hieu hai dong nay co y nghia gi?
buffdat_p pUartBuff = (buffdat_p) g_pUartBuffer[byUartNumber];
bufAddDat(byReceiverData, pUartBuff);     
*/
void
UART_DRIVER_IRQHandler(
    u8_t byUartNumber
) {
		ENTER_CRITCAL;
    /* Process Reciever */
    if ((g_uartbase[byUartNumber]->C2 & UART_C2_RIE_MASK) && \
        (g_uartbase[byUartNumber]->S1 & UART_S1_RDRF_MASK)) 
    {
          
         buffdat_p pUartBuff = (buffdat_p) g_pUartBuffer[byUartNumber];
         u8_t byReceiverData = UART_GetChar(byUartNumber);        
         bufAddDat(byReceiverData, pUartBuff);   
      
    }
    EXIT_CRITICAL;
}

#ifdef USE_DEBUG
/**
 * @func   retarget printf
 *
 * @brief  None
 *
 * @param  None
 *
 * @retval None
 */
struct __FILE
{
  int handle;
};

FILE __stdout;  //Use with printf
FILE __stdin;  //use with fget/sscanf, or scanf
//Retarget the fputc method to use the UART1
int 
fputc(
    int ch, 
    FILE *f
)
{
    /* Place your implementation of fputc here */

    UART_PutChar(UART_DEBUG,(u8)ch);

    return ch;
}

#endif

/* END FILE */

