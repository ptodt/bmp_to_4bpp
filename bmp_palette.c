/*****************************************************************************

    plik  : bmp_palette.c
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.30
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : implementacja funkcji generowania palet kolorów BMP

    licencja : MIT
*****************************************************************************/

#include "bmp_palette.h"

// Funkcja generowania palety z różnymi wariantami
void generate_palette(BMPColorEntry* palette, PaletteContext* context) {
    BMPColorEntry color0, color15;
    
    // Ustaw kolory bazowe (0 i ostatni) w zależności od wariantu
    switch (context->variant) {
        case PALETTE_BW:
            // Czarno-biała (domyślna)
            color0.blue = 0x00; color0.green = 0x00; color0.red = 0x00; color0.alpha = 0x00;
            color15.blue = 0xFF; color15.green = 0xFF; color15.red = 0xFF; color15.alpha = 0x00;
            break;
            
        case PALETTE_GRAY:
            // Szara
            color0.blue = 30; color0.green = 30; color0.red = 30; color0.alpha = 0x00;
            color15.blue = 128; color15.green = 128; color15.red = 128; color15.alpha = 0x00;
            break;
            
        case PALETTE_GREEN:
            // Zielona
            color0.blue = 120; color0.green = 170; color0.red = 170; color0.alpha = 0x00;
            color15.blue = 40; color15.green = 120; color15.red = 80; color15.alpha = 0x00;
            break;
            
        case PALETTE_PORTFOLIO:
            // Portfolio
            color0.blue = 144; color0.green = 238; color0.red = 144; color0.alpha = 0x00;
            color15.blue = 160; color15.green = 72; color15.red = 72; color15.alpha = 0x00;
            break;
            
        case PALETTE_OLED_YELLOW:
            // OLED żółty
            color0.blue = 20; color0.green = 20; color0.red = 20; color0.alpha = 0x00;
            color15.blue = 0; color15.green = 220; color15.red = 255; color15.alpha = 0x00;
            break;
            
        case PALETTE_CUSTOM:
            // Niestandardowa paleta
            color0.blue = context->custom_first[0]; color0.green = context->custom_first[1]; color0.red = context->custom_first[2]; color0.alpha = 0x00;
            color15.blue = context->custom_last[0]; color15.green = context->custom_last[1]; color15.red = context->custom_last[2]; color15.alpha = 0x00;
            break;
            
        default:
            // Domyślnie czarno-biała
            color0.blue = 0x00; color0.green = 0x00; color0.red = 0x00; color0.alpha = 0x00;
            color15.blue = 0xFF; color15.green = 0xFF; color15.red = 0xFF; color15.alpha = 0x00;
            break;
    }
    
    // Generuj paletę w zależności od liczby kolorów
    if (context->color_count == 2) {
        // Paleta 1bpp - tylko 2 kolory
        palette[0] = color0;
        palette[1] = color15;
    } else if (context->color_count == 16) {
        // Paleta 4bpp - interpolacja liniowa między color0 a color15
        for (int i = 0; i < 16; i++) {
            float factor = (float)i / 15.0f;  // 0.0 dla i=0, 1.0 dla i=15
            
            palette[i].blue = (uchar)(color0.blue + (color15.blue - color0.blue) * factor);
            palette[i].green = (uchar)(color0.green + (color15.green - color0.green) * factor);
            palette[i].red = (uchar)(color0.red + (color15.red - color0.red) * factor);
            palette[i].alpha = 0x00;
        }
    }
}