/*****************************************************************************

    plik  : utils.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : implementacja funkcji pomocniczych (konwersja obrazu i skala szarosci)

    licencja : MIT
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"
#include "utils.h"

// ============================================================================
// Funkcje konwersji do skali szarości
// ============================================================================

int convert_rgb_to_grayscale(uchar r, uchar g, uchar b) {
    // Konwersja do skali szarości: 0.299*R + 0.587*G + 0.114*B
    return (int)(0.299f * r + 0.587f * g + 0.114f * b);
}

uchar scale_to_4bpp(int gray_value) {
    // Skaluj do 4bpp (0-15) - dzielenie całkowite jak w Pythonie
    uchar gray_4bpp = (uchar)(gray_value / 16);
    if (gray_4bpp > 15) gray_4bpp = 15;
    return gray_4bpp;
}

int convert_to_grayscale_4bpp(uchar* image_data, uchar* grayscale_data, int width, int height, int bytes_per_row) {
    int bytes_per_pixel = 3; // Zakładamy 24-bit na razie
    
    // BMP przechowuje wiersze od dołu do góry, ale chcemy od góry do dołu jak PIL
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Odczytaj z dolnego wiersza najpierw (format BMP)
            int src_y = height - 1 - y;
            int src_offset = src_y * bytes_per_row + x * bytes_per_pixel;
            
            // Wyciągnij wartości RGB (BMP przechowuje jako BGR)
            uchar r = image_data[src_offset + 2];
            uchar g = image_data[src_offset + 1];
            uchar b = image_data[src_offset + 0];
            
            // Konwertuj do skali szarości
            int gray = convert_rgb_to_grayscale(r, g, b);
            
            // Skaluj do 4bpp
            uchar gray_4bpp = scale_to_4bpp(gray);
            
            // Zapisz w buforze skali szarości (od góry do dołu jak PIL)
            grayscale_data[y * width + x] = gray_4bpp;
        }
    }
    
    return 1;
}

// ============================================================================
// Funkcje pomocnicze
// ============================================================================

const char* generate_c_attributes(int use_progmem) {
    if (use_progmem) {
        return " PROGMEM";
    } else {
        return "";
    }
}

void set_default_extension(char* output_file, int output_format) {
    // Usuń istniejące rozszerzenie
    char* dot = strrchr(output_file, '.');
    if (dot) {
        *dot = '\0';
    }
    
    // Dodaj odpowiednie rozszerzenie na podstawie formatu
    switch (output_format) {
        case FORMAT_C_ARRAY:
            strncat(output_file, ".h", 2);
            break;
        case FORMAT_RAW_DATA:
            strncat(output_file, ".hex", 4);
            break;
        case FORMAT_ASSEMBLER:
            strncat(output_file, ".inc", 4);
            break;
    }
}

// ============================================================================
// Funkcje konwersji obrazu
// ============================================================================

int pack_pixels_4bpp(uchar* grayscale_data, uchar* packed_data, int width, int height, int scan_direction, int pixel_order) {
    int packed_index = 0;
    
    if (scan_direction) {
        // Skanowanie poziome (wiersze)
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x += 2) {
                uchar pixel1 = grayscale_data[y * width + x];
                uchar pixel2 = (x + 1 < width) ? grayscale_data[y * width + x + 1] : 0;
                
                // Pakuj dwa 4-bitowe piksele w jeden bajt
                if (pixel_order) {
                    // Little endian: piksel1|piksel2
                    packed_data[packed_index] = (pixel1 << 4) | pixel2;
                } else {
                    // Big endian: piksel2|piksel1
                    packed_data[packed_index] = (pixel2 << 4) | pixel1;
                }
                packed_index++;
            }
        }
    } else {
        // Skanowanie pionowe (kolumny)
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y += 2) {
                uchar pixel1 = grayscale_data[y * width + x];
                uchar pixel2 = (y + 1 < height) ? grayscale_data[(y + 1) * width + x] : 0;
                
                // Pakuj dwa 4-bitowe piksele w jeden bajt
                if (pixel_order) {
                    // Little endian: piksel1|piksel2
                    packed_data[packed_index] = (pixel1 << 4) | pixel2;
                } else {
                    // Big endian: piksel2|piksel1
                    packed_data[packed_index] = (pixel2 << 4) | pixel1;
                }
                packed_index++;
            }
        }
    }
    
    return 1;
}

// Implementacja wzorca Strategy
int write_array(uchar* packed_data, int data_size, int width, int height, const char* array_name, const char* output_path, int output_format, int use_progmem) {
    FILE* file = fopen(output_path, "w");
    if (!file) {
        return 0;
    }

    int result = 0;
    switch (output_format) {
        case 0: // FORMAT_C_ARRAY
            result = format_c_array_write(packed_data, data_size, width, height, array_name, file, use_progmem);
            break;
        case 1: // FORMAT_RAW_DATA
            result = format_raw_data_write(packed_data, data_size, file);
            break;
        case 2: // FORMAT_ASSEMBLER
            result = format_assembler_write(packed_data, data_size, width, height, array_name, file);
            break;
        default:
            result = 0;
            break;
    }
    
    fclose(file);
    return result;
}

// Zapis formatu tablicy C (implementacja Strategy)
int format_c_array_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int use_progmem) {
    // Pobierz aktualną datę i czas
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char datetime_str[16];
    strftime(datetime_str, sizeof(datetime_str), "%Y%m%dT%H%M", tm_info);
    
    fprintf(file, "// Generated by BMP to 4bpp Array Converter v1.0 2025-09-28 (c) PTODT %s\n", datetime_str);
    fprintf(file, "// Image size: %dx%d\n", width, height);
    fprintf(file, "const unsigned char %s[%d]%s = {\n", array_name, data_size, generate_c_attributes(use_progmem));
    
    for (int i = 0; i < data_size; i++) {
        if (i % 16 == 0) {
            fprintf(file, "    ");
        }
        fprintf(file, "0x%02X", packed_data[i]);
        if (i < data_size - 1) {
            fprintf(file, ", ");
        }
        if ((i + 1) % 16 == 0) {
            fprintf(file, "\n");
        }
    }
    
    if (data_size % 16 != 0) {
        fprintf(file, "\n");
    }
    
    fprintf(file, "};\n");
    return 1;
}

// Zapis formatu surowych danych (implementacja Strategy)
int format_raw_data_write(uchar* packed_data, int data_size, FILE* file) {
    for (int i = 0; i < data_size; i++) {
        if (i % 16 == 0) {
            fprintf(file, "    ");
        }
        fprintf(file, "0x%02X", packed_data[i]);
        if (i < data_size - 1) {
            fprintf(file, ", ");
        }
        if ((i + 1) % 16 == 0) {
            fprintf(file, "\n");
        }
    }
    
    if (data_size % 16 != 0) {
        fprintf(file, "\n");
    }
    return 1;
}

// Zapis formatu assemblera (implementacja Strategy)
int format_assembler_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file) {
    // Pobierz aktualną datę i czas
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char datetime_str[16];
    strftime(datetime_str, sizeof(datetime_str), "%Y%m%dT%H%M", tm_info);
    
    fprintf(file, "; Generated by BMP to 4bpp Array Converter v1.0 2025-09-28 (c) PTODT %s\n", datetime_str);
    fprintf(file, "; Image size: %dx%d\n", width, height);
    fprintf(file, "%s:\n", array_name);
    
    for (int i = 0; i < data_size; i++) {
        if (i % 16 == 0) {
            fprintf(file, "    .db ");
        }
        fprintf(file, "$%02X", packed_data[i]);
        if ((i + 1) % 16 == 0) {
            fprintf(file, "\n");
        } else {
            fprintf(file, ", ");
        }
    }
    
    if (data_size % 16 != 0) {
        fprintf(file, "\n");
    }
    return 1;
}
