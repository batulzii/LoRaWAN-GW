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
#ifndef __FLASH_H__
#define __FLASH_H__
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "macro.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define FLASH_SIZE                  32768
#define FLASH_PAGE_SIZE             1024

#define ENTER_CRITCAL         __disable_irq()
#define EXIT_CRITICAL         __enable_irq()
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

/**
 * @func   FLASH_Erasepage
 * @brief  None
 * @param  None
 * @retval None
 */ 
void
FLASH_Erasepage(
      u32_t dwAddress
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_WriteDword(
    u32_t dwAddr,
    u32_t dwData
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
FLASH_ReadDword(
    u32_t dwAddr
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
u16_t
FLASH_ReadWord(
    u32_t dwAddr
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_EraseNPage(
    u32_t dwAddrPageStart,
    u32_t dwSize
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_ReadNbyte(
    u32_t dwAddr,
    u8_p pByBuff,
    u32_t dwSize
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_WriteNbyte(
    u32_t dwAddr,
    u8_p pbyData,
    u16_t wSize
);

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
void
FLASH_Write4Byte(
    u32_t dwAddress,
    u8_p pbyData
);

void
FLASH_Read4Byte(
    u32_t dwAddr,
    u8_p pData
) ;
#endif /* END FILE */
