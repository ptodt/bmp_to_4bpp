/*****************************************************************************

    plik  : utils.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : plik naglowkowy dla funkcji pomocniczych (konwersja obrazu i skala szarosci)

    licencja : MIT
*****************************************************************************/

#ifndef __UTILS_H__
#define __UTILS_H__

#include "defs.h"

// Prototypy funkcji konwersji obrazu
int pack_pixels_4bpp(uchar* grayscale_data, uchar* packed_data, int width, int height, int scan_direction, int pixel_order);
int pack_pixels_1bpp(uchar* grayscale_data, uchar* packed_data, int width, int height, int scan_direction, int pixel_order);

// Wzorzec Strategy dla formatów wyjściowych
int write_array(uchar* packed_data, int data_size, int width, int height, const char* array_name, const char* output_path, int output_format, int use_progmem, int bits_per_pixel, int dithering_method);

// Funkcje pomocnicze
const char* generate_c_attributes(int use_progmem);
void set_default_extension(char* output_file, int output_format);

// Indywidualne zapisywacze formatów (implementacje Strategy)
int format_c_array_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int use_progmem, int bits_per_pixel, int dithering_method);
int format_raw_data_write(uchar* packed_data, int data_size, FILE* file, int bits_per_pixel, int dithering_method);
int format_assembler_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int bits_per_pixel, int dithering_method);
int format_masm_array_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int bits_per_pixel, int dithering_method);

// Prototypy funkcji konwersji do skali szarości
int convert_to_grayscale_4bpp(uchar* image_data, uchar* grayscale_data, int width, int height, int bytes_per_row);
int convert_to_grayscale_1bpp(uchar* image_data, uchar* grayscale_data, int width, int height, int bytes_per_row, int dithering_method);
int convert_rgb_to_grayscale(uchar r, uchar g, uchar b);
uchar scale_to_4bpp(int gray_value);
uchar scale_to_1bpp(int gray_value);

// Prototypy funkcji ditheringu
int apply_floyd_steinberg_dithering(uchar* grayscale_data, int width, int height);
int apply_ordered_dithering(uchar* grayscale_data, int width, int height);

#endif
