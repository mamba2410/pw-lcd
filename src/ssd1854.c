#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "ssd1854.h"


/*
 * Makes a new `pw_lcd_t` instance
 */
void lcd_init(pw_lcd_t *lcd) {
	lcd->status = 0x0B; // Magic value in datasheet
	lcd->status |= LCD_RESET_BIT;
	lcd->memory = malloc(LCD_RAM_LENGTH);
}


/*
 * Frees a `pw_lcd_t` instance
 */
void lcd_free(pw_lcd_t *lcd) {
	free(lcd->memory);
}

int lcd_read_file(pw_lcd_t *lcd, const char* fname) {
	FILE* fh = fopen(fname, "rb");
	if( fh == NULL) return -1;

	struct stat s;
	if( stat(fname, &s) == -1 ) return -1;
	fread(lcd->memory, s.st_size, 1, fh);
    //printf("%d\n", s.st_size);
	fclose(fh);

	return 0;
}

/*
 * Decodes lcd ram into a terminal buffer that can be displayed
 */
void lcd_decode_ram(pw_lcd_t lcd, char **term_buf) {
	uint8_t *lcd_ram = lcd.memory;

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
			//uint8_t pixel_mask = 1<<j;
			//pixel_value[j] = (bit_plane_upper&pixel_mask)<<1 | (bit_plane_lower&pixel_mask);
			//pixel_value[j] >>= j;
			pixel_value[j]  = ((bit_plane_upper>>j)&1)<<1;
			pixel_value[j] |= (bit_plane_lower>>j)&1;

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

void lcd_pw_to_bitmap(pw_lcd_t lcd, uint8_t *data, size_t width, size_t height) {

    uint8_t *ram = lcd.memory;
    uint8_t pu, pl, pval;
    size_t pcol, prow, pidx;

    for(size_t i = 0; i < LCD_RAM_LENGTH; i+=2) {
        pu = ram[i];
        pl = ram[i+1];

        for(size_t j = 0; j < PIXELS_PER_ROW; j++) {
            pval  = ((pu>>j)&1) << 1;
            pval |= ((pl>>j)&1);

            pcol = (i/2)%width;
            prow = i/(2*width) * PIXELS_PER_ROW + j;
            pidx = prow*width + pcol;

            data[pidx] = pval;
        }

    }

}


int lcd_write_u8(pw_lcd_t *lcd, size_t addr, uint8_t val) {
	if (addr >= LCD_RAM_LENGTH) return -1;
	lcd->memory[addr] = val;

	return 0;
}


