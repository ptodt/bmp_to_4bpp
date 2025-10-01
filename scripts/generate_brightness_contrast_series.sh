#!/bin/bash
# generate_brightness_contrast_series.sh
# Generuje serię obrazów BMP z różnymi kombinacjami jasności i kontrastu
#
# Copyright (c) 2025 PTODT <https://ptodt.org.pl>
# Autor: Michal Kolodziejski (2:480/112.10)
# Data: 2025.10.01
# Licencja: MIT

# Przejdź do katalogu głównego projektu (jeden poziom wyżej)
cd "$(dirname "$0")/.."

# Wczytaj wersję z pliku VERSION
if [ -f "VERSION" ]; then
    VERSION=$(cat "VERSION" | tr -d '\n\r')
else
    VERSION="1.0.3"
fi

# Konfiguracja
OUTPUT_DIR="preview"

echo "------------------------------------"
echo "-- BMP TO XBPP CONVERTER v$VERSION   --"
echo "-- BRIGHTNESS/CONTRAST SERIES GEN --"
echo "------------------------------------"
echo ""
echo "  CODE BY                                  "
echo "  ----.-.---.---.---.                      "
echo "      |-.---.   |--.|#==---.. .  .  .      "
echo "   |  | |   | --.   |#==---. . .           "
echo "  -'--'-'---'---'---'               2025.09"
echo ""

# Utwórz katalog wyjściowy
mkdir -p "$OUTPUT_DIR"

echo "- GENERATING BMP FILES WITH BRIGHTNESS/CONTRAST COMBINATIONS"
echo ""

# Generuj wszystkie kombinacje jasności i kontrastu (0-100% co 10%)
for br in 0 10 20 30 40 50 60 70 80 90 100; do
  for ct in 0 10 20 30 40 50 60 70 80 90 100; do
    echo "Generating br${br}_ct${ct}..."
    ./bmp_to_xbpp -1 -d floyd -br $br -ct $ct --bmp img/sample-image.bmp "$OUTPUT_DIR/br${br}_ct${ct}" > /dev/null 2>&1
  done
done

echo ""
echo "- DONE... FINISHED RESULTS ARE IN [$OUTPUT_DIR/] FOLDER"
count=$(ls "$OUTPUT_DIR"/*.bmp 2>/dev/null | wc -l)
echo "Generated $count images in $OUTPUT_DIR/ directory"
