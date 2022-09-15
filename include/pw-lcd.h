#ifndef PW_LCD_H
#define PW_LCD_H

#include <stdint.h>
#include <stddef.h>

#define TERMBUF_LENGTH	LCD_WIDTH*LCD_HEIGHT/PIXELS_PER_ROW
#define DEFAULT_LCD_WIDTH   96
#define DEFAULT_LCD_HEIGHT  64

void decode_ram(uint8_t *lcd_ram, char **term_buf);
void display(char **term_buf, size_t size, size_t width, size_t height);
int convert_image(const char *fin, const char *fout, int *width, int *height);
int view_image(const char* fname, int width, int height);
void usage();

#endif /* PW_LCD_H */
