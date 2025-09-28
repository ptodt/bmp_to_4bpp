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
typedef unsigned int dword;  // Use 32-bit for BMP compatibility
typedef word bool;
typedef char *string;

#define CTRLA 0x01
#define EOL   0x0D

#define PROGMEM __ATTR_PROGMEM__

// Stałe formatów wyjściowych
#define FORMAT_C_ARRAY     0  // Tablica C (.h) - pełna tablica C z deklaracją
#define FORMAT_RAW_DATA    1  // Surowe dane (.hex) - tylko dane bez deklaracji
#define FORMAT_ASSEMBLER   2  // Format assemblera (.inc)

// Typ formatu wyjściowego
typedef int OutputFormat;

// Kontekst konwersji
typedef struct {
    int scan_direction;        // 1 = poziomo (wiersze), 0 = pionowo (kolumny)
    int pixel_order;           // 1 = little endian (pixel1|pixel2), 0 = big endian (pixel2|pixel1)
    OutputFormat output_format; // Typ formatu wyjściowego
    int use_progmem;           // 1 = dodaj słowo kluczowe PROGMEM, 0 = bez PROGMEM
    char array_name[64];       // Nazwa tablicy wyjściowej
} ConversionContext;

#endif
