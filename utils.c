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

/**
 * @brief Konwertuje wartości RGB na skalę szarości
 * 
 * @details Funkcja konwertuje pojedynczy piksel z formatu RGB na skalę szarości
 * używając standardowej formuły luminancji. Wzór uwzględnia różną wrażliwość
 * ludzkiego oka na różne kolory (zielony jest postrzegany jako najjaśniejszy).
 * 
 * @param r Wartość składowej czerwonej (0-255)
 * @param g Wartość składowej zielonej (0-255) 
 * @param b Wartość składowej niebieskiej (0-255)
 * 
 * @return Wartość w skali szarości (0-255)
 * 
 * @note Wzór: 0.299*R + 0.587*G + 0.114*B
 * 
 * @example
 * ```c
 * uchar gray = convert_rgb_to_grayscale(255, 128, 64);
 * // gray = 0.299*255 + 0.587*128 + 0.114*64 = 76 + 75 + 7 = 158
 * ```
 */
int convert_rgb_to_grayscale(uchar r, uchar g, uchar b) {
    // Konwersja do skali szarości: 0.299*R + 0.587*G + 0.114*B
    return (int)(0.299f * r + 0.587f * g + 0.114f * b);
}

/**
 * @brief Skaluje wartość w skali szarości do formatu 4bpp
 * 
 * @details Funkcja konwertuje wartość w skali szarości (0-255) na format 4bpp (0-15).
 * Używa dzielenia całkowitego przez 16, co daje 16 równomiernie rozłożonych poziomów
 * szarości. Wartości powyżej 240 są obcinane do maksymalnej wartości 15.
 * 
 * @param gray_value Wartość w skali szarości (0-255)
 * 
 * @return Wartość w formacie 4bpp (0-15)
 * 
 * @note Mapowanie: 0-15→0, 16-31→1, ..., 240-255→15
 * 
 * @example
 * ```c
 * uchar gray4 = scale_to_4bpp(128);  // 128/16 = 8
 * uchar gray4 = scale_to_4bpp(255);  // 255/16 = 15 (obcięte)
 * uchar gray4 = scale_to_4bpp(0);    // 0/16 = 0
 * ```
 */
uchar scale_to_4bpp(int gray_value) {
    // Skaluj do 4bpp (0-15) - dzielenie całkowite jak w Pythonie
    uchar gray_4bpp = (uchar)(gray_value / 16);
    if (gray_4bpp > 15) gray_4bpp = 15;
    return gray_4bpp;
}

/**
 * @brief Konwertuje obraz BMP na skalę szarości w formacie 4bpp
 * 
 * @details Funkcja konwertuje cały obraz BMP z formatu RGB na skalę szarości 4bpp.
 * Przetwarza obraz piksel po piksel, konwertując każdy piksel RGB na wartość
 * w skali szarości, a następnie skalując do formatu 4bpp (0-15). Obsługuje
 * standardowy format BMP z odwróconym porządkiem wierszy (od dołu do góry).
 * 
 * @param image_data Wskaźnik do danych obrazu BMP (format BGR)
 * @param grayscale_data Wskaźnik do bufora wyjściowego (format 4bpp)
 * @param width Szerokość obrazu w pikselach
 * @param height Wysokość obrazu w pikselach
 * @param bytes_per_row Liczba bajtów na wiersz w obrazie BMP (z padding)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Obraz jest odwracany w pionie (BMP → PIL format)
 * @note Zakłada format 24-bit RGB (3 bajty na piksel)
 * 
 * @example
 * ```c
 * uchar* bmp_data = malloc(file_size);
 * uchar* gray_data = malloc(width * height);
 * 
 * // Wczytaj dane BMP
 * fread(bmp_data, 1, file_size, file);
 * 
 * // Konwertuj na skalę szarości 4bpp
 * if (convert_to_grayscale_4bpp(bmp_data, gray_data, width, height, row_size)) {
 *     // gray_data zawiera teraz dane w formacie 4bpp
 * }
 * ```
 */
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

// Funkcja generująca nagłówek dla wszystkich formatów
/**
 * @brief Generuje nagłówek pliku dla wszystkich formatów wyjściowych
 * 
 * @details Funkcja generuje spójny nagłówek komentarza dla wszystkich formatów
 * wyjściowych (C array, raw data, assembler, MASM). Nagłówek zawiera informacje
 * o wersji programu, czasie generowania, parametrach konwersji i metadanych obrazu.
 * Automatycznie wybiera odpowiedni prefiks komentarza (// lub ;) na podstawie
 * typu formatu wyjściowego.
 * 
 * @param file Wskaźnik do otwartego pliku wyjściowego
 * @param ctx Wskaźnik do struktury HeaderContext z parametrami nagłówka
 * 
 * @note Generuje aktualny czas w formacie YYYYMMDDTHHMMSS
 * @note Pomija informacje o rozmiarze obrazu jeśli width=0 lub height=0
 * @note Automatycznie wybiera prefiks komentarza na podstawie ctx->is_assembler
 * 
 * @example
 * ```c
 * HeaderContext ctx = {256, 64, 1, DITHERING_FLOYD, 60, 70, 1, 0};
 * FILE* file = fopen("output.h", "w");
 * write_file_header(file, &ctx);
 * // Generuje: // Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:30:00)...
 * ```
 */
void write_file_header(FILE* file, HeaderContext* ctx) {
    // Pobierz aktualną datę i czas generowania pliku
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char generation_time[16];
    strftime(generation_time, sizeof(generation_time), "%Y%m%dT%H%M%S", tm_info);
    
    const char* comment_prefix = ctx->is_assembler ? ";" : "//";
    
    fprintf(file, "%s Generated by BMP to xbpp Array Converter %s (%s) (c) %s (https://ptodt.org.pl)\n", 
            comment_prefix, VERSION_STRING, BUILD_DATETIME, COPYRIGHT_STRING);
    fprintf(file, "%s Created on: %s\n", comment_prefix, generation_time);
    
    if (ctx->width > 0 && ctx->height > 0) {
        fprintf(file, "%s Image size: %dx%d\n", comment_prefix, ctx->width, ctx->height);
    }
    
    fprintf(file, "%s Format: %dbpp", comment_prefix, ctx->bits_per_pixel);
    
    if (ctx->bits_per_pixel == BITS_PER_PIXEL_1BPP) {
        const char* dither_name = (ctx->dithering_method == DITHERING_FLOYD) ? "Floyd-Steinberg" :
                                 (ctx->dithering_method == DITHERING_ORDERED) ? "Ordered 8x8" : "None";
        fprintf(file, " (dithering: %s, brightness: %d%%, contrast: %d%%", dither_name, ctx->brightness, ctx->contrast);
        if (ctx->invert) {
            fprintf(file, ", inverted");
        }
        fprintf(file, ")");
    } else if (ctx->bits_per_pixel == BITS_PER_PIXEL_4BPP) {
        if (ctx->brightness != 50 || ctx->contrast != 50) {
            fprintf(file, " (brightness: %d%%, contrast: %d%%", ctx->brightness, ctx->contrast);
            if (ctx->invert) {
                fprintf(file, ", inverted");
            }
            fprintf(file, ")");
        } else if (ctx->invert) {
            fprintf(file, " (inverted)");
        }
    }
    fprintf(file, "\n");
}

/**
 * @brief Ustawia domyślne rozszerzenie pliku na podstawie formatu wyjściowego
 * 
 * @details Funkcja automatycznie dodaje odpowiednie rozszerzenie pliku na podstawie
 * wybranego formatu wyjściowego. Najpierw usuwa istniejące rozszerzenie (jeśli istnieje),
 * a następnie dodaje nowe zgodnie z konwencją nazewnictwa dla każdego formatu.
 * 
 * @param output_file Nazwa pliku wyjściowego (modyfikowana w miejscu)
 * @param output_format Format wyjściowy (FORMAT_C_ARRAY, FORMAT_RAW_DATA, etc.)
 * 
 * @note Modyfikuje string w miejscu - nie alokuje nowej pamięci
 * @note Mapowanie: C_ARRAY→.h, RAW_DATA→.hex, ASSEMBLER→.inc, MASM_ARRAY→.inc
 * 
 * @example
 * ```c
 * char filename[256] = "my_image";
 * set_default_extension(filename, FORMAT_C_ARRAY);
 * // filename = "my_image.h"
 * 
 * set_default_extension(filename, FORMAT_RAW_DATA);
 * // filename = "my_image.hex"
 * ```
 */
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

/**
 * @brief Pakuje piksele 4bpp do bajtów z obsługą różnych kierunków skanowania
 * 
 * @details Funkcja konwertuje piksele 4bpp (wartości 0-15) na spakowane bajty,
 * gdzie każdy bajt zawiera dwa piksele. Obsługuje skanowanie poziome (wiersze)
 * i pionowe (kolumny) oraz różne porządki bajtów (little/big endian).
 * 
 * @param grayscale_data Wskaźnik do danych obrazu w skali szarości 4bpp (0-15)
 * @param packed_data Wskaźnik do bufora wyjściowego na spakowane dane
 * @param width Szerokość obrazu w pikselach
 * @param height Wysokość obrazu w pikselach
 * @param scan_direction Kierunek skanowania (1=poziomy, 0=pionowy)
 * @param pixel_order Porządek pikseli w bajcie (1=little endian, 0=big endian)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Skanowanie poziome: wiersz po wierszu, 2 piksele na bajt
 * @note Skanowanie pionowe: kolumna po kolumnie, 2 piksele na bajt
 * @note Little endian: piksel1 w górnych 4 bitach, piksel2 w dolnych
 * @note Big endian: piksel2 w górnych 4 bitach, piksel1 w dolnych
 * 
 * @example
 * ```c
 * uchar gray_data[256*64];  // Obraz 256x64 w skali szarości 4bpp
 * uchar packed[128*64];     // Bufor na spakowane dane (połowa rozmiaru)
 * 
 * if (pack_pixels_4bpp(gray_data, packed, 256, 64, 1, 1)) {
 *     // Dane zostały spakowane pomyślnie
 * }
 * ```
 */
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

/**
 * @brief Zapisuje spakowane dane do pliku w wybranym formacie
 * 
 * @details Implementuje wzorzec Strategy do zapisu danych w różnych formatach
 * wyjściowych (C array, raw data, assembler, MASM). Funkcja obsługuje inwersję
 * danych, generuje odpowiednie nagłówki i formatuje dane zgodnie z wybranym
 * standardem. Automatycznie wybiera odpowiedni format na podstawie parametru
 * output_format.
 * 
 * @param packed_data Wskaźnik do spakowanych danych obrazu
 * @param data_size Rozmiar danych w bajtach
 * @param width Szerokość obrazu w pikselach
 * @param height Wysokość obrazu w pikselach
 * @param array_name Nazwa tablicy w pliku wyjściowym
 * @param output_path Ścieżka do pliku wyjściowego
 * @param output_format Format wyjściowy (FORMAT_C_ARRAY, FORMAT_RAW_DATA, etc.)
 * @param use_progmem Czy używać atrybutu PROGMEM (tylko dla C array)
 * @param bits_per_pixel Głębia kolorów (1 lub 4 bpp)
 * @param dithering_method Metoda ditheringu (tylko dla 1bpp)
 * @param brightness Jasność 0-100% (tylko dla 1bpp)
 * @param contrast Kontrast 0-100% (tylko dla 1bpp)
 * @param invert Czy odwrócić bity danych (1=tak, 0=nie)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Modyfikuje dane w miejscu jeśli invert=1
 * @note Generuje spójne nagłówki dla wszystkich formatów
 * @note Obsługuje różne formaty komentarzy (// dla C, ; dla assemblera)
 * 
 * @example
 * ```c
 * uchar data[1024];
 * if (write_array(data, 1024, 64, 32, "my_image", "output.h", 
 *                 FORMAT_C_ARRAY, 1, 4, 0, 50, 50, 0)) {
 *     // Plik został wygenerowany pomyślnie
 * }
 * ```
 */
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

/**
 * @brief Zapisuje dane w formacie tablicy C z obsługą PROGMEM
 * 
 * @details Funkcja generuje plik C z tablicą const unsigned char zawierającą
 * spakowane dane obrazu. Obsługuje atrybut PROGMEM dla mikrokontrolerów AVR,
 * formatuje dane w wierszach po 16 bajtów dla czytelności i generuje
 * odpowiedni nagłówek z metadanymi obrazu.
 * 
 * @param packed_data Wskaźnik do spakowanych danych obrazu
 * @param data_size Rozmiar danych w bajtach
 * @param width Szerokość obrazu w pikselach
 * @param height Wysokość obrazu w pikselach
 * @param array_name Nazwa tablicy w kodzie C
 * @param file Wskaźnik do otwartego pliku wyjściowego
 * @param use_progmem Czy dodać atrybut PROGMEM (1=tak, 0=nie)
 * @param bits_per_pixel Głębia kolorów (1 lub 4 bpp)
 * @param dithering_method Metoda ditheringu (tylko dla 1bpp)
 * @param brightness Jasność 0-100% (tylko dla 1bpp)
 * @param contrast Kontrast 0-100% (tylko dla 1bpp)
 * @param invert Czy odwrócić bity danych (1=tak, 0=nie)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Formatuje dane w wierszach po 16 bajtów z wcięciem
 * @note Używa prefiksu 0x dla wartości szesnastkowych
 * @note Dodaje przecinki między wartościami i średnik na końcu
 * 
 * @example
 * ```c
 * FILE* file = fopen("image.h", "w");
 * format_c_array_write(data, 1024, 64, 32, "my_image", file, 1, 4, 0, 50, 50, 0);
 * // Generuje: const unsigned char my_image[1024] PROGMEM = { 0x12, 0x34, ... };
 * ```
 */
int format_c_array_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int use_progmem, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    HeaderContext header_ctx = {width, height, bits_per_pixel, dithering_method, brightness, contrast, invert, 0};
    write_file_header(file, &header_ctx);
    fprintf(file, "const unsigned char %s[%d]%s = {\n", array_name, data_size, ((use_progmem) ? " PROGMEM" : ""));
    
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

/**
 * @brief Zapisuje dane w formacie surowych wartości szesnastkowych
 * 
 * @details Funkcja generuje plik z surowymi danymi w formacie szesnastkowym
 * bez deklaracji tablicy C. Formatuje dane w wierszach po 16 bajtów dla
 * czytelności i generuje odpowiedni nagłówek z metadanymi obrazu.
 * 
 * @param packed_data Wskaźnik do spakowanych danych obrazu
 * @param data_size Rozmiar danych w bajtach
 * @param file Wskaźnik do otwartego pliku wyjściowego
 * @param bits_per_pixel Głębia kolorów (1 lub 4 bpp)
 * @param dithering_method Metoda ditheringu (tylko dla 1bpp)
 * @param brightness Jasność 0-100% (tylko dla 1bpp)
 * @param contrast Kontrast 0-100% (tylko dla 1bpp)
 * @param invert Czy odwrócić bity danych (1=tak, 0=nie)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Formatuje dane w wierszach po 16 bajtów z wcięciem
 * @note Używa prefiksu 0x dla wartości szesnastkowych
 * @note Nie generuje deklaracji tablicy - tylko surowe dane
 * 
 * @example
 * ```c
 * FILE* file = fopen("image.hex", "w");
 * format_raw_data_write(data, 1024, file, 4, 0, 50, 50, 0);
 * // Generuje: 0x12, 0x34, 0x56, 0x78, ...
 * ```
 */
int format_raw_data_write(uchar* packed_data, int data_size, FILE* file, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    HeaderContext header_ctx = {0, 0, bits_per_pixel, dithering_method, brightness, contrast, invert, 0};
    write_file_header(file, &header_ctx);
    
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

/**
 * @brief Zapisuje dane w formacie assemblera z dyrektywami .db
 * 
 * @details Funkcja generuje plik assemblera z etykietą i dyrektywami .db
 * zawierającymi spakowane dane obrazu. Używa składni assemblera z prefiksem
 * $ dla wartości szesnastkowych i formatuje dane w wierszach po 16 bajtów.
 * 
 * @param packed_data Wskaźnik do spakowanych danych obrazu
 * @param data_size Rozmiar danych w bajtach
 * @param width Szerokość obrazu w pikselach
 * @param height Wysokość obrazu w pikselach
 * @param array_name Nazwa etykiety w kodzie assemblera
 * @param file Wskaźnik do otwartego pliku wyjściowego
 * @param bits_per_pixel Głębia kolorów (1 lub 4 bpp)
 * @param dithering_method Metoda ditheringu (tylko dla 1bpp)
 * @param brightness Jasność 0-100% (tylko dla 1bpp)
 * @param contrast Kontrast 0-100% (tylko dla 1bpp)
 * @param invert Czy odwrócić bity danych (1=tak, 0=nie)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Używa prefiksu $ dla wartości szesnastkowych
 * @note Generuje etykietę i dyrektywy .db
 * @note Formatuje dane w wierszach po 16 bajtów
 * 
 * @example
 * ```c
 * FILE* file = fopen("image.inc", "w");
 * format_assembler_write(data, 1024, 64, 32, "my_image", file, 4, 0, 50, 50, 0);
 * // Generuje: my_image:\n    .db $12, $34, $56, $78, ...
 * ```
 */
int format_assembler_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    HeaderContext header_ctx = {width, height, bits_per_pixel, dithering_method, brightness, contrast, invert, 1};
    write_file_header(file, &header_ctx);
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

/**
 * @brief Zapisuje dane w formacie MASM z makrem .array
 * 
 * @details Funkcja generuje plik MASM (Microsoft Macro Assembler) z makrem
 * .array zawierającym spakowane dane obrazu. Używa składni MASM z prefiksem
 * $ dla wartości szesnastkowych i formatuje dane w wierszach po 16 bajtów.
 * 
 * @param packed_data Wskaźnik do spakowanych danych obrazu
 * @param data_size Rozmiar danych w bajtach
 * @param width Szerokość obrazu w pikselach
 * @param height Wysokość obrazu w pikselach
 * @param array_name Nazwa tablicy w kodzie MASM
 * @param file Wskaźnik do otwartego pliku wyjściowego
 * @param bits_per_pixel Głębia kolorów (1 lub 4 bpp)
 * @param dithering_method Metoda ditheringu (tylko dla 1bpp)
 * @param brightness Jasność 0-100% (tylko dla 1bpp)
 * @param contrast Kontrast 0-100% (tylko dla 1bpp)
 * @param invert Czy odwrócić bity danych (1=tak, 0=nie)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Używa prefiksu $ dla wartości szesnastkowych
 * @note Generuje makro .array z deklaracją rozmiaru
 * @note Formatuje dane w wierszach po 16 bajtów
 * 
 * @example
 * ```c
 * FILE* file = fopen("image.inc", "w");
 * format_masm_array_write(data, 1024, 64, 32, "my_image", file, 4, 0, 50, 50, 0);
 * // Generuje: .array my_image[1024].byte\n $12, $34, $56, $78, ...
 * ```
 */
int format_masm_array_write(uchar* packed_data, int data_size, int width, int height, const char* array_name, FILE* file, int bits_per_pixel, int dithering_method, int brightness, int contrast, int invert) {
    HeaderContext header_ctx = {width, height, bits_per_pixel, dithering_method, brightness, contrast, invert, 1};
    write_file_header(file, &header_ctx);
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
                                byte_value |= (1 << bit);
                            } else {
                                // Big endian: bit 7 = pierwszy piksel (góra)
                                byte_value |= (1 << (7 - bit));
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

/**
 * @brief Stosuje algorytm ditheringu Floyd-Steinberg
 * 
 * @details Implementuje klasyczny algorytm ditheringu Floyd-Steinberg, który
 * rozprasza błędy kwantyzacji na sąsiednie piksele. Dla każdego piksela
 * oblicza błąd między oryginalną wartością a skwantowaną (0 lub 255),
 * a następnie rozdziela ten błąd na 4 sąsiednie piksele według ustalonej
 * matrycy wag. Daje to naturalny efekt ditheringu bez widocznych wzorów.
 * 
 * @param grayscale_data Wskaźnik do danych obrazu w skali szarości (0-255)
 * @param width Szerokość obrazu w pikselach
 * @param height Wysokość obrazu w pikselach
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Modyfikuje dane wejściowe w miejscu
 * @note Matryca wag: [0 0 7/16] [3/16 5/16 1/16]
 * @note Próg kwantyzacji: 127 (powyżej → 255, poniżej → 0)
 * 
 * @example
 * ```c
 * uchar image[256*64];  // Obraz 256x64 w skali szarości
 * 
 * // Zastosuj dithering Floyd-Steinberg
 * if (apply_floyd_steinberg_dithering(image, 256, 64)) {
 *     // Obraz zawiera teraz dithering, gotowy do konwersji 1bpp
 * }
 * ```
 */
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


