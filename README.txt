Space:
-256 pages
-0x800 addresses per page
-Addresses increment by 0x8 (e.g. 0x807f800, 0x807f808, etc)
-8 bytes per address

Functions:
	HAL_StatusTypeDef Flash_Write(uint32_t start_address,uint8_t *buf,size_t num_bytes)
		- always erase before writing, idk why that's just what the documentation says.
	HAL_StatusTypeDef Flash_Read(uint32_t address,uint8_t *buf,size_t num_chars)
		- reads num_chars characters, starting at address, to the char array buf.
	void Flash_Erase_Page(uint8_t page_num,uint8_t num_pages) 
		- erases all pages over [page_num, page_num + num_pages)
	uint32_t* Flash_Get_Free_Address()
		- starts from 0x807FFFF and goes backwards, returning the first free memory that it finds.
	uint32_t Flash_Page_To_Address(uint8_t page_num);
		- converts a page number to the address the page starts at.
	uint8_t Flash_Address_To_Page(uint32_t address);
		- converts an address to the page number it is on.
		
Example usage of the functions can be found in main.c