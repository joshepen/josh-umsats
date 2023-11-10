#include "stm32l4xx.h"
HAL_StatusTypeDef Flash_Read(uint32_t *address,uint8_t *buf,size_t num_chars){
	HAL_StatusTypeDef status = HAL_OK;

	if(!buf){
		status = HAL_ERROR;
		goto error;
	}
	size_t num_addresses = (num_chars + 8-1)/8;
	uint32_t num;
	uint32_t * address_pointer = (uint32_t *)  address;
	for(size_t i=0;i<num_addresses;i++){
		num = *(address_pointer);
		address_pointer += 0x1;
		for(int j=4;j<8;j++){
			buf[i*8 + j] = (num >> (8 * (7-j))) & 0xff;
		}
		num = *(address_pointer);
		address_pointer += 0x1;
		for(int j=0;j<4;j++){
			buf[i*8 + j] = (num >> (8 * (3-j))) & 0xff;
		}

	}


error:
	return status;
}
HAL_StatusTypeDef Flash_Write(uint32_t start_address,uint8_t *buf,size_t num_chars){ //writes 8 bytes at a time
	HAL_StatusTypeDef status = HAL_OK;

		if(!buf){
			status = HAL_ERROR;
			goto error;
		}
		uint64_t word;
		uint8_t letter;
		uint8_t size;
		for(uint8_t addr=0x0;addr<(num_chars+ 8-1)/8;addr++){
			word = 0x0;
			letter = 0x0;
			if(addr == (num_chars + 8-1)/8 - 1){ //if address is the last one, ceiling'd to next multiple of 8
				size = num_chars % 8;
			} else {
				size = 8;
			}
					for(uint32_t i=0;i < size;i++){
						letter = buf[addr * 8 + i];
						word = (word << 4 * 2);
						word += letter;
					}
					for(uint8_t k=0;k<8-size;k++){
						letter = 0xFF;
						word = (word << 4 * 2);
						word += letter;
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

uint32_t* Flash_Get_Free_Address(){
	uint32_t * end_address =(uint32_t*) 0x8000000;
	uint32_t * start_address = (uint32_t*) 0x807FFF0;
	uint32_t * current_address = start_address;
	uint8_t found = 0;
	char current_vals[8];
	while(current_address >= end_address && found != 1){
		Flash_Read(current_address,current_vals,8);
		found = 1;
		for(uint8_t j=0;j<strlen(current_vals);j++){
			if(current_vals[j] != 0xFF){
				found = 0;
			}
		}
		if(found == 0){
			current_address -= 0x2;
		} else {
			current_address += 0x2;
		}
	}
	if(found == 0){
		return 0;
	} else {
		return current_address;
	}

}

uint32_t Flash_Page_To_Address(uint8_t pageNum){
	uint32_t result = pageNum * 0x800 + 0x8000000;
	return result;
}

uint8_t Flash_Address_To_Page(uint32_t address){
	uint8_t result = (address-0x8000000) / 0x800;
	return result;
}
