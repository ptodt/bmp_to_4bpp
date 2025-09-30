/*****************************************************************************

    plik  : bmp_reader.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28

    opis  : plik naglowkowy dla obsługi plików BMP

    licencja : MIT
*****************************************************************************/

#ifndef __BMP_READER_H__
#define __BMP_READER_H__

#include "defs.h"
#include "bmp_defs.h"

// Prototypy funkcji
int read_bmp_header(FILE* file, BMPHeader* header, BMPInfoHeader* info);
int read_bmp_image_data(FILE* file, uchar* image_data, int data_size, dword data_offset);
int validate_bmp_format(BMPHeader* header, BMPInfoHeader* info);
int calculate_bmp_row_size(dword width, int bits_per_pixel);

#endif
