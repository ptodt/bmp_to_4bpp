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

// BMP header structures
#pragma pack(push, 1)
typedef struct {
    uchar signature[2];     // "BM"
    dword file_size;        // Size of the BMP file
    dword reserved;         // Reserved
    dword data_offset;      // Offset to image data
} BMPHeader;

typedef struct {
    dword header_size;      // Size of this header
    dword width;            // Image width
    dword height;           // Image height
    word planes;            // Number of color planes
    word bits_per_pixel;    // Bits per pixel
    dword compression;      // Compression type
    dword image_size;       // Size of image data
    dword x_pixels_per_meter;
    dword y_pixels_per_meter;
    dword colors_used;      // Number of colors used
    dword colors_important; // Number of important colors
} BMPInfoHeader;
#pragma pack(pop)

// Function prototypes
int read_bmp_header(FILE* file, BMPHeader* header, BMPInfoHeader* info);
int read_bmp_image_data(FILE* file, uchar* image_data, int data_size, dword data_offset);
int validate_bmp_format(BMPHeader* header, BMPInfoHeader* info);
int calculate_bmp_row_size(dword width, int bits_per_pixel);

#endif
