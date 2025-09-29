# BMP to xbpp Array Converter

Program do konwersji plików BMP na różne formaty tablic zawierających bajty obrazka w formacie 4bpp (4 bits per pixel) lub 1bpp (1 bit per pixel).

## Cel projektu

Program umożliwia konwersję obrazów BMP na dane w dwóch formatach:
- **4bpp (4 bits per pixel)** - dla wyświetlaczy LCD z paletą 16 odcieni (każdy bajt zawiera 2 piksele)
- **1bpp (1 bit per pixel)** - dla wyświetlaczy monochromatycznych (każdy bajt zawiera 8 pikseli)

Program obsługuje różne algorytmy ditheringu dla trybu 1bpp, aby uzyskać najlepszą jakość wizualną przy konwersji do czarno-białego.

### Przykłady konwersji

**Obraz wejściowy (BMP):**
![Obraz wejściowy](img/sample-image.bmp)

**Rezultat 4bpp na wyświetlaczu LCD (16 odcieni żółtego):**
![Rezultat na LCD](img/sample-image.png)

**Rezultaty 1bpp z różnymi algorytmami ditheringu:**

**Brak ditheringu (progowanie) - domyślne:**
![Brak ditheringu](img/sample-image-dither-none-1bpp.bmp)
*Proste progowanie - piksele powyżej 50% jasności stają się białe, poniżej - czarne.*

**Floyd-Steinberg dithering:**
![Floyd-Steinberg](img/sample-image-dither-floyd-1bpp.bmp)
*Algorytm rozprasza błędy kwantyzacji na sąsiednie piksele, tworząc naturalny efekt ditheringu.*

**Ordered 8x8 dithering:**
![Ordered 8x8](img/sample-image-dither-o8x8-1bpp.bmp)
*Używa matrycy Bayer 8x8 do tworzenia regularnego wzoru ditheringu, optymalnego dla wyświetlaczy.*

### Jak osiągnąć te rezultaty

1. **Konwersja 4bpp (domyślna)** - zobacz sekcję [Użycie](#użycie):
   ```bash
   ./bmp_to_xbpp img/sample-image.bmp
   ```

2. **Konwersja 1bpp bez ditheringu (domyślne)** - zobacz sekcję [Tryb 1bpp](#tryb-1bpp):
   ```bash
   ./bmp_to_xbpp -1 img/sample-image.bmp
   ```

3. **Konwersja 1bpp z Floyd-Steinberg dithering**:
   ```bash
   ./bmp_to_xbpp -1 -d floyd img/sample-image.bmp
   ```

4. **Konwersja 1bpp z Ordered dithering**:
   ```bash
   ./bmp_to_xbpp -1 -d o8x8 img/sample-image.bmp
   ```

## Opis

Program konwertuje obrazy BMP na różne formaty wyjściowe:

### Tryb 4bpp (domyślny)
- Każdy bajt zawiera 2 piksele w formacie 4bpp
- Obraz jest automatycznie konwertowany do skali szarości i skalowany do 16 odcieni (0-15)
- Idealny dla wyświetlaczy LCD z paletą kolorów

### Tryb 1bpp
- Każdy bajt zawiera 8 pikseli w formacie 1bpp (czarno-biały)
- Obraz jest konwertowany do skali szarości, a następnie dithering jest stosowany
- Idealny dla wyświetlaczy monochromatycznych (OLED, e-paper, LCD monochromatyczne)

## Kompilacja

### Linux/macOS (Makefile)
```bash
make
```

### Windows (Visual Studio)
Otwórz `bmp_to_4bpp.sln` w Visual Studio i zbuduj projekt (Ctrl+Shift+B).

Dostępne konfiguracje:
- Debug/Release dla x86 i x64
- Platform Toolset: v142 (Visual Studio 2019)

## Użycie

```bash
./bmp_to_xbpp [OPTIONS] input.bmp [output_file]
```

### Argumenty:
- `input.bmp` - plik wejściowy BMP (24-bit lub 32-bit)
- `output_file` - opcjonalny plik wyjściowy (domyślnie: image_data.h)

### Opcje główne:
- `-4, --4bpp` - Użyj 4 bits per pixel (domyślnie)
- `-1, --1bpp` - Użyj 1 bit per pixel (czarno-biały)

### Opcje skanowania:
- `-h, --horizontal` - Skanuj poziomo (wierszami) (domyślnie)
- `-v, --vertical` - Skanuj pionowo (kolumnami)
- `-l, --little-endian` - Kolejność pikseli little endian (domyślnie)
- `-b, --big-endian` - Kolejność pikseli big endian

### Opcje formatów wyjściowych:
- `-c, --c-array` - Format tablicy C (.h) (domyślnie)
- `-r, --raw-data` - Format surowych danych (.hex)
- `-a, --assembler` - Format assemblera (.inc)
- `-aa, --assembler-array` - Format MASM z makrem .array (.inc)
- `-p, --progmem` - Dodaj słowo kluczowe PROGMEM do tablic C
- `-n, --name NAME` - Ustaw nazwę tablicy (domyślnie: image_data)

### Opcje ditheringu (tylko dla 1bpp):
- `-d, --dither METHOD` - Metoda ditheringu
  - `none` - Brak ditheringu (proste progowanie) (domyślnie dla 1bpp)
  - `floyd` - Floyd-Steinberg dithering
  - `o8x8` - Ordered 8x8 dithering

### Inne opcje:
- `--help` - Pokaż pomoc

## Tryb 1bpp

Tryb 1bpp konwertuje obrazy do formatu monochromatycznego (czarno-biały), gdzie każdy bajt zawiera 8 pikseli. Program oferuje trzy algorytmy ditheringu:

### Algorytmy ditheringu

#### 1. Brak ditheringu (progowanie) - domyślny
- **Opis**: Proste progowanie - piksele >50% jasności = białe, ≤50% = czarne
- **Zalety**: Najszybsze, zachowuje ostre krawędzie
- **Wady**: Utrata szczegółów w półtonach
- **Użycie**: `./bmp_to_xbpp -1 image.bmp` (domyślne)

#### 2. Floyd-Steinberg dithering
- **Opis**: Rozprasza błędy kwantyzacji na sąsiednie piksele
- **Zalety**: Naturalny wygląd, dobra jakość dla fotografii
- **Wady**: Może tworzyć artefakty przy ostrych krawędziach
- **Użycie**: `./bmp_to_xbpp -1 -d floyd image.bmp`

#### 3. Ordered 8x8 dithering
- **Opis**: Używa matrycy Bayer 8x8 do tworzenia regularnego wzoru
- **Zalety**: Przewidywalny wzór, optymalny dla wyświetlaczy
- **Wady**: Może być widoczny regularny wzór
- **Użycie**: `./bmp_to_xbpp -1 -d o8x8 image.bmp`

### Endianness w trybie 1bpp

W trybie 1bpp kolejność bitów w bajcie ma znaczenie:
- **Little endian** (`-l`): bit 0 = pierwszy piksel (lewy)
- **Big endian** (`-b`): bit 7 = pierwszy piksel (lewy)

## Formaty wyjściowe

### 1. Tablica C (.h) - domyślny
Pełna tablica C z deklaracją:

**Format 4bpp:**
```c
// Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT 20250929T2205
// Image size: 256x64
// Format: 4bpp
const unsigned char image_data[8192] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, ...
};
```

**Format 1bpp bez ditheringu (domyślne):**
```c
// Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT 20250929T2206
// Image size: 256x64
// Format: 1bpp (dithering: None)
const unsigned char image_data[2048] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ...
};
```

Z PROGMEM:
```c
const unsigned char image_data[8192] PROGMEM = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, ...
};
```

### 2. Surowe dane (.hex)
Tylko dane bez deklaracji:
```
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FE DE
DD DD DD DD CF FF FF FF FF FF FF FF FF FF FF FF
...
```

### 3. Format assemblera (.inc)
Format dla assemblera:

**Format 4bpp:**
```
; Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT 20250929T2205
; Image size: 256x64
; Format: 4bpp
image_data:
    .db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    .db 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xDE
    ...
```

**Format 1bpp bez ditheringu (domyślne):**
```
; Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT 20250929T2207
; Image size: 256x64
; Format: 1bpp (dithering: None)
image_data:
    .db $00, $00, $00, $00, $00, $00, $00, $00
    .db $00, $00, $00, $00, $00, $00, $00, $00
    ...
```

### 4. Format MASM z makrem .array (.inc)
Format dla MASM assemblera z makrem .array:

**Format 4bpp:**
```
; Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT 20250929T2205
; Image size: 256x64
; Format: 4bpp
.array image_data[8192].byte
 $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FE, $DE
$FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FE, $DE
...
.enda
```

**Format 1bpp bez ditheringu:**
```
; Generated by BMP to xbpp Array Converter v1.0 2025-09-28 (c) PTODT 20250929T2208
; Image size: 256x64
; Format: 1bpp (dithering: None)
.array image_data[2048].byte
 $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
$00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
...
.enda
```

## Przykłady

### Tryb 4bpp (domyślny)

```bash
# Domyślne ustawienia (tablica C, poziomo, little endian)
./bmp_to_xbpp test.bmp image_data.h

# Format surowych danych
./bmp_to_xbpp -r test.bmp data.hex

# Format assemblera
./bmp_to_xbpp -a test.bmp sprite.inc

# Format MASM z makrem .array
./bmp_to_xbpp -aa -n image_data test.bmp sprite.inc

# Tablica C z PROGMEM
./bmp_to_xbpp -p test.bmp progmem_data.h

# Skanowanie pionowe z big endian
./bmp_to_xbpp -v -b test.bmp vertical_big.h

# Niestandardowa nazwa tablicy
./bmp_to_xbpp -n my_sprite test.bmp sprite.h
```

### Tryb 1bpp

```bash
# 1bpp bez ditheringu (domyślny)
./bmp_to_xbpp -1 test.bmp monochrome.h

# 1bpp z Floyd-Steinberg dithering
./bmp_to_xbpp -1 -d floyd test.bmp floyd.h

# 1bpp z Ordered dithering
./bmp_to_xbpp -1 -d o8x8 test.bmp ordered.h

# 1bpp z big endian
./bmp_to_xbpp -1 -b test.bmp big_endian.h

# 1bpp w formacie assemblera
./bmp_to_xbpp -1 -a test.bmp monochrome.inc

# 1bpp z niestandardową nazwą
./bmp_to_xbpp -1 -n my_icon test.bmp icon.h
```

### Kombinacje opcji

```bash
# Kombinacja opcji 4bpp
./bmp_to_xbpp -v -b -a -n sprite_data test.bmp sprite.inc

# Kombinacja opcji 1bpp
./bmp_to_xbpp -1 -d o8x8 -v -b test.bmp vertical_ordered.h

# Format MASM z niestandardową nazwą
./bmp_to_xbpp -aa -n MY_SPRITE test.bmp sprite.inc

# Pokaż pomoc
./bmp_to_xbpp --help
```

## Struktura plików

### Pliki źródłowe
- `bmp_to_xbpp.c` - główny plik źródłowy (logika główna)
- `bmp_reader.c` / `bmp_reader.h` - obsługa plików BMP
- `utils.c` / `utils.h` - funkcje pomocnicze (konwersja, pakowanie, zapis)
- `options.c` / `options.h` - obsługa argumentów wiersza poleceń
- `defs.h` - definicje typów i stałych

### Pliki kompilacji
- `Makefile` - dla Linux/macOS
- `bmp_to_xbpp.sln` - plik rozwiązania Visual Studio
- `bmp_to_xbpp.vcxproj` - plik projektu Visual Studio
- `bmp_to_xbpp.vcxproj.filters` - filtry plików w Solution Explorer
- `bmp_to_xbpp.vcxproj.user` - ustawienia użytkownika

### Dokumentacja
- `readme.md` - ten plik

## Rozmiary danych

### Tryb 4bpp
- **Rozmiar**: `(szerokość × wysokość) ÷ 2` bajtów
- **Przykład**: Obraz 256×64 pikseli = 8192 bajty
- **Zawartość**: 2 piksele na bajt (4 bity każdy)

### Tryb 1bpp
- **Rozmiar**: `(szerokość × wysokość) ÷ 8` bajtów
- **Przykład**: Obraz 256×64 pikseli = 2048 bajtów
- **Zawartość**: 8 pikseli na bajt (1 bit każdy)

## Wymagania

- **Linux/macOS**: Kompilator C99 (gcc)
- **Windows**: Visual Studio 2019 lub nowszy
- Obsługa plików BMP 24-bit i 32-bit

## Licencja

MIT - zobacz nagłówki plików źródłowych dla szczegółów.

## Lista zmian

### v1.0.1 (29/09/2025) - Rozszerzenie o obsługę 1bpp i dithering
- **Nowe funkcje:**
  - dodano obsługę trybu 1bpp (1 bit per pixel) dla wyświetlaczy monochromatycznych
  - zaimplementowano trzy algorytmy ditheringu:
    - Floyd-Steinberg dithering (domyślny dla 1bpp)
    - ordered 8x8 dithering (matryca Bayer)
    - brak ditheringu (proste progowanie)
  - dodano nowe opcje CLI: `-1/--1bpp`, `-d/--dither METHOD`
  - rozszerzono komentarze w plikach wynikowych o informacje o trybie i metodzie ditheringu

- **Ulepszenia:**
  - rozszerzono dokumentację o szczegółowe opisy algorytmów ditheringu
  - dodano przykłady obrazów pokazujące różnice między metodami ditheringu

- **Zachowana kompatybilność:**
  - tryb 4bpp pozostaje domyślnym i działa identycznie jak wcześniej
  - wszystkie istniejące opcje CLI działają bez zmian
  - opcje ditheringu są ignorowane dla trybu 4bpp

### v1.0.0 (28/09/2025) - Pierwsza wersja projektu
- **Funkcje podstawowe:**
  - konwersja obrazów BMP (24-bit, 32-bit) do formatu 4bpp
  - obsługa różnych formatów wyjściowych: C array, raw data, assembler, MASM
  - opcje skanowania: poziome/pionowe, little/big endian
  - wsparcie dla PROGMEM w tablicach C
  - niestandardowe nazwy tablic wyjściowych

- **Formaty wyjściowe:**
  - tablica C (.h) z opcjonalnym PROGMEM
  - surowe dane (.hex)
  - format assemblera (.inc)
  - format MASM z makrem .array (.inc)

- **Platformy:**
  - Linux/macOS (Makefile + gcc)
  - Windows (Visual Studio 2019+)

## Autor

Michal Kolodziejski (2:480/112.10)  
Copyright (c) PTODT <https://ptodt.org.pl>