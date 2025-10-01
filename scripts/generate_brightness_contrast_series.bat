@echo off
setlocal enabledelayedexpansion
REM generate_brightness_contrast_series.bat
REM Generuje serię obrazów BMP z różnymi kombinacjami jasności i kontrastu
REM
REM Copyright (c) 2025 PTODT <https://ptodt.org.pl>
REM Autor: Michal Kolodziejski (2:480/112.10)
REM Data: 2025.10.01
REM Licencja: MIT

REM Przejdź do katalogu głównego projektu (jeden poziom wyżej)
cd /d "%~dp0.."

REM Wczytaj wersję z pliku VERSION
if exist "VERSION" (
    for /f "delims=" %%i in (VERSION) do set VERSION=%%i
) else (
    set VERSION=1.0.3
)

REM Konfiguracja
set OUTPUT_DIR=preview

@ECHO ------------------------------------
@ECHO -- BMP TO XBPP CONVERTER v%VERSION%   --
@ECHO -- BRIGHTNESS/CONTRAST SERIES GEN --
@ECHO ------------------------------------
@ECHO.
@ECHO   CODE BY                                  
@ECHO   ----.-.---.---.---.                      
@ECHO       |-.---.   |--.|#==---.. .  .  .      
@ECHO    |  | |   | --.   |#==---. . .           
@ECHO   -'--'-'---'---'---'               2025.09
@ECHO.

REM Utwórz katalog wyjściowy
if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

@ECHO - GENERATING BMP FILES WITH BRIGHTNESS/CONTRAST COMBINATIONS
@ECHO.

REM Generuj wszystkie kombinacje jasności i kontrastu (0-100% co 10%)
for /L %%b in (0,10,100) do (
  for /L %%c in (0,10,100) do (
    echo Generating br%%b_ct%%c...
    bmp_to_xbpp.exe -1 -d floyd -br %%b -ct %%c --bmp img\sample-image.bmp %OUTPUT_DIR%\br%%b_ct%%c > nul 2>&1
  )
)

@ECHO.
@ECHO - DONE... FINISHED RESULTS ARE IN [%OUTPUT_DIR%\] FOLDER
for /f %%i in ('dir /b %OUTPUT_DIR%\*.bmp 2^>nul ^| find /c /v ""') do set count=%%i
@ECHO Generated !count! images in %OUTPUT_DIR%\ directory
@ECHO.
pause
