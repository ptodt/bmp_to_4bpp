/*****************************************************************************

    plik  : bmp_reader.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : implementacja us odczytu plików BMP

    licencja : MIT
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "bmp_reader.h"

/**
 * @brief Odczytuje nagłówki pliku BMP z pliku
 * 
 * @details Funkcja odczytuje główny nagłówek BMP (BMPHeader) i nagłówek
 * informacyjny (BMPInfoHeader) z otwartego pliku. Sprawdza poprawność
 * sygnatury pliku BMP ('BM') i zwraca status operacji.
 * 
 * @param file Wskaźnik do otwartego pliku BMP
 * @param header Wskaźnik do struktury BMPHeader (wyjściowa)
 * @param info Wskaźnik do struktury BMPInfoHeader (wyjściowa)
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Sprawdza sygnaturę 'BM' w nagłówku
 * @note Odczytuje struktury sekwencyjnie z pliku
 * @note Nie modyfikuje pozycji w pliku poza odczytem
 * 
 * @example
 * ```c
 * FILE* file = fopen("image.bmp", "rb");
 * BMPHeader header;
 * BMPInfoHeader info;
 * 
 * if (read_bmp_header(file, &header, &info)) {
 *     // Nagłówki zostały odczytane pomyślnie
 *     printf("Rozmiar: %dx%d, bpp: %d\n", info.width, info.height, info.bits_per_pixel);
 * }
 * ```
 */
int read_bmp_header(FILE* file, BMPHeader* header, BMPInfoHeader* info) {
    if (fread(header, sizeof(BMPHeader), 1, file) != 1) {
        return 0;
    }

    if (header->signature[0] != 'B' || header->signature[1] != 'M') {
        return 0;
    }

    if (fread(info, sizeof(BMPInfoHeader), 1, file) != 1) {
        return 0;
    }

    return 1;
}

/**
 * @brief Odczytuje dane obrazu BMP z pliku
 * 
 * @details Funkcja pozycjonuje plik na określonym offsetcie danych obrazu
 * i odczytuje surowe dane pikseli do bufora. Sprawdza czy odczytano
 * oczekiwaną liczbę bajtów i zwraca status operacji.
 * 
 * @param file Wskaźnik do otwartego pliku BMP
 * @param image_data Wskaźnik do bufora na dane obrazu (wyjściowy)
 * @param data_size Rozmiar danych do odczytania w bajtach
 * @param data_offset Offset początku danych obrazu w pliku
 * 
 * @return 1 w przypadku sukcesu, 0 w przypadku błędu
 * 
 * @note Pozycjonuje plik na data_offset przed odczytem
 * @note Sprawdza czy odczytano dokładnie data_size bajtów
 * @note Dane są odczytywane w formacie BGR (Blue-Green-Red)
 * 
 * @example
 * ```c
 * uchar* image_data = malloc(data_size);
 * if (read_bmp_image_data(file, image_data, data_size, data_offset)) {
 *     // Dane obrazu zostały odczytane pomyślnie
 * }
 * ```
 */
int read_bmp_image_data(FILE* file, uchar* image_data, int data_size, dword data_offset) {
    fseek(file, data_offset, SEEK_SET);
    if (fread(image_data, 1, data_size, file) != data_size) {
        return 0;
    }
    return 1;
}

/**
 * @brief Sprawdza czy format BMP jest obsługiwany przez program
 * 
 * @details Funkcja waliduje czy plik BMP ma format obsługiwany przez program.
 * Sprawdza głębię kolorów (musi być 24 lub 32 bpp) oraz kompresję
 * (musi być brak kompresji). Zwraca status zgodności z wymaganiami.
 * 
 * @param header Wskaźnik do nagłówka BMP (nieużywany w obecnej implementacji)
 * @param info Wskaźnik do nagłówka informacyjnego BMP
 * 
 * @return 1 jeśli format jest obsługiwany, 0 jeśli nie
 * 
 * @note Sprawdza głębię kolorów: 24 lub 32 bpp
 * @note Sprawdza kompresję: musi być 0 (brak kompresji)
 * @note Można rozszerzyć o dodatkowe walidacje w przyszłości
 * 
 * @example
 * ```c
 * BMPHeader header;
 * BMPInfoHeader info;
 * 
 * if (read_bmp_header(file, &header, &info) && validate_bmp_format(&header, &info)) {
 *     // Format BMP jest obsługiwany
 *     printf("Obsługiwany format: %d bpp\n", info.bits_per_pixel);
 * }
 * ```
 */
int validate_bmp_format(BMPHeader* header, BMPInfoHeader* info) {
    // Sprawdź czy to 24-bitowy lub 32-bitowy BMP
    if (info->bits_per_pixel != 24 && info->bits_per_pixel != 32) {
        return 0;
    }
    
    // Sprawdź czy kompresja jest obsługiwana (0 = brak kompresji)
    if (info->compression != 0) {
        return 0;
    }
    
    return 1;
}

/**
 * @brief Oblicza rozmiar wiersza w pliku BMP z wyrównaniem
 * 
 * @details Funkcja oblicza rzeczywisty rozmiar wiersza w pliku BMP,
 * uwzględniając wymagane wyrównanie do granicy 4-bajtowej. Każdy wiersz
 * w pliku BMP musi być wyrównany do wielokrotności 4 bajtów, co może
 * wymagać dodania padding'u na końcu wiersza.
 * 
 * @param width Szerokość obrazu w pikselach
 * @param bits_per_pixel Głębia kolorów w bitach na piksel
 * 
 * @return Rozmiar wiersza w bajtach (z wyrównaniem)
 * 
 * @note Wyrównanie do granicy 4-bajtowej jest wymagane przez format BMP
 * @note Wzór: ((width * bytes_per_pixel + 3) / 4) * 4
 * @note Padding jest dodawany automatycznie przez wzór
 * 
 * @example
 * ```c
 * int row_size = calculate_bmp_row_size(100, 24);  // 100 * 3 = 300, wyrównane do 300
 * int row_size2 = calculate_bmp_row_size(99, 24);  // 99 * 3 = 297, wyrównane do 300
 * int row_size3 = calculate_bmp_row_size(50, 32);  // 50 * 4 = 200, wyrównane do 200
 * ```
 */
int calculate_bmp_row_size(dword width, int bits_per_pixel) {
    int bytes_per_pixel = bits_per_pixel / 8;
    return ((width * bytes_per_pixel + 3) / 4) * 4; // Wyrównaj do granicy 4-bajtowej
}
