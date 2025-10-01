/*****************************************************************************

    plik  : defs.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : plik naglowkowy. definiuje najbardziej potrzebne rzeczy

    licencja : MIT
*****************************************************************************/

#ifndef __DEFS_H__
#define __DEFS_H__

typedef unsigned char uchar;
typedef unsigned short word;
typedef unsigned int dword;  // Użyj 32-bit dla kompatybilności z BMP
typedef word bool;
typedef char *string;

#define CTRLA 0x01
#define EOL   0x0D

#define PROGMEM __ATTR_PROGMEM__

// Stałe formatów wyjściowych
#define FORMAT_C_ARRAY     0  // Tablica C (.h) - pełna tablica C z deklaracją
#define FORMAT_RAW_DATA    1  // Surowe dane (.hex) - tylko dane bez deklaracji
#define FORMAT_ASSEMBLER   2  // Format assemblera (.inc)
#define FORMAT_MASM_ARRAY  3  // Format MASM z makrem .array (.inc)

// Stałe dla głębi kolorów
#define BITS_PER_PIXEL_4BPP  4  // 4 bits per pixel (domyślne)
#define BITS_PER_PIXEL_1BPP  1  // 1 bit per pixel

// Stałe dla metod ditheringu (tylko dla 1bpp)
#define DITHERING_NONE        0  // Bez ditheringu - proste progowanie
#define DITHERING_FLOYD       1  // Floyd-Steinberg dithering (domyślne)
#define DITHERING_ORDERED     2  // Ordered 8x8 dithering

// Typ formatu wyjściowego
typedef int OutputFormat;

// Kontekst konwersji
typedef struct {
    int scan_direction;        // 1 = poziomo (wiersze), 0 = pionowo (kolumny)
    int pixel_order;           // 1 = little endian (pixel1|pixel2), 0 = big endian (pixel2|pixel1)
    OutputFormat output_format; // Typ formatu wyjściowego
    int use_progmem;           // 1 = dodaj słowo kluczowe PROGMEM, 0 = bez PROGMEM
    char array_name[64];       // Nazwa tablicy wyjściowej
    int bits_per_pixel;        // 4 = 4bpp (domyślne), 1 = 1bpp
    int dithering_method;      // Metoda ditheringu (tylko dla 1bpp)
    int brightness;            // Jasność 0-100% (tylko dla 1bpp)
    int contrast;              // Kontrast 0-100% (tylko dla 1bpp)
    int generate_bmp;          // 1 = generuj BMP preview (tylko dla 1bpp)
    int invert;                // 1 = odwróć bity (zamień 0 na 1 i odwrotnie)
    int palette_variant;       // Wariant palety dla 1bpp (0=BW, 1=GRAY, 2=GREEN, 3=PORTFOLIO, 4=OLED_YELLOW, 5=CUSTOM)
    int palette_4bpp_variant;  // Wariant palety dla 4bpp (0=BW, 1=GRAY, 2=GREEN, 3=PORTFOLIO, 4=OLED_YELLOW, 5=CUSTOM)
    uchar custom_color_first[3];  // Pierwszy kolor w rampie niestandardowej (R,G,B)
    uchar custom_color_last[3];   // Ostatni kolor w rampie niestandardowej (R,G,B)
} ConversionContext;

// Kontekst podglądu BMP
typedef struct {
    int width;                 // Szerokość obrazu
    int height;                // Wysokość obrazu
    const char* output_path;   // Ścieżka do pliku wyjściowego
    int palette_variant;       // Wariant palety (0=BW, 1=GRAY, 2=GREEN, 3=PORTFOLIO, 4=OLED_YELLOW, 5=CUSTOM)
    uchar* custom_first;       // Pierwszy kolor w rampie niestandardowej (R,G,B)
    uchar* custom_last;        // Ostatni kolor w rampie niestandardowej (R,G,B)
    int scan_direction;        // 1 = poziomo (wiersze), 0 = pionowo (kolumny)
} PreviewContext;

#endif
