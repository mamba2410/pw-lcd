#include <stdio.h>
#include <stdint.h>
#include <pw-lcd.h>
#include "ssd1854.h"

#include <stdlib.h>
#include <string.h>

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

	// New LCD instance
	pw_lcd_t lcd;
	lcd_init(&lcd);

	// Terminal buffer
	// Size Nx8 to make filling it easier.
	// Can break into blocks when displaying to fit to screen resolution
	char **term_buf = (char**)malloc(sizeof(char*) * TERMBUF_LENGTH);
	for(size_t i = 0; i < TERMBUF_LENGTH; i++)
		term_buf[i] = (char*)malloc(sizeof(char) * PIXELS_PER_ROW);


	// Open file as lcd ram contents
	const char *fname = "test.bin";
	lcd_read_file(&lcd, fname);
	
	// Convert ram contents to 'pixel values'
	lcd_decode_ram(lcd, term_buf);

	// display term_buf to screen
	display(term_buf);


	// Frees
	for(size_t i = 0; i < TERMBUF_LENGTH; i++)
		free(term_buf[i]);
	free(term_buf);
	lcd_free(&lcd);

	return 0;
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


void lcd_write(uint8_t** lcd_ram, uint8_t mode, uint8_t value) {
	
}
