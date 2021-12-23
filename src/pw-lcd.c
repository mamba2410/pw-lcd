#include <stdio.h>
#include <stdint.h>
#include <pw-lcd.h>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define LCD_WIDTH	96
#define LCD_HEIGHT	64
#define BIT_DEPTH	2
#define PIXELS_PER_ROW	8
#define LCD_RAM_LENGTH	LCD_WIDTH*LCD_HEIGHT*BIT_DEPTH/8
#define TERMBUF_LENGTH	LCD_WIDTH*LCD_HEIGHT/PIXELS_PER_ROW

void decode_ram(uint8_t *lcd_ram, char **term_buf);
void display(char **term_buf);

	/*
	 *	LCD split into rows 8-pixels tall.
	 *	Each 2 bytes are eight 2-bit pixels 
	 * 
	 *	bytes 0b00110101 0b11001010 in mem corresponds to pixel value of 
	 *	~.....01.........~
	 *	~.....01.........~
	 *	~.....10.........~
	 *	~.....10.........~
	 *	~.....01.........~
	 *	~.....10.........~
	 *	~.....01.........~
	 *	~.....10.........~
	 *
	 *	treat display as an Nx8 array of pixels, where N=WIDTH*HEIGHT/8
	 *	
	 *
	 */
int main(int argc, char** argv){

	// Frame buffer that the LCD should read
	// Assumes the above format
	uint8_t *lcd_ram = (uint8_t*)malloc(LCD_RAM_LENGTH);

	// Terminal buffer
	// Size Nx8 to make filling it easier.
	// Can break into blocks when displaying to fit to screen resolution
	char **term_buf = (char**)malloc(sizeof(char*) * TERMBUF_LENGTH);
	for(size_t i = 0; i < TERMBUF_LENGTH; i++)
		term_buf[i] = (char*)malloc(sizeof(char) * PIXELS_PER_ROW);


	// Open file as lcd ram contents
	const char *fname = "test.bin";
	FILE* fh = fopen(fname, "rb");
	if( fh == NULL) return -1;

	struct stat s;
	if( stat(fname, &s) == -1 ) return -1;
	fread(lcd_ram, s.st_size, 1, fh);
	fclose(fh);

	
	// Convert ram contents to 'pixel values'
	decode_ram(lcd_ram, term_buf);

	// display term_buf to screen
	display(term_buf);


	// Frees
	for(size_t i = 0; i < TERMBUF_LENGTH; i++)
		free(term_buf[i]);
	free(term_buf);
	free(lcd_ram);

	return 0;
}



void decode_ram(uint8_t *lcd_ram, char **term_buf) {

	// Loop over ram contents, two bytes at a time
	for(size_t i = 0; i < LCD_RAM_LENGTH; i+=2) {

		// Temp numerical values for one column of pixels
		uint8_t pixel_value[PIXELS_PER_ROW];

		// Bit planes from ram
		uint8_t bit_plane_upper = lcd_ram[i];
		uint8_t bit_plane_lower = lcd_ram[i+1];

		// For each pixel in this chunk
		for(size_t j = 0; j < PIXELS_PER_ROW; j++) {
			// Calculate pixel value
			uint8_t pixel_mask = 1<<j;
			pixel_value[j] = (bit_plane_upper&pixel_mask)<<1 | (bit_plane_lower&pixel_mask);
			pixel_value[j] >>= j;


			// Set terminal character
			char tchar = ' ';
			switch(pixel_value[j]) {
				case 0: tchar = '.'; break;
				case 1: tchar = 'o'; break;
				case 2: tchar = '+'; break;
				case 3: tchar = '#'; break;
				default: printf("Invalid value: %u\n", pixel_value[j]);
			}
		
			// div by 2 since 2 bytes make one set of row characters
			term_buf[i/2][j] = tchar;
		}
	}
}



void display(char **term_buf) {
	char* line = malloc(sizeof(char)*2*LCD_WIDTH+4);

	// b loops over blocks.
	// One block is a set of 8*LCD_WIDTH pixels
	for(size_t b = 0; b*LCD_WIDTH < TERMBUF_LENGTH; b++) {

		// s loops over the row in each block
		// One row is 1*LCD_WIDTH pixels
		for(size_t s = 0; s < PIXELS_PER_ROW; s++) {
			sprintf(line, "%2d: ", b*PIXELS_PER_ROW+s);

			// Append each pixel/character in the row to the line buffer
			// Double up to make image square
			for(size_t i = 0; i < LCD_WIDTH; i++) {
				char c = term_buf[b*LCD_WIDTH+i][s];
				sprintf(line, "%s%c%c", line, c, c);
			}
			printf("%s\n", line);
		}
	}

}

