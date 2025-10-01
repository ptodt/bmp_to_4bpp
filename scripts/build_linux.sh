#!/bin/bash
# Skrypt do budowania wersji Linux za pomocą Docker
#
# Copyright (c) 2025 PTODT <https://ptodt.org.pl>
# Autor: Michal Kolodziejski (2:480/112.10)
# Data: 2025.10.01
# Licencja: MIT

# Wczytaj wersję z pliku VERSION
if [ -f "VERSION" ]; then
    VERSION=$(cat "VERSION" | tr -d '\n\r')
    if [ "$VERSION" = "PLEASE_CONFIGURE_VERSION" ]; then
        echo "BŁĄD: Wersja nie została skonfigurowana!"
        echo "Przed budowaniem skonfiguruj wersję w pliku VERSION w katalogu głównym"
        echo "Zobacz sekcję 'System wersjonowania' w readme.md"
        exit 1
    fi
else
    echo "BŁĄD: Plik VERSION nie istnieje!"
    echo "Przed budowaniem skonfiguruj wersję w pliku VERSION w katalogu głównym"
    echo "Zobacz sekcję 'System wersjonowania' w readme.md"
    exit 1
fi

echo "=========================================="
echo "-- BMP TO XBPP CONVERTER v$VERSION   --"
echo "-- LINUX BUILD SCRIPT                --"
echo "=========================================="
echo ""
echo "  CODE BY                                  "
echo "  ----.-.---.---.---.                      "
echo "      |-.---.   |--.|#==---.. .  .  .      "
echo "   |  | |   | --.   |#==---. . .           "
echo "  -'--'-'---'---'---'               2025.09"
echo ""

# Sprawdź czy Docker jest dostępny
if ! command -v docker &> /dev/null; then
    echo "BŁĄD: Docker nie jest zainstalowany!"
    echo "Zainstaluj Docker Desktop i spróbuj ponownie."
    exit 1
fi

# Przejdź do katalogu głównego projektu
cd "$(dirname "$0")/.."

echo "Budowanie wersji Linux (wersja: ${VERSION})..."

# Sprawdź czy wersja została poprawnie odczytana
if [ "${VERSION}" = "PLEASE_CONFIGURE_VERSION" ]; then
    echo "UWAGA: Wykryto problem z konfiguracją wersji!"
    echo "   Sprawdź czy plik VERSION istnieje i zawiera poprawny numer wersji."
    echo "   Przeczytaj sekcję 'System wersjonowania' w README.md"
    echo ""
fi

# Aktualizuj wersję
echo "Aktualizowanie wersji..."
./scripts/update_version.sh

if [ $? -ne 0 ]; then
    echo "BŁĄD: Błąd podczas aktualizacji wersji!"
    exit 1
fi

echo "Budowanie obrazu Docker..."
docker build -f scripts/Dockerfile.linux -t bmp_to_xbpp_builder .

if [ $? -ne 0 ]; then
    echo "BŁĄD: Błąd podczas budowania obrazu Docker!"
    exit 1
fi

echo "Obraz Docker zbudowany pomyślnie!"
echo ""

# Utwórz katalog release/${VERSION} jeśli nie istnieje
mkdir -p release/${VERSION}

# Skopiuj plik wykonywalny z kontenera
echo "Kopiowanie pliku wykonywalnego z kontenera..."
docker run --rm -v "$(pwd)/release/${VERSION}:/output" bmp_to_xbpp_builder cp bmp_to_xbpp /output/

if [ $? -ne 0 ]; then
    echo "BŁĄD: Błąd podczas kopiowania pliku wykonywalnego!"
    exit 1
fi

# Sprawdź czy plik został skopiowany
if [ ! -f "release/${VERSION}/bmp_to_xbpp" ]; then
    echo "BŁĄD: Błąd podczas kopiowania pliku wykonywalnego!"
    exit 1
fi

# Skopiuj pliki do katalogu release
echo "Kopiowanie plików..."
cp img/sample-image.bmp release/${VERSION}/
cp FILE_ID.DIZ release/${VERSION}/

# Utwórz archiwum ZIP
echo "Tworzenie archiwum ZIP..."
cd release/${VERSION}
zip bmp_to_xbpp_linux_x86.zip bmp_to_xbpp sample-image.bmp FILE_ID.DIZ

if [ $? -ne 0 ]; then
    echo "BŁĄD: Błąd podczas tworzenia archiwum ZIP!"
    exit 1
fi

# Usuń pliki po utworzeniu ZIP
echo "Usuwanie plików..."
rm bmp_to_xbpp sample-image.bmp

# Wróć do katalogu głównego
cd ../..

echo "Archiwum utworzone pomyślnie!"
echo ""

echo "Zawartość katalogu release/${VERSION}/:"
ls -la release/${VERSION}/

echo ""
echo "Budowanie wersji Linux zakończone pomyślnie!"
echo "Archiwum: release/${VERSION}/bmp_to_xbpp_linux_x86.zip"

