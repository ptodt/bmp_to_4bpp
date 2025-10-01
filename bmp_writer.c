/*****************************************************************************

    plik  : bmp_writer.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.30
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : implementacja funkcji zapisu plików BMP

    licencja : MIT
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_writer.h"
#include "bmp_palette.h"

// Funkcja generowania pliku BMP z danych 1bpp
int generate_1bpp_bmp(uchar* packed_data, PreviewContext* preview_ctx) {
    int width = preview_ctx->width;
    int height = preview_ctx->height;
    const char* output_path = preview_ctx->output_path;
    int palette_variant = preview_ctx->palette_variant;
    uchar* custom_first = preview_ctx->custom_first;
    uchar* custom_last = preview_ctx->custom_last;
    int scan_direction = preview_ctx->scan_direction;
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
    if (scan_direction) {
        // Skanowanie poziome - dane są w formacie wierszowym
        for (int y = height - 1; y >= 0; y--) {
            fwrite(packed_data + (y * ((width + 7) / 8)), 1, (width + 7) / 8, file);
            // Dodaj dopełnienie wiersza
            int padding = row_size - ((width + 7) / 8);
            for (int p = 0; p < padding; p++) {
                fputc(0x00, file);
            }
        }
    } else {
        // Skanowanie pionowe - dane są w formacie kolumnowym, musimy je przekonwertować
        uchar* row_data = malloc((width + 7) / 8);
        for (int y = height - 1; y >= 0; y--) {
            // Wyzeruj wiersz
            memset(row_data, 0, (width + 7) / 8);
            
            // Skopiuj piksele z kolumn do wiersza
            for (int x = 0; x < width; x++) {
                int col_bytes = (height + 7) / 8;
                int byte_index = x * col_bytes + (y / 8);
                int bit_index = y % 8;
                
                if (byte_index < (width * col_bytes)) {
                    uchar pixel_bit = (packed_data[byte_index] >> bit_index) & 1;
                    if (pixel_bit) {
                        int row_byte = x / 8;
                        int row_bit = 7 - (x % 8);
                        row_data[row_byte] |= (1 << row_bit);
                    }
                }
            }
            
            fwrite(row_data, 1, (width + 7) / 8, file);
            // Dodaj dopełnienie wiersza
            int padding = row_size - ((width + 7) / 8);
            for (int p = 0; p < padding; p++) {
                fputc(0x00, file);
            }
        }
        free(row_data);
    }
    
    fclose(file);
    return 1;
}

// Funkcja generowania pliku BMP z danych 4bpp
int generate_4bpp_bmp(uchar* packed_data, PreviewContext* preview_ctx) {
    int width = preview_ctx->width;
    int height = preview_ctx->height;
    const char* output_path = preview_ctx->output_path;
    int palette_variant = preview_ctx->palette_variant;
    uchar* custom_first = preview_ctx->custom_first;
    uchar* custom_last = preview_ctx->custom_last;
    int scan_direction = preview_ctx->scan_direction;
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
    if (scan_direction) {
        // Skanowanie poziome - dane są w formacie wierszowym
        for (int y = height - 1; y >= 0; y--) {
            fwrite(packed_data + (y * ((width + 1) / 2)), 1, (width + 1) / 2, file);
            // Dodaj dopełnienie wiersza
            int padding = row_size - ((width + 1) / 2);
            for (int p = 0; p < padding; p++) {
                fputc(0x00, file);
            }
        }
    } else {
        // Skanowanie pionowe - dane są w formacie kolumnowym, musimy je przekonwertować
        uchar* row_data = malloc((width + 1) / 2);
        for (int y = height - 1; y >= 0; y--) {
            // Wyzeruj wiersz
            memset(row_data, 0, (width + 1) / 2);
            
            // Skopiuj piksele z kolumn do wiersza
            for (int x = 0; x < width; x += 2) {
                int col_bytes = (height + 1) / 2;
                int byte_index = (x / 2) * col_bytes + (y / 2);
                
                if (byte_index < ((width / 2) * col_bytes)) {
                    uchar packed_byte = packed_data[byte_index];
                    uchar pixel1, pixel2;
                    
                    // W formacie pionowym, każdy bajt zawiera 2 piksele z tej samej kolumny
                    if (y % 2 == 0) {
                        // Pierwszy piksel w parze
                        pixel1 = (packed_byte >> 4) & 0x0F;
                        pixel2 = packed_byte & 0x0F;
                    } else {
                        // Drugi piksel w parze
                        pixel1 = packed_byte & 0x0F;
                        pixel2 = (packed_byte >> 4) & 0x0F;
                    }
                    
                    // Zapisz piksele w wierszu
                    if (x + 1 < width) {
                        row_data[x / 2] = (pixel1 << 4) | pixel2;
                    } else {
                        row_data[x / 2] = (pixel1 << 4);
                    }
                }
            }
            
            fwrite(row_data, 1, (width + 1) / 2, file);
            // Dodaj dopełnienie wiersza
            int padding = row_size - ((width + 1) / 2);
            for (int p = 0; p < padding; p++) {
                fputc(0x00, file);
            }
        }
        free(row_data);
    }
    
    fclose(file);
    return 1;
}
