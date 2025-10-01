# BMP to xbpp Array Converter

Program do konwersji plików BMP na różne formaty tablic zawierających bajty obrazka w formacie 4bpp (4 bits per pixel) lub 1bpp (1 bit per pixel).

## Cel projektu

Program umożliwia konwersję obrazów BMP na dane w dwóch formatach:
- **4bpp (4 bits per pixel)** - dla wyświetlaczy LCD z paletą 16 odcieni (każdy bajt zawiera 2 piksele)
- **1bpp (1 bit per pixel)** - dla wyświetlaczy monochromatycznych (każdy bajt zawiera 8 pikseli)

Program obsługuje różne algorytmy ditheringu dla trybu 1bpp, aby uzyskać najlepszą jakość wizualną przy konwersji do czarno-białego. Dodatkowo oferuje zaawansowane opcje regulacji obrazu: jasność i kontrast, które pozwalają na precyzyjne dostrojenie wyglądu konwersji.

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

**Przykład z regulacją jasności i kontrastu (br60, ct70):**
![1bpp br60 ct70](img/sample-image-dither-floyd-br60-ct70.bmp)
*Jasność 60%, kontrast 70% - optymalne ustawienia dla większości obrazów.*

### Regulacja jasności i kontrastu

Program oferuje zaawansowane opcje regulacji obrazu dla trybu 1bpp:

#### Parametry regulacji:
- **Jasność (`-br`, `--brightness`)**: 0-100% (domyślnie 50%)
  - 0% = maksymalnie ciemny obraz
  - 50% = oryginalna jasność
  - 100% = maksymalnie jasny obraz

- **Kontrast (`-ct`, `--contrast`)**: 0-100% (domyślnie 50%)
  - 0% = brak kontrastu (szary)
  - 50% = oryginalny kontrast
  - 100% = maksymalny kontrast

#### Przykłady regulacji jasności (kontrast stały 80%):

**Jasność 10%:**
![Jasność 10%](img/sample-image-dither-floyd-br10-ct80.bmp)
*Bardzo ciemny obraz - większość szczegółów zanika*

**Jasność 50%:**
![Jasność 50%](img/sample-image-dither-floyd-br50-ct80.bmp)
*Średnia jasność - zachowuje większość szczegółów*

**Jasność 100%:**
![Jasność 100%](img/sample-image-dither-floyd-br100-ct80.bmp)
*Maksymalnie jasny obraz - może powodować przepalenie jasnych obszarów*

#### Przykłady regulacji kontrastu (jasność stała 50%):

**Kontrast 10%:**
![Kontrast 10%](img/sample-image-dither-floyd-br50-ct10.bmp)
*Bardzo niski kontrast - obraz wygląda "płasko"*

**Kontrast 50%:**
![Kontrast 50%](img/sample-image-dither-floyd-br50-ct50.bmp)
*Średni kontrast - naturalny wygląd*

**Kontrast 80%:**
![Kontrast 80%](img/sample-image-dither-floyd-br50-ct80.bmp)
*Wysoki kontrast - wyostrzone krawędzie i szczegóły*

#### Przykłady kombinacji jasności i kontrastu:

**Jasność 70%, Kontrast 100%:**
![Jasność 70%, Kontrast 100%](img/sample-image-dither-floyd-br70-ct100.bmp)
*Jasny obraz z maksymalnym kontrastem - idealny do wyświetlaczy o niskiej jakości*

**Jasność 60%, Kontrast 70%:**
![Jasność 60%, Kontrast 70%](img/sample-image-dither-floyd-br60-ct70.bmp)
*Zbalansowane ustawienia - dobre dla większości zastosowań*

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

5. **Konwersja 1bpp z regulacją jasności i kontrastu**:
   ```bash
   # Jasność 70%, Kontrast 100%
   ./bmp_to_xbpp -1 -d floyd -br 70 -ct 100 --bmp img/sample-image.bmp output/br70_ct100
   
   # Jasność 60%, Kontrast 70%
   ./bmp_to_xbpp -1 -d floyd -br 60 -ct 70 --bmp img/sample-image.bmp output/br60_ct70
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

### Opcje regulacji obrazu (tylko dla 1bpp):
- `-br, --brightness PERC` - Jasność 0-100% (domyślnie 50%)
- `-ct, --contrast PERC` - Kontrast 0-100% (domyślnie 50%)
- `--bmp` - Generuj BMP preview (z niestandardową sekcją copyright)

### Opcje inwersji:
- `-i, --invert` - Odwróć bity (zamień 0 na 1 i odwrotnie)

### Opcje palet:
- `--palette VARIANT` - Wariant palety dla 1bpp (bw, gray, green, portfolio, oled_yellow, custom)
- `--palette4bpp VAR` - Wariant palety dla 4bpp (bw, gray, green, portfolio, oled_yellow, custom)
- `-cf, --color_first_in_ramp (r,g,b)` - Pierwszy kolor w rampie niestandardowej (wartości 8-bitowe)
- `-cl, --color_last_in_ramp (r,g,b)` - Ostatni kolor w rampie niestandardowej (wartości 8-bitowe)

### Inne opcje:
- `--help` - Pokaż pomoc

## Palety kolorów

Program oferuje różne palety kolorów zarówno dla trybu 1bpp jak i 4bpp, dostosowane do różnych typów wyświetlaczy:

### Palety niestandardowe

Program obsługuje palety niestandardowe dla obu trybów (1bpp i 4bpp) poprzez opcje `-cf` i `-cl`:

**Przykład użycia:**
```bash
# Paleta niestandardowa 1bpp (czerwony → niebieski)
./bmp_to_xbpp -1 --palette custom -cf "(255,0,0)" -cl "(0,0,255)" --bmp input.bmp output.h

# Paleta niestandardowa 4bpp (zielony → żółty)
./bmp_to_xbpp -4 --palette4bpp custom -cf "(0,255,0)" -cl "(255,255,0)" --bmp input.bmp output.h
```

**Format kolorów:**
- Kolory podawane w formacie `(r,g,b)` gdzie r, g, b to wartości 0-255
- Dla 1bpp: generowane są tylko 2 kolory (pierwszy i ostatni)
- Dla 4bpp: generowane są 16 kolorów z interpolacją liniową między pierwszym a ostatnim

### Palety 1bpp (2 kolory):
- **BW (czarno-biała)** - domyślna: (0,0,0) i (255,255,255)
- **GRAY (szara)**: (30,30,30) i (128,128,128) - subtelne odcienie szarości
- **GREEN (zielona)**: (170,170,120) i (80,120,40) - klasyczny zielony LCD
- **PORTFOLIO (Atari Portfolio)**: (144,238,144) i (72,72,160) - charakterystyczne kolory
- **OLED_YELLOW (OLED żółty)**: (20,20,20) i (255,220,0) - dla wyświetlaczy OLED

### Palety 4bpp (16 kolorów z interpolacją liniową):
- **BW (czarno-biała)** - domyślna: interpolacja od (0,0,0) do (255,255,255)
- **GRAY (szara)**: interpolacja od (30,30,30) do (128,128,128) - ograniczony zakres
- **GREEN (zielona)**: interpolacja od (170,170,120) do (80,120,40) - odcienie zieleni
- **PORTFOLIO (Atari Portfolio)**: interpolacja od (144,238,144) do (72,72,160)
- **OLED_YELLOW (OLED żółty)**: interpolacja od (20,20,20) do (255,220,0)

### Zastosowania palet:

#### Palety 1bpp:
- **BW**: Uniwersalna, dobre dla większości wyświetlaczy monochromatycznych
- **GRAY**: Subtelne odcienie, idealna dla wyświetlaczy z ograniczonym kontrastem
- **GREEN**: Klasyczny zielony LCD, retro wygląd
- **PORTFOLIO**: Specjalnie dla Atari Portfolio i podobnych urządzeń
- **OLED_YELLOW**: Dla wyświetlaczy OLED z żółtymi pikselami

#### Palety 4bpp:
- **BW**: Pełny zakres szarości, uniwersalna
- **GRAY**: Ograniczony zakres, subtelne przejścia
- **GREEN**: Odcienie zieleni, retro estetyka
- **PORTFOLIO**: Charakterystyczne kolory Atari Portfolio
- **OLED_YELLOW**: Optymalna dla wyświetlaczy OLED

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

### Regulacja jasności i kontrastu

Program oferuje zaawansowaną kontrolę nad konwersją 1bpp poprzez parametry jasności i kontrastu:

#### Jasność (0-100%)
- **0%**: Bardzo ciemny obraz (prawie wszystkie piksele czarne)
- **25%**: Ciemny obraz
- **50%**: Neutralna jasność (domyślna)
- **75%**: Jasny obraz
- **100%**: Bardzo jasny obraz (prawie wszystkie piksele białe)

#### Kontrast (0-100%)
- **0%**: Brak kontrastu (wszystkie piksele w środku skali)
- **25%**: Niski kontrast
- **50%**: Normalny kontrast (domyślny)
- **75%**: Wysoki kontrast
- **100%**: Maksymalny kontrast (tylko czarne i białe piksele)

#### Przykłady użycia:
```bash
# Ciemny obraz z niskim kontrastem
./bmp_to_xbpp -1 -br 25 -ct 25 image.bmp

# Jasny obraz z wysokim kontrastem
./bmp_to_xbpp -1 -br 75 -ct 75 image.bmp

# Ekstremalne wartości
./bmp_to_xbpp -1 -br 0 -ct 100 image.bmp  # Bardzo ciemny, maksymalny kontrast
./bmp_to_xbpp -1 -br 100 -ct 0 image.bmp  # Bardzo jasny, brak kontrastu

# Palety niestandardowe
./bmp_to_xbpp -1 --palette custom -cf "(255,0,0)" -cl "(0,0,255)" image.bmp  # Czerwony → niebieski
./bmp_to_xbpp -4 --palette4bpp custom -cf "(0,255,0)" -cl "(255,255,0)" image.bmp  # Zielony → żółty

# Generowanie BMP preview do wizualizacji
./bmp_to_xbpp -1 -br 25 -ct 75 -d floyd --bmp image.bmp preview
```

#### Przykłady graficzne z różnymi ustawieniami:

**Jasność (kontrast 50%, Floyd-Steinberg):**
- `sample-image-br0-ct50-floyd.bmp` - 0% jasności (bardzo ciemny)
- `sample-image-br15-ct50-floyd.bmp` - 15% jasności (ciemny)
- `sample-image-br25-ct50-floyd.bmp` - 25% jasności (ciemny)
- `sample-image-br50-ct50-floyd.bmp` - 50% jasności (neutralny)
- `sample-image-br75-ct50-floyd.bmp` - 75% jasności (jasny)

**Kontrast (jasność 50%, Floyd-Steinberg):**
- `sample-image-br50-ct0-floyd.bmp` - 0% kontrastu (brak kontrastu)
- `sample-image-br50-ct50-floyd.bmp` - 50% kontrastu (normalny)
- `sample-image-br50-ct100-floyd.bmp` - 100% kontrastu (maksymalny)

### Endianness w trybie 1bpp

W trybie 1bpp kolejność bitów w bajcie ma znaczenie:
- **Little endian** (`-l`): bit 0 = pierwszy piksel (lewy)
- **Big endian** (`-b`): bit 7 = pierwszy piksel (lewy)

### Inwersja bitów

Program oferuje opcję inwersji bitów (`-i`, `--invert`), która zamienia wszystkie bity w danych wyjściowych:
- **0** staje się **1**
- **1** staje się **0**

#### Kiedy używać inwersji?

Inwersja bitów jest przydatna dla wyświetlaczy, które interpretują bity odwrotnie niż standardowo:

**Wyświetlacze wymagające inwersji:**
- **Atari Portfolio**: bit 1 = zgaszony piksel (czarny), bit 0 = zapalony piksel (biały)
- **Standardowy zielony LCD**: wymaga inwersji dla poprawnego wyświetlania
- **Niektóre wyświetlacze LCD** z niestandardową logiką

**Wyświetlacze bez inwersji:**
- **Wyświetlacze OLED**: standardowa logika (bit 1 = zapalony, bit 0 = zgaszony)
- **Większość nowoczesnych wyświetlaczy**: standardowa logika

**Standardowo**: bit 1 = zapalony piksel (biały), bit 0 = zgaszony piksel (czarny)

#### Przykład inwersji:

**1bpp - Obraz oryginalny (bez inwersji):**
![1bpp Normal](img/sample-image-dither-floyd-br60-ct70-normal.bmp)
*Czarne obszary = 0, białe obszary = 1 (dither: Floyd-Steinberg, br: 60%, ct: 70%)*

**1bpp - Obraz z inwersją (`-i`):**
![1bpp Invert](img/sample-image-dither-floyd-br60-ct70-invert.bmp)
*Czarne obszary = 1, białe obszary = 0 (dither: Floyd-Steinberg, br: 60%, ct: 70%)*

**4bpp - Obraz oryginalny (bez inwersji):**
![4bpp BW Normal](img/sample-image-4bpp-bw-normal.bmp)
*Ciemne obszary = niskie wartości (0-7), jasne obszary = wysokie wartości (8-15) - paleta BW*

**4bpp - Obraz z inwersją (`-i`):**
![4bpp BW Invert](img/sample-image-4bpp-bw-invert.bmp)
*Ciemne obszary = wysokie wartości (8-15), jasne obszary = niskie wartości (0-7) - paleta BW*

#### Przykłady palet 1bpp:

**Paleta BW (czarno-biała) - domyślna:**
![BW Normal](img/sample-image-dither-floyd-br60-ct70-1bpp-bw-normal.bmp)
*Indeks 0: (0,0,0), Indeks 1: (255,255,255) - standardowa paleta*

![BW Invert](img/sample-image-dither-floyd-br60-ct70-1bpp-bw-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta GRAY (szara):**
![GRAY Normal](img/sample-image-dither-floyd-br60-ct70-1bpp-gray-normal.bmp)
*Indeks 0: (30,30,30), Indeks 1: (128,128,128) - szara paleta*

![GRAY Invert](img/sample-image-dither-floyd-br60-ct70-1bpp-gray-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta GREEN (zielona):**
![GREEN Normal](img/sample-image-dither-floyd-br60-ct70-1bpp-green-normal.bmp)
*Indeks 0: (170,170,120), Indeks 1: (80,120,40) - zielona paleta*

![GREEN Invert](img/sample-image-dither-floyd-br60-ct70-1bpp-green-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta PORTFOLIO (Atari Portfolio):**
![PORTFOLIO Normal](img/sample-image-dither-floyd-br60-ct70-1bpp-portfolio-normal.bmp)
*Indeks 0: (144,238,144), Indeks 1: (72,72,160) - paleta Atari Portfolio*

![PORTFOLIO Invert](img/sample-image-dither-floyd-br60-ct70-1bpp-portfolio-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta OLED_YELLOW (OLED żółty):**
![OLED_YELLOW Normal](img/sample-image-dither-floyd-br60-ct70-1bpp-oled_yellow-normal.bmp)
*Indeks 0: (20,20,20), Indeks 1: (255,220,0) - paleta OLED żółty*

![OLED_YELLOW Invert](img/sample-image-dither-floyd-br60-ct70-1bpp-oled_yellow-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

#### Przykłady palet 4bpp:

**Paleta BW (czarno-biała) - domyślna:**
![4bpp BW Normal](img/sample-image-4bpp-bw-normal.bmp)
*Interpolacja liniowa od (0,0,0) do (255,255,255) - 16 odcieni szarości*

![4bpp BW Invert](img/sample-image-4bpp-bw-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta GRAY (szara):**
![4bpp GRAY Normal](img/sample-image-4bpp-gray-normal.bmp)
*Interpolacja liniowa od (30,30,30) do (128,128,128) - 16 odcieni szarości w ograniczonym zakresie*

![4bpp GRAY Invert](img/sample-image-4bpp-gray-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta GREEN (zielona):**
![4bpp GREEN Normal](img/sample-image-4bpp-green-normal.bmp)
*Interpolacja liniowa od (170,170,120) do (80,120,40) - 16 odcieni zieleni*

![4bpp GREEN Invert](img/sample-image-4bpp-green-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta PORTFOLIO (Atari Portfolio):**
![4bpp PORTFOLIO Normal](img/sample-image-4bpp-portfolio-normal.bmp)
*Interpolacja liniowa od (144,238,144) do (72,72,160) - 16 odcieni Portfolio*

![4bpp PORTFOLIO Invert](img/sample-image-4bpp-portfolio-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

**Paleta OLED_YELLOW (OLED żółty):**
![4bpp OLED_YELLOW Normal](img/sample-image-4bpp-oled_yellow-normal.bmp)
*Interpolacja liniowa od (20,20,20) do (255,220,0) - 16 odcieni żółci*

![4bpp OLED_YELLOW Invert](img/sample-image-4bpp-oled_yellow-invert.bmp)
*Z inwersją - dla wyświetlaczy wymagających odwróconej logiki*

#### Użycie:
```bash
# Generowanie z inwersją bitów
./bmp_to_xbpp -1 -d floyd -br 60 -ct 70 -i --bmp img/sample-image.bmp inverted_image

# Inwersja dla 4bpp
./bmp_to_xbpp -4 -i --bmp img/sample-image.bmp inverted_4bpp

# Różne palety 1bpp (2 kolory)
./bmp_to_xbpp -1 --palette bw --bmp img/sample-image.bmp bw_image
./bmp_to_xbpp -1 --palette gray --bmp img/sample-image.bmp gray_image
./bmp_to_xbpp -1 --palette green --bmp img/sample-image.bmp green_image
./bmp_to_xbpp -1 --palette portfolio --bmp img/sample-image.bmp portfolio_image
./bmp_to_xbpp -1 --palette oled_yellow --bmp img/sample-image.bmp oled_yellow_image

# Różne palety 4bpp (16 kolorów z interpolacją liniową)
./bmp_to_xbpp -4 --palette4bpp bw --bmp img/sample-image.bmp bw_4bpp
./bmp_to_xbpp -4 --palette4bpp gray --bmp img/sample-image.bmp gray_4bpp
./bmp_to_xbpp -4 --palette4bpp green --bmp img/sample-image.bmp green_4bpp
./bmp_to_xbpp -4 --palette4bpp portfolio --bmp img/sample-image.bmp portfolio_4bpp
./bmp_to_xbpp -4 --palette4bpp oled_yellow --bmp img/sample-image.bmp oled_yellow_4bpp

# Palety 4bpp z inwersją
./bmp_to_xbpp -4 --palette4bpp bw -i --bmp img/sample-image.bmp bw_4bpp_inverted
./bmp_to_xbpp -4 --palette4bpp gray -i --bmp img/sample-image.bmp gray_4bpp_inverted
./bmp_to_xbpp -4 --palette4bpp green -i --bmp img/sample-image.bmp green_4bpp_inverted
./bmp_to_xbpp -4 --palette4bpp portfolio -i --bmp img/sample-image.bmp portfolio_4bpp_inverted
./bmp_to_xbpp -4 --palette4bpp oled_yellow -i --bmp img/sample-image.bmp oled_yellow_4bpp_inverted

# Palety z inwersją (dla wyświetlaczy wymagających odwróconej logiki)
./bmp_to_xbpp -1 --palette bw -i --bmp img/sample-image.bmp bw_inverted
./bmp_to_xbpp -1 --palette portfolio -i --bmp img/sample-image.bmp portfolio_inverted
./bmp_to_xbpp -1 --palette oled_yellow -i --bmp img/sample-image.bmp oled_yellow_inverted
```

## Formaty wyjściowe

### 1. Tablica C (.h) - domyślny
Pełna tablica C z deklaracją:

**Format 4bpp:**
```c
// Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:29:31) (c) PTODT (https://ptodt.org.pl)
// Created on: 20251001T113453
// Image size: 256x64
// Format: 4bpp
const unsigned char image_data[8192] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, ...
};
```

**Format 1bpp bez ditheringu (domyślne):**
```c
// Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:29:31) (c) PTODT (https://ptodt.org.pl)
// Created on: 20251001T113453
// Image size: 256x64
// Format: 1bpp (dithering: None)
const unsigned char image_data[2048] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ...
};
```

**Format 1bpp z inwersją bitów:**
```c
// Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:29:31) (c) PTODT (https://ptodt.org.pl)
// Created on: 20251001T113453
// Image size: 256x64
// Format: 1bpp (dithering: Floyd-Steinberg, brightness: 60%, contrast: 70%, inverted)
const unsigned char image_data[2048] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, ...
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
; Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:29:31) (c) PTODT (https://ptodt.org.pl)
; Created on: 20251001T113453
; Image size: 256x64
; Format: 4bpp
image_data:
    .db $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF
    .db $FF, $FF, $FF, $FF, $FF, $FF, $FE, $DE
    ...
```

**Format 1bpp bez ditheringu (domyślne):**
```
; Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:29:31) (c) PTODT (https://ptodt.org.pl)
; Created on: 20251001T113453
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
; Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:29:31) (c) PTODT (https://ptodt.org.pl)
; Created on: 20251001T113453
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
; Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 11:29:31) (c) PTODT (https://ptodt.org.pl)
; Created on: 20251001T113453
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

# 1bpp z regulacją jasności i kontrastu
./bmp_to_xbpp -1 -br 25 -ct 75 test.bmp dark_high_contrast.h

# 1bpp z jasnym obrazem i niskim kontrastem
./bmp_to_xbpp -1 -br 75 -ct 25 test.bmp bright_low_contrast.h

# 1bpp z generowaniem BMP preview
./bmp_to_xbpp -1 -br 25 -ct 75 -d floyd --bmp test.bmp dark_high_contrast

# 4bpp z generowaniem BMP preview
./bmp_to_xbpp -4 --bmp test.bmp grayscale_preview

# 1bpp z inwersją bitów (dla Atari Portfolio)
./bmp_to_xbpp -1 -d floyd -br 60 -ct 70 -i --bmp test.bmp portfolio_image

# 4bpp z inwersją bitów
./bmp_to_xbpp -4 -i --bmp test.bmp inverted_4bpp

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

### v1.0.3 (30/09/2025) - Palety niestandardowe i refaktoryzacja
- **Nowe funkcje:**
  - dodano palety niestandardowe dla obu trybów (1bpp i 4bpp) z opcjami `-cf` i `-cl`
  - refaktoryzacja systemu palet - jedna funkcja `generate_palette` z `PaletteContext`
  - uproszczenie kodu poprzez unifikację obsługi palet 1bpp i 4bpp
- **Ulepszenia:**
  - dodano strukturę `PaletteContext` dla lepszej organizacji parametrów palet
  - zunifikowano stałe palet (usunięto duplikaty PALETTE_1BPP_* i PALETTE_4BPP_*)
  - dodano obsługę interpolacji liniowej dla palet niestandardowych 4bpp

### v1.0.2 (30/09/2025) - Rozszerzenie o regulację jasności, kontrastu i inwersję bitów
- **Nowe funkcje:**
  - dodano regulację jasności (`-br`, `--brightness`) 0-100% dla trybu 1bpp
  - dodano regulację kontrastu (`-ct`, `--contrast`) 0-100% dla trybu 1bpp
  - dodano opcję `--bmp` do generowania BMP preview dla obu trybów (1bpp i 4bpp)
  - dodano opcję inwersji bitów (`-i`, `--invert`) dla obu trybów (1bpp i 4bpp)
  - rozszerzono komentarze w plikach wynikowych o informacje o jasności, kontrastu i inwersji
  - dodano skrypty Shell i BAT do generowania serii obrazów z różnymi ustawieniami

- **Ulepszenia:**
  - naprawiono kolejność pikseli w trybie poziomym dla 1bpp
  - rozszerzono dokumentację o szczegółowe opisy regulacji obrazu i inwersji bitów
  - dodano przykłady obrazów pokazujące efekty różnych ustawień jasności i kontrastu
  - dodano sekcję ze skryptami do automatycznego generowania serii testowych
  - dodano szczegółowe wyjaśnienie zastosowań inwersji bitów (Atari Portfolio, OLED, LCD)

- **Zachowana kompatybilność:**
  - wszystkie istniejące opcje CLI działają bez zmian
  - opcje regulacji obrazu są ignorowane dla trybu 4bpp (z wyjątkiem `--bmp`)
  - opcja inwersji bitów działa dla obu trybów (1bpp i 4bpp)

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

## Skrypty do generowania serii obrazów

Projekt zawiera gotowe skrypty do automatycznego generowania serii obrazów BMP z różnymi kombinacjami jasności i kontrastu. Te skrypty są nieocenione do testowania i porównywania różnych parametrów konwersji dla Twojej konkretnej grafiki.

### Dostępne skrypty

- **`scripts/generate_brightness_contrast_series.sh`** - skrypt dla Linux/macOS
- **`scripts/generate_brightness_contrast_series.bat`** - skrypt dla Windows

### Co generują skrypty

Skrypty automatycznie generują **121 plików BMP** z wszystkimi kombinacjami jasności i kontrastu:
- **Jasność:** 0%, 10%, 20%, ..., 100% (11 wartości)
- **Kontrast:** 0%, 10%, 20%, ..., 100% (11 wartości)
- **Łącznie:** 11 × 11 = 121 kombinacji

Każdy plik jest nazywany według wzoru: `br{jasność}_ct{kontrast}.bmp`

**Przykłady nazw plików:**
- `br0_ct0.bmp` - jasność 0%, kontrast 0% (bardzo ciemny, bez kontrastu)
- `br50_ct50.bmp` - jasność 50%, kontrast 50% (domyślne ustawienia)
- `br60_ct70.bmp` - jasność 60%, kontrast 70% (zalecane ustawienia)
- `br100_ct100.bmp` - jasność 100%, kontrast 100% (maksymalne wartości)

### Jak wykorzystać do porównania parametrów

1. **Uruchom skrypt** na swojej grafice:
   ```bash
   ./scripts/generate_brightness_contrast_series.sh
   ```

2. **Przejrzyj wygenerowane obrazy** w katalogu `preview/`:
   - Otwórz katalog `preview/` w przeglądarce obrazów
   - Porównaj różne kombinacje jasności i kontrastu
   - Znajdź optymalne ustawienia dla Twojej grafiki

3. **Wybierz najlepsze ustawienia** na podstawie wizualnej oceny:
   - **Dla ciemnych grafik:** wyższa jasność (60-80%)
   - **Dla jasnych grafik:** niższa jasność (20-40%)
   - **Dla grafik o niskim kontraście:** wyższy kontrast (70-90%)
   - **Dla grafik o wysokim kontraście:** niższy kontrast (30-50%)

4. **Użyj wybranych parametrów** w rzeczywistej konwersji:
   ```bash
   ./bmp_to_xbpp -1 -d floyd -br 60 -ct 70 -i img/your-image.bmp output.h
   ```

### Przykłady zastosowań

- **Testowanie przed konwersją:** Sprawdź jak Twoja grafika wygląda z różnymi ustawieniami
- **Optymalizacja dla wyświetlacza:** Znajdź najlepsze ustawienia dla konkretnego wyświetlacza
- **Porównanie algorytmów:** Uruchom skrypt z różnymi metodami ditheringu
- **Dokumentacja:** Stwórz zestaw przykładów pokazujących wpływ parametrów

### Konfiguracja

W obu skryptach można łatwo zmienić:
- **Katalog wyjściowy:** zmienna `OUTPUT_DIR` (domyślnie `preview`)
- **Zakres wartości:** edytuj pętle w skrypcie (domyślnie 0-100% co 10%)

### 💡 Praktyczna wskazówka

**Najlepszy sposób na znalezienie optymalnych parametrów:**

1. **Uruchom skrypt** na swojej grafice
2. **Otwórz katalog `preview/`** w przeglądarce obrazów z możliwością porównywania (np. IrfanView, XnView)
3. **Użyj trybu porównania** aby zobaczyć różnice między ustawieniami
4. **Zwróć uwagę na:**
   - Czy szczegóły są dobrze widoczne
   - Czy kontrast nie jest zbyt wysoki (utrata szczegółów)
   - Czy jasność nie jest zbyt niska (ciemne obszary)
5. **Zapisz nazwę najlepszego pliku** (np. `br60_ct70.bmp`)
6. **Użyj tych parametrów** w rzeczywistej konwersji: `-br 60 -ct 70`


## Sekcja copyright w plikach BMP

Program generuje pliki BMP preview z niestandardową sekcją copyright, która jest umieszczana po standardowych nagłówkach BMP, ale przed danymi pikseli. Ta sekcja nie wpływa na kompatybilność z standardowymi przeglądarkami obrazów, które ją ignorują.

### Zawartość sekcji copyright:

```
COPY
Generated by BMP to xbpp Array Converter 1.0.3 (2025-10-01 22:33:36)
Copyright (c) PTODT 2025-10-01
https://ptodt.org.pl
Build time: 2025-10-01 22:33:29
-=# Didn't like it? Mind you own values #=-
```

### Szczegóły techniczne:

- **Rozmiar sekcji**: 256 bajtów (stały)
- **Pozycja**: Po `BITMAPINFOHEADER` i palecie, przed danymi pikseli
- **Format**: Nagłówek "COPY" (4 bajty) + długość sekcji (4 bajty) + tekst copyright + wypełnienie zerami
- **Kompatybilność**: Standardowe przeglądarki obrazów ignorują tę sekcję
- **Zawartość**: Informacje o wersji, prawach autorskich, dacie budowania i humorystyczna linia

#### Struktura sekcji copyright:

```
Offset | Rozmiar | Opis
-------|---------|-----
0x00   | 4 bajty | Nagłówek sekcji: "COPY"
0x04   | 4 bajty | Długość sekcji (little-endian, max 248 bajtów)
0x08   | N bajtów| Tekst copyright (UTF-8, null-terminated)
0x08+N | M bajtów| Wypełnienie zerami do 256 bajtów
```

#### Wpływ na plik BMP:

- **Rozmiar pliku**: Zwiększa się o 256 bajtów
- **Offset danych**: `bfOffBits` w `BITMAPFILEHEADER` jest zwiększany o 256 bajtów
- **Kompatybilność**: Plik pozostaje zgodny ze standardem BMP
- **Przeglądarki**: Standardowe przeglądarki (GIMP, Photoshop, Windows Photo Viewer) ignorują sekcję
- **Niestandardowe narzędzia**: Mogą odczytać sekcję używając offsetu z `bfOffBits`

#### Implementacja:

- **Funkcja**: `write_bmp_copyright_section()` w `bmp_writer.c`
- **Wywołanie**: Automatycznie przy generowaniu BMP preview (`--bmp`)
- **Zależności**: Wymaga `version.h` z informacjami o wersji i dacie budowania
- **Obsługa błędów**: Sprawdza rozmiar tekstu (max 248 bajtów) i wypełnia zerami

### Zastosowanie:

- **Identyfikacja**: Łatwe rozpoznanie plików wygenerowanych przez program
- **Wersjonowanie**: Informacja o wersji programu i dacie budowania
- **Prawa autorskie**: Jasne oznaczenie autorstwa i źródła
- **Debugowanie**: Pomocne przy identyfikacji problemów z konwersją

## System wersjonowania

Projekt używa scentralizowanego systemu wersjonowania:

### Pliki wersjonowania:
- **`VERSION`** - plik tekstowy zawierający numer wersji (np. `1.0.3`)
- **`version.h`** - automatycznie generowany plik nagłówkowy z stałymi wersji
- **`FILE_ID.DIZ`** - automatycznie generowany plik opisowy dla archiwów ZIP
- **`scripts/update_version.sh`** - skrypt do aktualizacji `version.h` i `FILE_ID.DIZ` na podstawie `VERSION` (Linux/macOS)
- **`scripts/update_version.bat`** - skrypt do aktualizacji `version.h` i `FILE_ID.DIZ` na podstawie `VERSION` (Windows)

#### O pliku FILE_ID.DIZ:
`FILE_ID.DIZ` to plik opisowy w formacie z czasów BBS-ów (Bulletin Board Systems) - systemów wymiany plików popularnych w latach 80-90. Format ten charakteryzuje się:
- **Maksymalnie 45 znaków w linii** - dopasowane do szerokości terminali tekstowych
- **Grafika ASCII** - proste rysunki złożone ze znaków tekstowych
- **Zwięzłe opisy** - krótkie, ale informacyjne opisy funkcjonalności
- **Nostalgiczny charakter** - przypomnienie ery przed graficznymi interfejsami

Plik jest automatycznie generowany podczas budowania i zawiera aktualne informacje o wersji, dacie budowania oraz opis funkcjonalności programu.
- **`scripts/build_linux.sh`** - skrypt do budowania wersji Linux za pomocą Docker
- **`scripts/build_macos.sh`** - skrypt do budowania wersji macOS
- **`scripts/build_windows.sh`** - skrypt do budowania wersji Windows za pomocą MinGW-w64
- **`scripts/Dockerfile.linux`** - definicja kontenera Ubuntu do budowania wersji Linux
- **`scripts/Dockerfile.windows`** - definicja kontenera Ubuntu z MinGW-w64 do budowania wersji Windows

### Jak aktualizować wersję:

#### Linux/macOS:
1. **Edytuj plik `VERSION`** - zmień numer wersji na nowy
2. **Uruchom `make`** - automatycznie wygeneruje `version.h` i skompiluje projekt

#### Windows (Visual Studio):
1. **Edytuj plik `VERSION`** - zmień numer wersji na nowy
2. **Skompiluj projekt** - Visual Studio automatycznie wygeneruje `version.h` przed kompilacją

### Budowanie wersji macOS:

#### Wymagania:
- macOS z zainstalowanymi Xcode Command Line Tools
- Make i GCC dostępne w systemie

#### Uruchomienie:
```bash
# Przejdź do katalogu scripts
cd scripts

# Uruchom skrypt budowania
./build_macos.sh
```

#### Rezultat:
- Utworzy katalog `release/{VERSION}/` z archiwum `bmp_to_xbpp_macOS_x86.zip`
- Wersja jest automatycznie odczytywana z pliku `VERSION`
- Automatycznie aktualizuje `version.h` i `FILE_ID.DIZ`, kompiluje projekt
- Archiwum zawiera: `bmp_to_xbpp`, `sample-image.bmp`, `FILE_ID.DIZ`

### Budowanie wersji Linux:

#### Wymagania:
- Docker Desktop zainstalowany i uruchomiony

#### Uruchomienie:
```bash
# Przejdź do katalogu scripts
cd scripts

# Uruchom skrypt budowania
./build_linux.sh
```

#### Rezultat:
- Utworzy katalog `release/{VERSION}/` z archiwum `bmp_to_xbpp_linux_x86.zip`
- Wersja jest automatycznie odczytywana z pliku `VERSION`
- Archiwum zawiera: `bmp_to_xbpp`, `sample-image.bmp`, `FILE_ID.DIZ`

### Budowanie wersji Windows:

#### Wymagania:
- Docker Desktop zainstalowany i uruchomiony

#### Uruchomienie:
```bash
# Przejdź do katalogu scripts
cd scripts

# Uruchom skrypt budowania
./build_windows.sh
```

#### Rezultat:
- Utworzy katalog `release/{VERSION}/` z archiwum `bmp_to_xbpp_windows_x86.zip`
- Wersja jest automatycznie odczytywana z pliku `VERSION`
- Archiwum zawiera: `bmp_to_xbpp.exe`, `sample-image.bmp`, `FILE_ID.DIZ`

### Przykład aktualizacji:

#### Linux/macOS:
```bash
# Zmień wersję w pliku VERSION
echo "1.0.4" > VERSION

# Skompiluj projekt (automatycznie zaktualizuje version.h)
make clean && make

# Wszystkie wygenerowane pliki będą zawierać nową wersję
```

#### Windows:
```cmd
REM Zmień wersję w pliku VERSION
echo 1.0.4 > VERSION

REM Skompiluj projekt w Visual Studio (automatycznie zaktualizuje version.h)
REM Lub uruchom skrypt ręcznie:
scripts\update_version.bat
```

### Stałe wersji w kodzie:
- `VERSION_STRING` - numer wersji (np. "1.0.3")
- `VERSION_DATE` - data aktualizacji (np. "2025-10-01")
- `BUILD_TIME` - dokładny czas budowania (np. "20251001T1038")
- `COPYRIGHT_STRING` - informacja o prawach autorskich ("PTODT (https://ptodt.org.pl)")

## Autor

Michal Kolodziejski (2:480/112.10)  
Copyright (c) PTODT <https://ptodt.org.pl>