/*****************************************************************************

    plik  : bmp_palette.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.30
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : plik nagłówkowy dla generowania palet kolorów BMP

    licencja : MIT
*****************************************************************************/

#ifndef BMP_PALETTE_H
#define BMP_PALETTE_H

#include "defs.h"
#include "bmp_defs.h"

// Definicje wariantów palet
#define PALETTE_BW        0  // Czarno-biała (domyślna)
#define PALETTE_GRAY      1  // Szara
#define PALETTE_GREEN     2  // Zielona
#define PALETTE_PORTFOLIO 3  // Portfolio
#define PALETTE_OLED_YELLOW 4  // OLED żółty
#define PALETTE_CUSTOM    5  // Niestandardowa

// Struktura kontekstu palety
typedef struct {
    int variant;              // Wariant palety (0-5)
    int color_count;          // Liczba kolorów do wygenerowania (2 dla 1bpp, 16 dla 4bpp)
    uchar custom_first[3];    // Pierwszy kolor w rampie niestandardowej (R,G,B)
    uchar custom_last[3];     // Ostatni kolor w rampie niestandardowej (R,G,B)
} PaletteContext;

// Prototyp funkcji generowania palety
void generate_palette(BMPColorEntry* palette, PaletteContext* context);

#endif
