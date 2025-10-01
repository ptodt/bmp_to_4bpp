#!/bin/bash
# Skrypt do aktualizacji wersji na podstawie pliku VERSION
#
# Copyright (c) 2025 PTODT <https://ptodt.org.pl>
# Autor: Michal Kolodziejski (2:480/112.10)
# Data: 2025.10.01
# Licencja: MIT

# Przejdź do katalogu głównego projektu (jeden poziom wyżej)
cd "$(dirname "$0")/.."

VERSION_FILE="VERSION"
VERSION_HEADER="version.h"
FILE_ID_DIZ="FILE_ID.DIZ"

if [ ! -f "$VERSION_FILE" ]; then
    echo "Błąd: Plik $VERSION_FILE nie istnieje"
    exit 1
fi

VERSION=$(cat "$VERSION_FILE" | tr -d '\n\r')
DATE=$(date +%Y-%m-%d)
BUILD_TIME=$(date +%Y%m%dT%H%M%S)
BUILD_DATETIME=$(date +%Y-%m-%d\ %H:%M:%S)

cat > "$VERSION_HEADER" << EOF
/*****************************************************************************
    plik  : version.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : $DATE
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : automatycznie generowany plik z informacjami o wersji

    licencja : MIT
*****************************************************************************/

#ifndef VERSION_H
#define VERSION_H

#define VERSION_STRING "$VERSION"
#define VERSION_DATE "$DATE"
#define BUILD_TIME "$BUILD_TIME"
#define BUILD_DATETIME "$BUILD_DATETIME"
#define COPYRIGHT_STRING "PTODT"

#endif
EOF

# Wygeneruj FILE_ID.DIZ
cat > "$FILE_ID_DIZ" << EOF
          BMP to xbpp Array Converter 
          $VERSION ($BUILD_DATETIME)
       (c) PTODT (https://ptodt.org.pl)

CODE BY                                      
    ----.-.---.---.---.                      
        |-.---.   |--.|#==---.. .  .  .      
     |  | |   | --.   |#==---. . .           
    -'--'-'---'---'---'              2025.09

Usage: https://github.com/ptodt/bmp_to_4bpp

Converts  a  BMP  image   to  an  array  (C,
assembler include file, hex values separated
by  commas)  containing packed  pixel  data.
Supports  both 4bpp  (4 bits per  pixel) and
1bpp (1 bit per pixel) formats.

 -=# Didn't like it? Mind you own values #=-

EOF

echo "Zaktualizowano $VERSION_HEADER z wersją $VERSION ($DATE) - build time: $BUILD_TIME"
echo "Wygenerowano $FILE_ID_DIZ z wersją $VERSION ($BUILD_DATETIME)"
