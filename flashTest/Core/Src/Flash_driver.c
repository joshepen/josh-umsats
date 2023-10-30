#include "stm32l4xx.h"
HAL_StatusTypeDef Flash_Read(uint32_t *address,uint8_t *buf,size_t num_bytes){
	HAL_StatusTypeDef status = HAL_OK;

	if(!buf){
		status = HAL_ERROR;
		goto error;
	}
	uint32_t * address_pointer = (uint32_t *)  address;
	for(size_t i=0;i<num_bytes;i++){
		uint32_t num = 0x0;
		num = *(address_pointer);
		address_pointer += 0x1;
		buf[i] = num;
	}


error:
	return status;
}
HAL_StatusTypeDef Flash_Write(uint32_t start_address,uint8_t *buf,size_t num_bytes){
	HAL_StatusTypeDef status = HAL_OK;

		if(!buf){
			status = HAL_ERROR;
			goto error;
		}
		for(uint8_t addr=0x0;addr<(num_bytes + 8-1)/8;addr++){
			uint64_t word = 0x0;
			uint8_t letter = 0x0;
			uint8_t end_val;
			uint8_t size;
			if(addr == (num_bytes + 8-1)/8 - 1){
				size = num_bytes % 8;
			} else {
				size = 8;
			}
					for(uint8_t k=0;k<8-size;k++){
						letter = 0xFF;
						word = (word << 4 * 2);
						word += letter;
								}
					for(uint32_t i=8-size;i < 8;i++){
						letter = buf[addr * 8 + 8 - i - 1];
						word = (word << 4 * 2);
						word += letter;
						end_val = i;
					}

					HAL_FLASH_Unlock ();

					    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,start_address + addr * 8, word);
		}


		HAL_FLASH_Lock();

error:
	return status;
}
void Flash_Erase_Page(uint8_t page_num,uint8_t num_pages){
		HAL_FLASH_Unlock ();

	FLASH_EraseInitTypeDef FLASH_EraseInitStruct = {0};
	FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	FLASH_EraseInitStruct.Banks = FLASH_BANK_1;
	FLASH_EraseInitStruct.Page = page_num;
	FLASH_EraseInitStruct.NbPages = num_pages;
	uint32_t  errorStatus = 0;

	HAL_FLASHEx_Erase(&FLASH_EraseInitStruct,&errorStatus);

	HAL_FLASH_Lock();
}
