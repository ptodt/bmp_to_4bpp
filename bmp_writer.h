/*****************************************************************************

    plik  : bmp_writer.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.30
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : plik nagłówkowy dla zapisu plików BMP

    licencja : MIT
*****************************************************************************/

#ifndef BMP_WRITER_H
#define BMP_WRITER_H

#include "defs.h"
#include "bmp_defs.h"

// Prototypy funkcji zapisu BMP
int generate_1bpp_bmp(uchar* packed_data, PreviewContext* preview_ctx);
int generate_4bpp_bmp(uchar* packed_data, PreviewContext* preview_ctx);

#endif
