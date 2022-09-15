#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include "bmp.h"

int bmp_to_bytes(const char* fname, uint8_t *bytes) {
	FILE* fh = fopen(fname, "rb");
    if( fh == NULL) {
        printf("Error opening file\n");
        return -1;
    }

	struct stat s;
	if( stat(fname, &s) == -1 ) return -1;

    uint8_t *buf = malloc(s.st_size);
	fread(buf, s.st_size, 1, fh);
    fclose(fh);

    uint32_t *bmp_len_ptr = (uint32_t*)&(buf[0x02]);
    uint32_t *data_offset_ptr = (uint32_t*)&(buf[0x0a]);
    //uint32_t *dib_size_ptr = (uint32_t*)&(buf[0x0e]);
    uint16_t *bitmap_depth = (uint16_t*)&(buf[0x1c]);
    uint32_t *compression_format = (uint32_t*)&(buf[0x1e]);
    uint32_t *data_size = (uint32_t*)&(buf[0x22]);
    uint32_t *n_colour_palette = (uint32_t*)&(buf[0x2e]);
    int32_t *bitmap_width = (int32_t*)&(buf[0x12]);
    int32_t *bitmap_height = (int32_t*)&(buf[0x16]);

    printf("Length: %d\n", *bmp_len_ptr);
    printf("Data offset: %04x\n", *data_offset_ptr);
    printf("length - data offset: %d\n", *bmp_len_ptr - *data_offset_ptr);

    printf("Image size: %dx%d\n", *bitmap_width, *bitmap_height);
    printf("bitmap depth: %d\n", *bitmap_depth);
    printf("compression format: %d\n", *compression_format);
    printf("data size: %d\n", *data_size);
    printf("Number in palette: %d\n", *n_colour_palette);

    memcpy(bytes, &(buf[*data_offset_ptr]), *data_size);

    free(buf);

    return 0;
}

int convert_8bpp_to_pw(uint8_t *data, uint8_t *pw, size_t data_size, size_t width, size_t height) {

    size_t pw_size = data_size/4;
    memset(pw, 0, pw_size);

    for(size_t i = 0; i < data_size; i++) {
        size_t row = height-i/width;    // bmp (usually) starts bottom-left
        size_t col = i%width;
        uint8_t bitpos = row%8;
        uint8_t bitrow = row/8;

        size_t pw_index = 2*(bitrow*width + col);

        uint8_t pixval = (data[i] >> 6)&0x03; // Only use the top 2 bits

        pw[pw_index]   |= ((pixval&2)>>1) << bitpos;
        pw[pw_index+1] |= (pixval&1) << bitpos;


    }

    return 0;
}

