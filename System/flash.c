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
 
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "board.h"
#include "flash.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
//#define FLASH_TIME_OUT               100000000

enum FCMD {
    Read1s = 0x01,
    ProgramCheck = 0x02,
    ReadResource = 0x03,
    ProgramLongword = 0x06,
    EraseSector = 0x09,
    Read1sBlock = 0x40,
    ReadOnce = 0x41,
    ProgramOnce = 0x43,
    EraseAll = 0x44,
    VerifyBackdoor = 0x45
};
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
 * @brief  Xoa bo nho flash o 24 bit dia chi dwAddress
* @param   dwAddress: dia chi bo nho flash can xoa
 * @retval None
 */ 
void
FLASH_Erasepage(
      u32_t dwAddress
) {

    if((dwAddress & 0x1FF) != 0) return; 

    ENTER_CRITCAL;

    while (((FTFA->FSTAT) & (1UL << 7)) == 0x00){}  
   
		//Xoa flag bang cach ghi 1 vao chung
    FTFA->FSTAT = FTFA_FSTAT_FPVIOL_MASK | FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_RDCOLERR_MASK;

    /* Setup command */
		/*Byte dau tien dung de dinh nghia flash comment
			3 byte tiep theo la flash address
			8 byte con lai la databyte
			Xem page 444 datasheet*/
    FTFA->FCCOB0 = EraseSector;
    FTFA->FCCOB1 = GET_BIT_16_23(dwAddress);
    FTFA->FCCOB2 = GET_BIT_8_15(dwAddress);
    FTFA->FCCOB3 = GET_BIT_0_7(dwAddress);
		
		/*Cho phep lenh bat dau thuc thi. Xem phan thanh ghi FCCOBn*/
    FTFA->FSTAT = FTFA_FSTAT_CCIF_MASK;
			
    /* Wait for completed command */
    while (!(FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK));
  
    EXIT_CRITICAL;
}

/**
 * @func   ErasePage
 * @brief  None
 * @param  
				dwAddrPageStart: dia chi cua page dau tien can xoa
				dwSize: 
 * @retval None
 */ 
void
FLASH_EraseNPage(
    u32_t dwAddrPageStart,
    u32_t dwSize
) {	
    u8_t byNumOfPage = 0;  
    u8_t i; 
    u32_t dwAddr = dwAddrPageStart;

    byNumOfPage = dwSize >> 10;
    if (dwSize > (byNumOfPage << 10))  byNumOfPage++;

    for (i = 0; i < byNumOfPage; i++) {
        FLASH_Erasepage(dwAddr);
        dwAddr += FLASH_PAGE_SIZE;
    }
}

/**
 * @func   FLASH_Write4Byte
 * @brief  Viet 4 byte pbyData vao dia chi dwAddress
 * @param  
					dwAddress: Dia chi bo nho flash can ghi du lieu
					pbyData: Du lieu can ghi 
 * @retval None
 */ 
void
FLASH_Write4Byte(
    u32_t dwAddress,
    u8_p pbyData
) {
    /* Check address */
    if ((dwAddress&0x3) != 0) return; 

    ENTER_CRITCAL;
		
		/*Cho lenh truoc do duoc thuc hien xong*/
    while (((FTFA->FSTAT)&(1UL << 7)) == 0x00){} 
		
			/*Neu da thuc hien xong lenh truoc do thi xoa co ACCERR va FPVIOL*/
    if (!((FTFA->FSTAT)==0x80))     { FTFA->FSTAT = 0x30; }
		
    /* Select command */
    FTFA->FCCOB0 = ProgramLongword;
		
    /* Write address + data */
    FTFA->FCCOB1 = GET_BIT_16_23(dwAddress);
    FTFA->FCCOB2 = GET_BIT_8_15(dwAddress);
    FTFA->FCCOB3 = GET_BIT_0_7(dwAddress);

    FTFA->FCCOB4 = pbyData[3];
    FTFA->FCCOB5 = pbyData[2];
    FTFA->FCCOB6 = pbyData[1];
    FTFA->FCCOB7 = pbyData[0];
		
    /* Start command */
    FTFA->FSTAT = 0x80;
		
    /* Wait for completed */
    while ((((FTFA->FSTAT)&(1UL << 7))==0x00)) {}

    EXIT_CRITICAL;
}

/**
 * @func   FLASH_WriteDword
 * @brief  Khong khac ham FLASH_Write4Byte la may, khac o day la du lieu dau vao u32_t thay vi u8_p nhu o FLASH_Write4Byte
 * @param  None
 * @retval None
 */ 
void
FLASH_WriteDword(
    u32_t dwAddr,
    u32_t dwData
) {
    u8_t pbuff[4];

    pbuff[0] = GET_BIT_0_7(dwData);
    pbuff[1] = GET_BIT_8_15(dwData);
    pbuff[2] = GET_BIT_16_23(dwData);
    pbuff[3] = GET_BIT_24_31(dwData);
    FLASH_Write4Byte(dwAddr,pbuff);
 }

/**
 * @func   ReadFlashDWord
 * @brief  None
 * @param  None
 * @retval None
 */ 
u32_t
FLASH_ReadDword(
    u32_t dwAddr
) {
    return READ32(dwAddr);	//Tra ve gia tri cua o nho co dia chi dwAddr, 4byte do dia chi kia la 4 byte
}

/**
 * @func 
 * @brief  Tra ve gia tri 4 o nho lien tiep bat dau tu o co dia chi dwAddr
 * @param  None
 * @retval None
 */ 
void
FLASH_Read4Byte(
    u32_t dwAddr,
    u8_p pData
) {
        u8_t i;
        u32_t dwAddreess = dwAddr;
        for(i =0; i<4; i++) {
           pData[i] = READ8(dwAddreess);
           dwAddreess++;
        }
}

/**
 * @func   ReadFlashNByte
 * @brief  None
 * @param  None
 * @retval None
 */ 
void
FLASH_ReadNbyte(
    u32_t dwAddr,
    u8_p pByBuff,
    u32_t dwSize
) {
    u16_t bySizeDiv4;
    u32_t dwTemp;	
    u8_t i,j;

    bySizeDiv4 = dwSize>>2;
    for (i = 0; i < bySizeDiv4; i++){
        dwTemp = FLASH_ReadDword(dwAddr + (i<<2));
        for (j = 0; j < 4; j++) {
            *pByBuff = dwTemp >> (j<<3);
            pByBuff++;
        }
    }	
}

/**
 * @func   ProgramFlashNByte
 * @brief  None
 * @param  None
 * @retval None
 */ 
void
FLASH_WriteNbyte(
    u32_t dwAddr,
    u8_p pbyData,
    u16_t wSize
) {
    u32_t dwAddress = dwAddr;
    u32_t dwData;
    u8_t i;
	
    /* Size = k*4 byte */
    wSize = wSize >> 2;

    while (wSize > 0) {
       dwData = 0;
       for (i = 0; i < 4; i++) {
           dwData |= *pbyData << (i<<3); //Can dich (i<<3) de dwData tro thanh 4byte.
           pbyData++;    
       } 
     
       FLASH_WriteDword(dwAddress,dwData);       
       wSize --;
       dwAddress += 4;
    }
}

/**
 * @func 
 * @brief  None
 * @param  None
 * @retval Return size flash in chip
 */ 
u32_t 
FLASH_Size(void) {
    u32_t retval ;

    retval = (SIM->FCFG2 & 0x7F000000u) >> (24-13);
    if (SIM->FCFG2 & (1<<23))        //Possible second flash bank
        retval += (SIM->FCFG2 & 0x007F0000u) >> (16-13);

    return retval;
}
/* END FILE */
