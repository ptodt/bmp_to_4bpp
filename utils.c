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
        case FORMAT_MASM_ARRAY:
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
int write_array(uchar* packed_data, int data_size, int width, int height, const char* array_name, const char* output_path, int output_format, int use_progmem, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    FILE* file = fopen(output_path, "w");
    if (!file) {
        return 0;
    }

    // Zastosuj inwersję do danych jeśli wymagane
    if (invert) {
        if (bits_per_pixel == BITS_PER_PIXEL_1BPP) {
            // Dla 1bpp: odwróć bity
            for (int i = 0; i < data_size; i++) {
                packed_data[i] = ~packed_data[i];
            }
        } else if (bits_per_pixel == BITS_PER_PIXEL_4BPP) {
            // Dla 4bpp: odwróć wartości pikseli (0-15 staje się 15-0)
            for (int i = 0; i < data_size; i++) {
                uchar byte = packed_data[i];
                uchar low_nibble = 15 - (byte & 0x0F);
                uchar high_nibble = 15 - ((byte >> 4) & 0x0F);
                packed_data[i] = (high_nibble << 4) | low_nibble;
            }
        }
    }

    int result = 0;
    switch (output_format) {
        case 0: // FORMAT_C_ARRAY
            result = format_c_array_write(packed_data, data_size, width, height, array_name, file, use_progmem, bits_per_pixel, dithering_method, brightness, contrast, invert);
            break;
        case 1: // FORMAT_RAW_DATA
            result = format_raw_data_write(packed_data, data_size, file, bits_per_pixel, dithering_method, brightness, contrast, invert);
            break;
        case 2: // FORMAT_ASSEMBLER
            result = format_assembler_write(packed_data, data_size, width, height, array_name, file, bits_per_pixel, dithering_method, brightness, contrast, invert);
            break;
        case 3: // FORMAT_MASM_ARRAY
            result = format_masm_array_write(packed_data, data_size, width, height, array_name, file, bits_per_pixel, dithering_method, brightness, contrast, invert);
            break;
        default:
            result = 0;
            break;
    }
    
    fclose(file);
    return result;
}

// Zapis formatu tablicy C (implementacja Strategy)
int format_c_array_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int use_progmem, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    // Pobierz aktualną datę i czas
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char datetime_str[16];
    strftime(datetime_str, sizeof(datetime_str), "%Y%m%dT%H%M", tm_info);
    
    fprintf(file, "// Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT %s\n", datetime_str);
    fprintf(file, "// Image size: %dx%d\n", width, height);
    fprintf(file, "// Format: %dbpp", bits_per_pixel);
    if (bits_per_pixel == BITS_PER_PIXEL_1BPP) {
        const char* dither_name = (dithering_method == DITHERING_FLOYD) ? "Floyd-Steinberg" :
                                 (dithering_method == DITHERING_ORDERED) ? "Ordered 8x8" : "None";
        fprintf(file, " (dithering: %s, brightness: %d%%, contrast: %d%%", dither_name, brightness, contrast);
        if (invert) {
            fprintf(file, ", inverted");
        }
        fprintf(file, ")");
    } else if (bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        if (brightness != 50 || contrast != 50) {
            fprintf(file, " (brightness: %d%%, contrast: %d%%", brightness, contrast);
            if (invert) {
                fprintf(file, ", inverted");
            }
            fprintf(file, ")");
        } else if (invert) {
            fprintf(file, " (inverted)");
        }
    }
    fprintf(file, "\n");
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
int format_raw_data_write(uchar* packed_data, int data_size, FILE* file, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    // Pobierz aktualną datę i czas
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char datetime_str[32];
    strftime(datetime_str, sizeof(datetime_str), "%Y%m%dT%H%M", tm_info);
    
    fprintf(file, "// Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT %s\n", datetime_str);
    fprintf(file, "// Format: %dbpp", bits_per_pixel);
    if (bits_per_pixel == BITS_PER_PIXEL_1BPP) {
        const char* dither_name = (dithering_method == DITHERING_FLOYD) ? "Floyd-Steinberg" :
                                 (dithering_method == DITHERING_ORDERED) ? "Ordered 8x8" : "None";
        fprintf(file, " (dithering: %s, brightness: %d%%, contrast: %d%%", dither_name, brightness, contrast);
        if (invert) {
            fprintf(file, ", inverted");
        }
        fprintf(file, ")");
    } else if (bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        if (brightness != 50 || contrast != 50) {
            fprintf(file, " (brightness: %d%%, contrast: %d%%", brightness, contrast);
            if (invert) {
                fprintf(file, ", inverted");
            }
            fprintf(file, ")");
        } else if (invert) {
            fprintf(file, " (inverted)");
        }
    }
    fprintf(file, "\n");
    
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
int format_assembler_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    // Pobierz aktualną datę i czas
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char datetime_str[16];
    strftime(datetime_str, sizeof(datetime_str), "%Y%m%dT%H%M", tm_info);
    
    fprintf(file, "; Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT %s\n", datetime_str);
    fprintf(file, "; Image size: %dx%d\n", width, height);
    fprintf(file, "; Format: %dbpp", bits_per_pixel);
    if (bits_per_pixel == BITS_PER_PIXEL_1BPP) {
        const char* dither_name = (dithering_method == DITHERING_FLOYD) ? "Floyd-Steinberg" :
                                 (dithering_method == DITHERING_ORDERED) ? "Ordered 8x8" : "None";
        fprintf(file, " (dithering: %s, brightness: %d%%, contrast: %d%%", dither_name, brightness, contrast);
        if (invert) {
            fprintf(file, ", inverted");
        }
        fprintf(file, ")");
    } else if (bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        if (brightness != 50 || contrast != 50) {
            fprintf(file, " (brightness: %d%%, contrast: %d%%", brightness, contrast);
            if (invert) {
                fprintf(file, ", inverted");
            }
            fprintf(file, ")");
        } else if (invert) {
            fprintf(file, " (inverted)");
        }
    }
    fprintf(file, "\n");
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

// Zapis formatu MASM z makrem .array (implementacja Strategy)
int format_masm_array_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    // Pobierz aktualną datę i czas
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char datetime_str[16];
    strftime(datetime_str, sizeof(datetime_str), "%Y%m%dT%H%M", tm_info);
    
    fprintf(file, "; Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT %s\n", datetime_str);
    fprintf(file, "; Image size: %dx%d\n", width, height);
    fprintf(file, "; Format: %dbpp", bits_per_pixel);
    if (bits_per_pixel == BITS_PER_PIXEL_1BPP) {
        const char* dither_name = (dithering_method == DITHERING_FLOYD) ? "Floyd-Steinberg" :
                                 (dithering_method == DITHERING_ORDERED) ? "Ordered 8x8" : "None";
        fprintf(file, " (dithering: %s, brightness: %d%%, contrast: %d%%", dither_name, brightness, contrast);
        if (invert) {
            fprintf(file, ", inverted");
        }
        fprintf(file, ")");
    } else if (bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        if (brightness != 50 || contrast != 50) {
            fprintf(file, " (brightness: %d%%, contrast: %d%%", brightness, contrast);
            if (invert) {
                fprintf(file, ", inverted");
            }
            fprintf(file, ")");
        } else if (invert) {
            fprintf(file, " (inverted)");
        }
    }
    fprintf(file, "\n");
    fprintf(file, ".array %s[%d].byte\n", array_name, data_size);
    
    for (int i = 0; i < data_size; i++) {
        if (i % 16 == 0) {
            if (i == 0) {
                fprintf(file, " "); // Tylko pierwsza linia ma wcięcie
            }
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
    
    fprintf(file, ".enda\n");
    return 1;
}

// ============================================================================
// Funkcje dla 1bpp
// ============================================================================

uchar scale_to_1bpp(int gray_value) {
    // Proste progowanie: jeśli wartość > 127 to 1, inaczej 0
    return (gray_value > 127) ? 1 : 0;
}

int convert_to_grayscale_1bpp(uchar* image_data, uchar* grayscale_data, int width, int height, int bytes_per_row, int dithering_method, int brightness, int contrast) {
    int bytes_per_pixel = 3; // Zakładamy 24-bit na razie
    
    // Najpierw konwertuj do skali szarości (0-255)
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
            
            // Zapisz w buforze skali szarości (od góry do dołu jak PIL)
            grayscale_data[y * width + x] = (uchar)gray;
        }
    }
    
    // Zastosuj regulację jasności i kontrastu
    if (!adjust_brightness_contrast(grayscale_data, width, height, brightness, contrast)) {
        return 0;
    }
    
    // Zastosuj odpowiedni dithering
    switch (dithering_method) {
        case DITHERING_FLOYD:
            if (!apply_floyd_steinberg_dithering(grayscale_data, width, height)) {
                return 0;
            }
            break;
        case DITHERING_ORDERED:
            if (!apply_ordered_dithering(grayscale_data, width, height)) {
                return 0;
            }
            break;
        case DITHERING_NONE:
        default:
            // Brak ditheringu - tylko progowanie
            break;
    }
    
    // Konwertuj do 1bpp (0 lub 1)
    for (int i = 0; i < width * height; i++) {
        grayscale_data[i] = scale_to_1bpp(grayscale_data[i]);
    }
    
    return 1;
}

int pack_pixels_1bpp(uchar* grayscale_data, uchar* packed_data, int width, int height, int scan_direction, int pixel_order) {
    int packed_index = 0;
    
    if (scan_direction) {
        // Skanowanie poziome (wiersze)
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x += 8) {
                uchar byte_value = 0;
                
                for (int bit = 0; bit < 8; bit++) {
                    int pixel_x = x + bit;
                    if (pixel_x < width) {
                        uchar pixel_value = grayscale_data[y * width + pixel_x];
                        if (pixel_value) {
                            if (pixel_order) {
                                // Little endian: bit 7 = pierwszy piksel (lewy)
                                byte_value |= (1 << (7 - bit));
                            } else {
                                // Big endian: bit 0 = pierwszy piksel (lewy)
                                byte_value |= (1 << bit);
                            }
                        }
                    }
                }
                
                packed_data[packed_index++] = byte_value;
            }
        }
    } else {
        // Skanowanie pionowe (kolumny)
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y += 8) {
                uchar byte_value = 0;
                
                for (int bit = 0; bit < 8; bit++) {
                    int pixel_y = y + bit;
                    if (pixel_y < height) {
                        uchar pixel_value = grayscale_data[pixel_y * width + x];
                        if (pixel_value) {
                            if (pixel_order) {
                                // Little endian: bit 0 = pierwszy piksel (góra)
                                // W trybie pionowym odwracamy kolejność bitów
                                byte_value |= (1 << (7 - bit));
                            } else {
                                // Big endian: bit 7 = pierwszy piksel (góra)
                                // W trybie pionowym odwracamy kolejność bitów
                                byte_value |= (1 << bit);
                            }
                        }
                    }
                }
                
                packed_data[packed_index++] = byte_value;
            }
        }
    }
    
    return 1;
}

// ============================================================================
// Funkcje ditheringu
// ============================================================================

int apply_floyd_steinberg_dithering(uchar* grayscale_data, int width, int height) {
    // Floyd-Steinberg dithering
    // Błędy są rozpraszane na piksele: prawo, lewo-dół, dół, prawo-dół
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int old_pixel = grayscale_data[y * width + x];
            int new_pixel = (old_pixel > 127) ? 255 : 0;
            int error = old_pixel - new_pixel;
            
            grayscale_data[y * width + x] = (uchar)new_pixel;
            
            // Rozprowadź błąd na sąsiednie piksele
            if (x + 1 < width) {
                int new_value = grayscale_data[y * width + (x + 1)] + (error * 7 / 16);
                grayscale_data[y * width + (x + 1)] = (uchar)((new_value < 0) ? 0 : (new_value > 255) ? 255 : new_value);
            }
            
            if (y + 1 < height) {
                if (x > 0) {
                    int new_value = grayscale_data[(y + 1) * width + (x - 1)] + (error * 3 / 16);
                    grayscale_data[(y + 1) * width + (x - 1)] = (uchar)((new_value < 0) ? 0 : (new_value > 255) ? 255 : new_value);
                }
                
                int new_value = grayscale_data[(y + 1) * width + x] + (error * 5 / 16);
                grayscale_data[(y + 1) * width + x] = (uchar)((new_value < 0) ? 0 : (new_value > 255) ? 255 : new_value);
                
                if (x + 1 < width) {
                    int new_value = grayscale_data[(y + 1) * width + (x + 1)] + (error * 1 / 16);
                    grayscale_data[(y + 1) * width + (x + 1)] = (uchar)((new_value < 0) ? 0 : (new_value > 255) ? 255 : new_value);
                }
            }
        }
    }
    
    return 1;
}

int apply_ordered_dithering(uchar* grayscale_data, int width, int height) {
    // Ordered 8x8 dithering - matryca Bayer 8x8
    static const uchar bayer_matrix[8][8] = {
        { 0, 32, 8, 40, 2, 34, 10, 42},
        {48, 16, 56, 24, 50, 18, 58, 26},
        {12, 44, 4, 36, 14, 46, 6, 38},
        {60, 28, 52, 20, 62, 30, 54, 22},
        { 3, 35, 11, 43, 1, 33, 9, 41},
        {51, 19, 59, 27, 49, 17, 57, 25},
        {15, 47, 7, 39, 13, 45, 5, 37},
        {63, 31, 55, 23, 61, 29, 53, 21}
    };
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int threshold = bayer_matrix[y % 8][x % 8];
            int pixel_value = grayscale_data[y * width + x];
            
            if (pixel_value > threshold) {
                grayscale_data[y * width + x] = 255;
            } else {
                grayscale_data[y * width + x] = 0;
            }
        }
    }
    
    return 1;
}

// Funkcja regulacji jasności i kontrastu
int adjust_brightness_contrast(uchar* grayscale_data, int width, int height, int brightness, int contrast) {
    // Konwersja procentów na współczynniki
    // Jasność: 0% = -255, 50% = 0, 100% = +255
    float brightness_factor = (brightness - 50.0f) * 5.1f; // 255/50 = 5.1
    
    // Kontrast: 0% = 0.0, 50% = 1.0, 100% = 2.0
    float contrast_factor = contrast / 50.0f;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel_index = y * width + x;
            int original_value = grayscale_data[pixel_index];
            
            // Zastosuj kontrast (względem środka skali 127.5)
            float adjusted_value = (original_value - 127.5f) * contrast_factor + 127.5f;
            
            // Zastosuj jasność
            adjusted_value += brightness_factor;
            
            // Ogranicz do zakresu 0-255
            if (adjusted_value < 0) adjusted_value = 0;
            if (adjusted_value > 255) adjusted_value = 255;
            
            grayscale_data[pixel_index] = (uchar)adjusted_value;
        }
    }
    
    return 1;
}


