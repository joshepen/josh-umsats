#include "stm32l4xx.h"
HAL_StatusTypeDef Flash_Read(uint32_t address,uint8_t *buf,size_t num_bytes){
	HAL_StatusTypeDef status = HAL_OK;

	if(!buf){
		status = HAL_ERROR;
		goto error;
	}
	int x;
	uint32_t * address_pointer = (uint32_t *)  address;
	for(size_t i=0;i<num_bytes;i++){
		uint32_t num = 0x0;
		for(size_t j=0;j<8;j+=2){
			num=address_pointer[i*8+j]%0x100;
			buf[i * 4 + j/2] = num;
		}
		//for (uint32_t k=0;k<4;k++){
			//buf[i * 4 + k] = num / (int) pow(0x100,k) % 0x100;
		//}
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
		HAL_FLASH_Unlock ();

		for(uint32_t i=0;i<num_bytes*4;i++){
		    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,start_address+ 8 * i, buf[i]);
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
void Flash_Erase_Address(uint8_t address,size_t num_bytes){

	//store current contents of page
	uint8_t start_page = (address - 0x8000000) / 0x800;
	uint8_t *start_address = start_page * 0x800 + 0x8000000;
	uint8_t end_page = (address + num_bytes - 0x8000000) / 0x800;
	uint32_t temp[0x800];
	uint8_t index_temp = 0x0;
	for(uint8_t i = 0;i<0x800;i++){
			temp[index_temp] = *(start_address + i);
			index_temp++;
	}

	//erase page
	Flash_Erase_Page(start_page,end_page - start_page + 0x1);

	//rewrite contents
	index_temp = 0;
	for(uint32_t i = start_address;i<start_address+0x800;i++){
		if(i < address || i > address+num_bytes){
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,i,temp[index_temp]);
			index_temp++;
		}
	}

	HAL_FLASH_Lock();
}
