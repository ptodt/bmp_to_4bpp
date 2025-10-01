#!/bin/bash
# Skrypt do budowania wersji macOS
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
echo "-- MACOS BUILD SCRIPT                 --"
echo "=========================================="
echo ""
echo "  CODE BY                                  "
echo "  ----.-.---.---.---.                      "
echo "      |-.---.   |--.|#==---.. .  .  .      "
echo "   |  | |   | --.   |#==---. . .           "
echo "  -'--'-'---'---'---'               2025.09"
echo ""

# Sprawdź czy jesteśmy na macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "BŁĄD: Ten skrypt jest przeznaczony dla macOS!"
    echo "Aktualny system: $OSTYPE"
    exit 1
fi

# Sprawdź czy make jest dostępny
if ! command -v make &> /dev/null; then
    echo "BŁĄD: Make nie jest zainstalowany!"
    echo "Zainstaluj Xcode Command Line Tools: xcode-select --install"
    exit 1
fi

# Sprawdź czy gcc jest dostępny
if ! command -v gcc &> /dev/null; then
    echo "BŁĄD: GCC nie jest zainstalowany!"
    echo "Zainstaluj Xcode Command Line Tools: xcode-select --install"
    exit 1
fi

# Przejdź do katalogu głównego projektu
cd "$(dirname "$0")/.."

echo "Budowanie wersji macOS (wersja: ${VERSION})..."

# Sprawdź czy wersja została poprawnie odczytana
if [ "${VERSION}" = "PLEASE_CONFIGURE_VERSION" ]; then
    echo "UWAGA: Wykryto problem z konfiguracją wersji!"
    echo "   Sprawdź czy plik VERSION istnieje i zawiera poprawny numer wersji."
    echo "   Przeczytaj sekcję 'System wersjonowania' w README.md"
    echo ""
fi

# Aktualizuj wersję i skompiluj projekt
echo "Aktualizowanie wersji i kompilacja..."
make clean && make

if [ $? -ne 0 ]; then
    echo "BŁĄD: Błąd podczas kompilacji!"
    exit 1
fi

echo "Kompilacja zakończona pomyślnie!"
echo ""

# Utwórz katalog release/${VERSION} jeśli nie istnieje
mkdir -p release/${VERSION}

# Skopiuj plik wykonywalny do katalogu release
echo "Kopiowanie pliku wykonywalnego..."
cp bmp_to_xbpp release/${VERSION}/

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
zip bmp_to_xbpp_macOS_x86.zip bmp_to_xbpp sample-image.bmp FILE_ID.DIZ

if [ $? -ne 0 ]; then
    echo "BŁĄD: Błąd podczas tworzenia archiwum ZIP!"
    exit 1
fi

# Usuń pliki po utworzeniu ZIP
echo "Usuwanie plików..."
rm bmp_to_xbpp sample-image.bmp

# Wróć do katalogu głównego
cd ../..

# Usuń również plik wykonywalny z katalogu głównego
echo "Usuwanie pliku wykonywalnego z katalogu głównego..."
rm -f bmp_to_xbpp

# Wyczyść pliki obiektowe
echo "Usuwanie plików obiektowych..."
rm -f *.o

echo "Archiwum utworzone pomyślnie!"
echo ""

echo "Zawartość katalogu release/${VERSION}/:"
ls -la release/${VERSION}/

echo ""
echo "Budowanie wersji macOS zakończone pomyślnie!"
echo "Archiwum: release/${VERSION}/bmp_to_xbpp_macOS_x86.zip"
