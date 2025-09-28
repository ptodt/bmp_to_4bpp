/*****************************************************************************

    plik  : options.h
    autor : Michal Kolodziejski (2:480/112.10)
    data  : 2025.09.28
    copyright  : PTODT <https://ptodt.org.pl>

    opis  : obsługa argumentów wiersza poleceń i wyświetlanie pomocy

    licencja : MIT
*****************************************************************************/

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "defs.h"

// Prototypy funkcji obsługi argumentów
void print_usage(const char* program_name);
int parse_arguments(int argc, char* argv[], ConversionContext* context, char** input_file, char** output_file);

#endif
