#include <stdio.h>
#include <stdint.h>
#include <pw-lcd.h>
#include "ssd1854.h"
#include "bmp.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

#define TERMBUF_LENGTH	LCD_WIDTH*LCD_HEIGHT/PIXELS_PER_ROW

void decode_ram(uint8_t *lcd_ram, char **term_buf);
void display(char **term_buf);
int convert_image(const char *fin, const char *fout, int width, int height);
int view_image(const char* fname);
void usage();


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

    const char* fin = 0;
    const char* fout = 0;
    const char* fview = 0;
    bool bview_image = false;
    int width = -1, height = -1;

    int c;
    const char* options = "i:o:vw:h:";

    while((c = getopt(argc, argv, options)) != -1) {
        switch(c) {
            case 'i':
                fin = optarg;
                break;
            case 'o':
                fout = optarg;
                break;
            case 'v':
                bview_image = true;
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'w':
                width = atoi(optarg);
                break;
            case ':':
                printf("Error: Option \'%c\' needs an argument\n", optopt);
                usage();
                return EXIT_FAILURE;
            case '?':
                printf("Unknown option character \'%s\'\n", optopt);
                usage();
            default: return EXIT_FAILURE;
        }
    }

    if(fin == 0) {
        printf("Error: Need input file\n");
        usage();
        return EXIT_FAILURE;
    }

    int err = 0;

    if(fout != 0) {
        err = convert_image(fin, fout, width, height);
        if(err) {
            printf("Error converting image \"%s\" to \"%s\".\n", fin, fout);
            return EXIT_FAILURE;
        }
    }


    if(bview_image) {
        fview = (fout == 0)?fin:fout;
        err = view_image(fview);

        if(err) {
            printf("Error viewing image \"%s\"\n", fview);
            return EXIT_FAILURE;
        }

    }

    return 0;

}

void usage() {
    printf("\
Usage: pw-lcd -i <input> [-o <output] [-v]\n\
    -i <input>  Input file.\n\
    -o <output> Output file\n\
    -v          View image\n\
    -w          Image width\n\
    -h          Image height\n\
");

}

int convert_image(const char *fin, const char *fout, int width, int height) {

    uint8_t *bmp_img = malloc(2304);
    uint8_t *pw_img = malloc(2304/4);
    size_t bmp_size = bmp_to_bytes(fin, bmp_img, &width, &height);

    if(bmp_size == 0) {
        return -1;
    }

    int err = convert_8bpp_to_pw(bmp_img, pw_img, bmp_size, width, height);

    if(err) {
        return -1;
    }

    FILE *fh = fopen(fout, "wb");
    if(!fh) {
        printf("Error: Can't open file for writing\n");
        return -1;
    }
    fwrite(pw_img, 1, 2304/4, fh);
    fclose(fh);

    free(bmp_img);
    free(pw_img);

    return 0;

}

int view_image(const char* fname) {

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
	//const char *fname = "test_image.bin";
	lcd_read_file(&lcd, fname);
    //memcpy(lcd.memory, pokeball_bin, pokeball_bin_len);

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
