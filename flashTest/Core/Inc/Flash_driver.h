/*
 * Flash_driver.h
 *
 *  Created on: Apr. 5, 2023
 *      Author: JoshPe
 */
#include "stm32l4xx.h"
#ifndef HARDWARE_PERIPHERALS_INC_FLASH_DRIVER_H_
#define HARDWARE_PERIPHERALS_INC_FLASH_DRIVER_H_

HAL_StatusTypeDef Flash_Read(uint32_t address,uint8_t *buf,size_t num_chars);
HAL_StatusTypeDef Flash_Write(uint32_t address,uint8_t *buf,size_t num_chars);
HAL_StatusTypeDef Flash_Erase_Page(uint8_t page_num,uint8_t num_pages);
uint32_t* Flash_Get_Free_Address();
uint32_t Flash_Page_To_Address(uint8_t page_num);
uint8_t Flash_Address_To_Page(uint32_t address);


#endif /* HARDWARE_PERIPHERALS_INC_FLASH_DRIVER_H_ */
