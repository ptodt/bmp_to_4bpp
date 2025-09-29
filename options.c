/*****************************************************************************

    plik  : options.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : implementacja obsługi argumentów wiersza poleceń i wyświetlania pomocy

    licencja : MIT
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "options.h"

// ============================================================================
// Funkcje obsługi argumentów
// ============================================================================

void print_usage(const char* program_name) {
    printf("Usage: %s [OPTIONS] INPUT_BMP [OUTPUT_C_FILE]\n", program_name);
    printf("\n");
    printf("Converts a BMP image to a C array containing packed pixel data.\n");
    printf("Supports both 4bpp (4 bits per pixel) and 1bpp (1 bit per pixel) formats.\n");
    printf("\n");
    printf("Options:\n");
    printf("  -4, --4bpp          Use 4 bits per pixel (default)\n");
    printf("  -1, --1bpp          Use 1 bit per pixel (black/white)\n");
    printf("  -h, --horizontal    Scan horizontally (rows) (default)\n");
    printf("  -v, --vertical      Scan vertically (columns)\n");
    printf("  -l, --little-endian Little endian pixel order (default)\n");
    printf("  -b, --big-endian    Big endian pixel order\n");
    printf("  -c, --c-array       C-like Array format (.h) (default)\n");
    printf("  -r, --raw-data      Raw Data format (.hex)\n");
    printf("  -a, --assembler     Assembler format (.inc)\n");
    printf("  -aa, --assembler-array MASM array format (.inc)\n");
    printf("  -p, --progmem       Add PROGMEM keyword to C arrays\n");
    printf("  -n, --name NAME     Set array name (default: image_data)\n");
    printf("\n");
    printf("Dithering options (only for 1bpp):\n");
    printf("  -d, --dither METHOD Floyd-Steinberg dithering (default for 1bpp)\n");
    printf("                      METHODS: floyd, o8x8, none\n");
    printf("\n");
    printf("  --help              Show this help message\n");
    printf("\n");
    printf("If OUTPUT_FILE is not specified, defaults to 'image_data.h'.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s image.bmp                    # 4bpp conversion\n", program_name);
    printf("  %s -1 image.bmp                 # 1bpp with Floyd-Steinberg\n", program_name);
    printf("  %s -1 -d o8x8 image.bmp         # 1bpp with ordered dithering\n", program_name);
    printf("  %s -1 -d none image.bmp         # 1bpp without dithering\n", program_name);
    printf("  %s -c -p image.bmp output.h\n", program_name);
    printf("  %s -r image.bmp data.hex\n", program_name);
    printf("  %s -a image.bmp data.inc\n", program_name);
    printf("  %s -aa image.bmp data.inc\n", program_name);
    printf("  %s -n my_image -p image.bmp\n", program_name);
    printf("  %s -n sprite_data -v -a image.bmp sprite.inc\n", program_name);
}

int parse_arguments(int argc, char* argv[], ConversionContext* context, char** input_file, char** output_file) {
    *input_file = NULL;
    *output_file = "image_data.h"; // Zostanie zaktualizowane na podstawie formatu
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Obsługa opcji
            if (strcmp(argv[i], "-4") == 0 || strcmp(argv[i], "--4bpp") == 0) {
                context->bits_per_pixel = BITS_PER_PIXEL_4BPP;
            } else if (strcmp(argv[i], "-1") == 0 || strcmp(argv[i], "--1bpp") == 0) {
                context->bits_per_pixel = BITS_PER_PIXEL_1BPP;
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--horizontal") == 0) {
                context->scan_direction = 1;
            } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--vertical") == 0) {
                context->scan_direction = 0;
            } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--little-endian") == 0) {
                context->pixel_order = 1;
            } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--big-endian") == 0) {
                context->pixel_order = 0;
            } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--c-array") == 0) {
                context->output_format = FORMAT_C_ARRAY;
            } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--raw-data") == 0) {
                context->output_format = FORMAT_RAW_DATA;
            } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--assembler") == 0) {
                context->output_format = FORMAT_ASSEMBLER;
            } else if (strcmp(argv[i], "-aa") == 0 || strcmp(argv[i], "--assembler-array") == 0) {
                context->output_format = FORMAT_MASM_ARRAY;
            } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--progmem") == 0) {
                context->use_progmem = 1;
            } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--name") == 0) {
                if (i + 1 < argc) {
                    strncpy(context->array_name, argv[i + 1], sizeof(context->array_name) - 1);
                    context->array_name[sizeof(context->array_name) - 1] = '\0';
                    i++; // Pomiń następny argument, bo to nazwa tablicy
                } else {
                    printf("Error: --name requires an argument\n");
                    return 0;
                }
            } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dither") == 0) {
                if (i + 1 < argc) {
                    if (strcmp(argv[i + 1], "floyd") == 0) {
                        context->dithering_method = DITHERING_FLOYD;
                    } else if (strcmp(argv[i + 1], "o8x8") == 0) {
                        context->dithering_method = DITHERING_ORDERED;
                    } else if (strcmp(argv[i + 1], "none") == 0) {
                        context->dithering_method = DITHERING_NONE;
                    } else {
                        printf("Error: Invalid dithering method '%s'. Use: floyd, o8x8, or none\n", argv[i + 1]);
                        return 0;
                    }
                    i++; // Pomiń następny argument, bo to metoda ditheringu
                } else {
                    printf("Error: -d/--dither requires an argument (floyd, o8x8, or none)\n");
                    return 0;
                }
            } else if (strcmp(argv[i], "--help") == 0) {
                return 0; // Pokaże pomoc
            } else {
                printf("Error: Unknown option '%s'\n", argv[i]);
                return 0;
            }
        } else {
            // Obsługa argumentów plików
            if (*input_file == NULL) {
                *input_file = argv[i];
            } else if (strcmp(*output_file, "image_data.h") == 0) {
                *output_file = argv[i];
            } else {
                printf("Error: Too many file arguments\n");
                return 0;
            }
        }
    }
    
    if (*input_file == NULL) {
        printf("Error: No input file specified\n");
        return 0;
    }
    
    return 1;
}
