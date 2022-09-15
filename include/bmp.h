#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stddef.h>

int bmp_to_bytes(const char* fname, uint8_t *bytes);
int convert_8bpp_to_pw(uint8_t *data, uint8_t *pw, size_t data_size, size_t width, size_t height);

#endif /* BMP_H */
