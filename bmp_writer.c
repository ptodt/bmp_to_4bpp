/*****************************************************************************

    plik  : bmp_writer.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.30
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : implementacja funkcji zapisu plików BMP

    licencja : MIT
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "bmp_writer.h"
#include "bmp_palette.h"

// Funkcja generowania pliku BMP z danych 1bpp
int generate_1bpp_bmp(uchar* packed_data, int width, int height, const char* output_path, int palette_variant, uchar* custom_first, uchar* custom_last) {
    FILE* file = fopen(output_path, "wb");
    if (!file) {
        return 0;
    }
    
    // Oblicz rozmiar wiersza z dopełnieniem (musi być wielokrotnością 4)
    int row_size = ((width + 7) / 8 + 3) & ~3; // Zaokrąglij w górę do wielokrotności 4
    int data_size = row_size * height;
    
    // Nagłówek BMP
    BMPHeader bmp_header = {
        .signature = {0x42, 0x4D}, // "BM"
        .file_size = 0,            // Zostanie wypełnione
        .reserved = 0,
        .data_offset = BMP_1BPP_DATA_OFFSET
    };
    
    // Oblicz rozmiar pliku
    unsigned int file_size = BMP_HEADER_SIZE + BMP_INFO_HEADER_SIZE + BMP_1BPP_PALETTE_SIZE + data_size;
    bmp_header.file_size = file_size;
    
    // Nagłówek informacyjny BMP
    BMPInfoHeader info_header = {
        .header_size = BMP_INFO_HEADER_SIZE,
        .width = width,
        .height = height,
        .planes = 1,
        .bits_per_pixel = 1,
        .compression = BMP_COMPRESSION_NONE,
        .image_size = data_size,
        .x_pixels_per_meter = 0,
        .y_pixels_per_meter = 0,
        .colors_used = 2,
        .colors_important = 0
    };
    
    // Paleta kolorów - wybrana wariant
    BMPColorEntry palette[2];
    PaletteContext palette_context = {palette_variant, 2, {custom_first[0], custom_first[1], custom_first[2]}, {custom_last[0], custom_last[1], custom_last[2]}};
    generate_palette(palette, &palette_context);
    
    // Zapisz nagłówek BMP
    fwrite(&bmp_header, 1, sizeof(BMPHeader), file);
    fwrite(&info_header, 1, sizeof(BMPInfoHeader), file);
    fwrite(palette, 1, sizeof(palette), file);
    
    
    // Zapisz dane obrazu (od dołu do góry)
    for (int y = height - 1; y >= 0; y--) {
        fwrite(packed_data + (y * ((width + 7) / 8)), 1, (width + 7) / 8, file);
        // Dodaj dopełnienie wiersza
        int padding = row_size - ((width + 7) / 8);
        for (int p = 0; p < padding; p++) {
            fputc(0x00, file);
        }
    }
    
    fclose(file);
    return 1;
}

// Funkcja generowania pliku BMP z danych 4bpp
int generate_4bpp_bmp(uchar* packed_data, int width, int height, const char* output_path, int palette_variant, uchar* custom_first, uchar* custom_last) {
    FILE* file = fopen(output_path, "wb");
    if (!file) {
        return 0;
    }
    
    // Oblicz rozmiar wiersza z dopełnieniem (musi być wielokrotnością 4)
    int row_size = ((width + 1) / 2 + 3) & ~3; // Zaokrąglij w górę do wielokrotności 4
    int data_size = row_size * height;
    
    // Nagłówek BMP
    BMPHeader bmp_header = {
        .signature = {0x42, 0x4D}, // "BM"
        .file_size = 0,            // Zostanie wypełnione
        .reserved = 0,
        .data_offset = BMP_4BPP_DATA_OFFSET
    };
    
    // Oblicz rozmiar pliku
    unsigned int file_size = BMP_HEADER_SIZE + BMP_INFO_HEADER_SIZE + BMP_4BPP_PALETTE_SIZE + data_size;
    bmp_header.file_size = file_size;
    
    // Nagłówek informacyjny BMP
    BMPInfoHeader info_header = {
        .header_size = BMP_INFO_HEADER_SIZE,
        .width = width,
        .height = height,
        .planes = 1,
        .bits_per_pixel = 4,
        .compression = BMP_COMPRESSION_NONE,
        .image_size = data_size,
        .x_pixels_per_meter = 0,
        .y_pixels_per_meter = 0,
        .colors_used = 16,
        .colors_important = 0
    };
    
    // Paleta kolorów (16 odcieni)
    BMPColorEntry palette[16];
    PaletteContext palette_context = {palette_variant, 16, {custom_first[0], custom_first[1], custom_first[2]}, {custom_last[0], custom_last[1], custom_last[2]}};
    generate_palette(palette, &palette_context);
    
    // Zapisz nagłówek BMP
    fwrite(&bmp_header, 1, sizeof(BMPHeader), file);
    fwrite(&info_header, 1, sizeof(BMPInfoHeader), file);
    fwrite(palette, 1, sizeof(palette), file);
    
    
    // Zapisz dane obrazu (od dołu do góry)
    for (int y = height - 1; y >= 0; y--) {
        fwrite(packed_data + (y * ((width + 1) / 2)), 1, (width + 1) / 2, file);
        // Dodaj dopełnienie wiersza
        int padding = row_size - ((width + 1) / 2);
        for (int p = 0; p < padding; p++) {
            fputc(0x00, file);
        }
    }
    
    fclose(file);
    return 1;
}
