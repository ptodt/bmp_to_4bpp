/*****************************************************************************

    plik  : bmp_reader.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : implementacja us odczytu plików BMP

    licencja : MIT
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "bmp_reader.h"

int read_bmp_header(FILE* file, BMPHeader* header, BMPInfoHeader* info) {
    if (fread(header, sizeof(BMPHeader), 1, file) != 1) {
        return 0;
    }

    if (header->signature[0] != 'B' || header->signature[1] != 'M') {
        return 0;
    }

    if (fread(info, sizeof(BMPInfoHeader), 1, file) != 1) {
        return 0;
    }

    return 1;
}

int read_bmp_image_data(FILE* file, uchar* image_data, int data_size, dword data_offset) {
    fseek(file, data_offset, SEEK_SET);
    if (fread(image_data, 1, data_size, file) != data_size) {
        return 0;
    }
    return 1;
}

int validate_bmp_format(BMPHeader* header, BMPInfoHeader* info) {
    // Sprawdź czy to 24-bitowy lub 32-bitowy BMP
    if (info->bits_per_pixel != 24 && info->bits_per_pixel != 32) {
        return 0;
    }
    
    // Sprawdź czy kompresja jest obsługiwana (0 = brak kompresji)
    if (info->compression != 0) {
        return 0;
    }
    
    return 1;
}

int calculate_bmp_row_size(dword width, int bits_per_pixel) {
    int bytes_per_pixel = bits_per_pixel / 8;
    return ((width * bytes_per_pixel + 3) / 4) * 4; // Wyrównaj do granicy 4-bajtowej
}
