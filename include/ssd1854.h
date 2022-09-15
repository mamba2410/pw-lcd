#ifndef SSD1854_H
#define SSD1854_H

#include <stdint.h>
#include <stddef.h>

#define LCD_RESET_BIT	(1<<5)
#define LCD_ON_BIT		(1<<6)
#define LCD_BUSY_BIT	(1<<7)


#define LCD_WIDTH	96
#define LCD_HEIGHT	48
#define BIT_DEPTH	2
#define PIXELS_PER_ROW	8
#define LCD_RAM_LENGTH	LCD_WIDTH*LCD_HEIGHT*BIT_DEPTH/8


typedef struct {
	uint8_t *memory;
	uint8_t status;
    size_t capacity;
    size_t width;
    size_t height;
} pw_lcd_t;


void lcd_init(pw_lcd_t *lcd, size_t width, size_t height);
void lcd_free(pw_lcd_t *lcd);
void lcd_decode_ram(pw_lcd_t lcd, char **term_buf);
int lcd_read_file(pw_lcd_t *lcd, const char* fname);


#endif /* SSD1854_H */
