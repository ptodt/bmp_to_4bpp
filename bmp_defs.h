/*****************************************************************************

    plik  : bmp_defs.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.30
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : definicje struktur i stałych dla plików BMP

    licencja : MIT
*****************************************************************************/

#ifndef BMP_DEFS_H
#define BMP_DEFS_H

#include "defs.h"

// Struktury nagłówka BMP
#pragma pack(push, 1)
typedef struct {
    uchar signature[2];     // "BM"
    dword file_size;        // Rozmiar pliku BMP
    dword reserved;         // Zarezerwowane
    dword data_offset;      // Przesunięcie do danych obrazu
} BMPHeader;

typedef struct {
    dword header_size;      // Rozmiar tego nagłówka
    dword width;            // Szerokość obrazu
    dword height;           // Wysokość obrazu
    word planes;            // Liczba płaszczyzn kolorów
    word bits_per_pixel;    // Bity na piksel
    dword compression;      // Typ kompresji
    dword image_size;       // Rozmiar danych obrazu
    dword x_pixels_per_meter;
    dword y_pixels_per_meter;
    dword colors_used;      // Liczba używanych kolorów
    dword colors_important; // Liczba ważnych kolorów
} BMPInfoHeader;
#pragma pack(pop)

// Wpis palety kolorów BMP
typedef struct {
    uchar blue;     // Składowa niebieska
    uchar green;    // Składowa zielona
    uchar red;      // Składowa czerwona
    uchar alpha;    // Składowa alfa (zwykle 0)
} BMPColorEntry;

// Stałe BMP
#define BMP_SIGNATURE_1 "BM"
#define BMP_HEADER_SIZE 14
#define BMP_INFO_HEADER_SIZE 40
#define BMP_COMPRESSION_NONE 0

// Stałe BMP 1bpp
#define BMP_1BPP_PALETTE_SIZE 8
#define BMP_1BPP_DATA_OFFSET (BMP_HEADER_SIZE + BMP_INFO_HEADER_SIZE + BMP_1BPP_PALETTE_SIZE)

// Stałe BMP 4bpp
#define BMP_4BPP_PALETTE_SIZE 64
#define BMP_4BPP_DATA_OFFSET (BMP_HEADER_SIZE + BMP_INFO_HEADER_SIZE + BMP_4BPP_PALETTE_SIZE)

#endif
