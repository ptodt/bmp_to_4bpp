/*****************************************************************************
    plik  : bmp_to_4bpp.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : konwersja pliku BMP na tablice C zawierajace bajty obrazka w formacie 4bpp
            (4 bits per pixel) - 2 piksele na bajt

    licencja : MIT
*****************************************************************************/

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "bmp_reader.h"
#include "utils.h"
#include "options.h"
#include "bmp_writer.h"

int main(int argc, char* argv[]) {
    printf("\n");
    printf("BMP to xbpp Array Converter v1.0 2025-09-28\n");
    printf("\n");
    printf("CODE BY                                      \n");
    printf("    ----.-.---.---.---.                      \n");
    printf("        |-.---.   |--.|#==---.. .  .  .      \n");
    printf("     |  | |   | --.   |#==---. . .           \n");
    printf("    -'--'-'---'---'---'               2025.09\n");
    printf("\n");

    ConversionContext context = {1, 1, FORMAT_C_ARRAY, 0, "image_data", BITS_PER_PIXEL_4BPP, DITHERING_NONE, 50, 50, 0}; // Domyślnie: poziomo, little endian, tablica C, bez PROGMEM, nazwa tablicy, 4bpp, None, jasność 50%, kontrast 50%, bez BMP
    char* input_path = NULL;
    char* output_path = NULL;
    char output_buffer[256]; // Bufor na ścieżkę wyjściową

    if (!parse_arguments(argc, argv, &context, &input_path, &output_path)) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Skopiuj ścieżkę wyjściową do bufora i ustaw domyślne rozszerzenie
    strncpy(output_buffer, output_path, sizeof(output_buffer) - 1);
    output_buffer[sizeof(output_buffer) - 1] = '\0';
    
    if (strcmp(output_path, "image_data.h") == 0) {
        set_default_extension(output_buffer, context.output_format);
        output_path = output_buffer;
    }

    printf("- converting %s to %s\n", input_path, output_path);

    FILE* file = fopen(input_path, "rb");
    if (!file) {
        printf("Error: Cannot open input file %s\n", input_path);
        return 1;
    }

    BMPHeader header;
    BMPInfoHeader info_header;

    if (!read_bmp_header(file, &header, &info_header)) {
        printf("Error: Invalid BMP file format\n");
        fclose(file);
        return 1;
    }

    printf("- image size: %dx%d pixels\n", (int)info_header.width, (int)info_header.height);
    printf("- bits per pixel: %d\n", (int)info_header.bits_per_pixel);

    if (!validate_bmp_format(&header, &info_header)) {
        printf("Error: Only uncompressed 24-bit and 32-bit BMP files are supported\n");
        fclose(file);
        return 1;
    }

    // Wyświetl opcje konwersji
    printf("- opcje konwersji:\n");
    printf("  - głębia kolorów: %dbpp\n", context.bits_per_pixel);
    printf("  - kierunek skanowania: %s\n", context.scan_direction ? "poziomy" : "pionowy");
    printf("  - kolejność pikseli: %s endian\n", context.pixel_order ? "little" : "big");
    printf("  - format wyjściowy: %s\n", 
           context.output_format == FORMAT_C_ARRAY ? "Tablica C (.h)" :
           context.output_format == FORMAT_RAW_DATA ? "Surowe dane (.hex)" : "Assembler (.inc)");
    printf("  - PROGMEM: %s\n", context.use_progmem ? "tak" : "nie");
    printf("  - nazwa tablicy: %s\n", context.array_name);
    if (context.bits_per_pixel == BITS_PER_PIXEL_1BPP) {
        printf("  - metoda ditheringu: %s\n",
               context.dithering_method == DITHERING_FLOYD ? "Floyd-Steinberg" :
               context.dithering_method == DITHERING_ORDERED ? "Ordered 8x8" : "Brak");
        printf("  - jasność: %d%%\n", context.brightness);
        printf("  - kontrast: %d%%\n", context.contrast);
    }

    // Oblicz rozmiar wiersza z dopełnieniem
    int row_size = calculate_bmp_row_size(info_header.width, info_header.bits_per_pixel);
    int image_data_size = row_size * info_header.height;

    // Alokuj pamięć na dane obrazu
    uchar* image_data = (uchar*)malloc(image_data_size);
    if (!image_data) {
        printf("Error: Cannot allocate memory for image data\n");
        fclose(file);
        return 1;
    }

    // Odczytaj dane obrazu
    if (!read_bmp_image_data(file, image_data, image_data_size, header.data_offset)) {
        printf("Error: Cannot read image data\n");
        free(image_data);
        fclose(file);
        return 1;
    }

    fclose(file);

    // Konwertuj do skali szarości
    int width = (int)info_header.width;
    int height = (int)info_header.height;
    
    // Dla 4bpp upewnij się, że szerokość jest parzysta
    if (context.bits_per_pixel == BITS_PER_PIXEL_4BPP && width % 2 != 0) {
        width++;
    }

    uchar* grayscale_data = (uchar*)malloc(width * height);
    if (!grayscale_data) {
        printf("Error: Cannot allocate memory for grayscale data\n");
        free(image_data);
        return 1;
    }

    // Konwertuj do skali szarości w zależności od trybu
    if (context.bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        if (!convert_to_grayscale_4bpp(image_data, grayscale_data, (int)info_header.width, (int)info_header.height, row_size)) {
            printf("Error: Failed to convert to grayscale\n");
            free(image_data);
            free(grayscale_data);
            return 1;
        }
        } else if (context.bits_per_pixel == BITS_PER_PIXEL_1BPP) {
            if (!convert_to_grayscale_1bpp(image_data, grayscale_data, (int)info_header.width, (int)info_header.height, row_size, context.dithering_method, context.brightness, context.contrast)) {
            printf("Error: Failed to convert to grayscale with dithering\n");
            free(image_data);
            free(grayscale_data);
            return 1;
        }
    } else {
        printf("Error: Unsupported bits per pixel: %d\n", context.bits_per_pixel);
        free(image_data);
        free(grayscale_data);
        return 1;
    }

    // Pakuj piksele w odpowiednim formacie
    int packed_size;
    if (context.bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        packed_size = (width * height) / 2; // 2 piksele na bajt
    } else {
        packed_size = (width * height + 7) / 8; // 8 pikseli na bajt
    }
    
    uchar* packed_data = (uchar*)malloc(packed_size);
    if (!packed_data) {
        printf("Error: Cannot allocate memory for packed data\n");
        free(image_data);
        free(grayscale_data);
        return 1;
    }

    // Wybierz odpowiednią funkcję pakowania
    int pack_result;
    if (context.bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        pack_result = pack_pixels_4bpp(grayscale_data, packed_data, width, height, context.scan_direction, context.pixel_order);
    } else {
        pack_result = pack_pixels_1bpp(grayscale_data, packed_data, width, height, context.scan_direction, context.pixel_order);
    }
    
    if (!pack_result) {
        printf("Error: Failed to pack pixels\n");
        free(image_data);
        free(grayscale_data);
        free(packed_data);
        return 1;
    }

    // Zapisz plik wyjściowy
    if (!write_array(packed_data, packed_size, width, height, context.array_name, output_path, context.output_format, context.use_progmem, context.bits_per_pixel, context.dithering_method, context.brightness, context.contrast)) {
        printf("Error: Failed to write output file\n");
        free(image_data);
        free(grayscale_data);
        free(packed_data);
        return 1;
    }
    
    // Generuj BMP preview jeśli wymagane
    if (context.generate_bmp) {
        char bmp_path[256];
        strcpy(bmp_path, output_path);
        char* ext = strrchr(bmp_path, '.');
        if (ext) {
            strcpy(ext, ".bmp");
        } else {
            strcat(bmp_path, ".bmp");
        }
        
        int success = 0;
        if (context.bits_per_pixel == BITS_PER_PIXEL_1BPP) {
            success = generate_1bpp_bmp(packed_data, width, height, bmp_path);
        } else if (context.bits_per_pixel == BITS_PER_PIXEL_4BPP) {
            success = generate_4bpp_bmp(packed_data, width, height, bmp_path);
        }
        
        if (success) {
            printf("- BMP preview saved: %s\n", bmp_path);
        } else {
            printf("Warning: Failed to generate BMP preview\n");
        }
    }

    printf("- conversion completed successfully\n");
    printf("- packed data size: %d bytes\n", packed_size);

    free(image_data);
    free(grayscale_data);
    free(packed_data);

    return 0;
}



