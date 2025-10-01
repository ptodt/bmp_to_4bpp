@echo off
REM Skrypt do aktualizacji wersji na podstawie pliku VERSION (Windows)
REM
REM Copyright (c) 2025 PTODT <https://ptodt.org.pl>
REM Autor: Michal Kolodziejski (2:480/112.10)
REM Data: 2025.10.01
REM Licencja: MIT

REM Przejdź do katalogu głównego projektu (jeden poziom wyżej)
cd /d "%~dp0.."

set VERSION_FILE=VERSION
set VERSION_HEADER=version.h

if not exist "%VERSION_FILE%" (
    echo Error: File %VERSION_FILE% does not exist
    exit /b 1
)

for /f "delims=" %%i in (%VERSION_FILE%) do set VERSION=%%i
for /f "tokens=1-3 delims=/" %%a in ('date /t') do set DATE=%%c-%%a-%%b
for /f "tokens=1-2 delims= " %%a in ("%DATE%") do set DATE=%%a
for /f "tokens=1-2 delims= " %%a in ('time /t') do set TIME=%%a
set BUILD_TIME=%DATE:T=%%TIME::=%
set BUILD_DATETIME=%DATE% %TIME%

(
echo /*****************************************************************************
echo     plik  : version.h
echo     autor : Michal Kolodziejski (2:480/112.10^)
echo     data  : %DATE%
echo     copyright  : PTODT ^<https://ptodt.org.pl^>
echo.
echo     opis  : automatycznie generowany plik z informacjami o wersji
echo.
echo     licencja : MIT
echo *****************************************************************************/
echo.
echo #ifndef VERSION_H
echo #define VERSION_H
echo.
echo #define VERSION_STRING "%VERSION%"
echo #define VERSION_DATE "%DATE%"
echo #define BUILD_TIME "%BUILD_TIME%"
echo #define BUILD_DATETIME "%BUILD_DATETIME%"
echo #define COPYRIGHT_STRING "PTODT"
echo.
echo #endif
) > %VERSION_HEADER%

echo Updated %VERSION_HEADER% with version %VERSION% (%DATE%) - build time: %BUILD_TIME%
