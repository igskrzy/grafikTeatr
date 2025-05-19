#ifndef CZAS_PRACY_H
#define CZAS_PRACY_H

#include "Pracownik.h"

struct CzasPracy{
    CzasPracy();
    CzasPracy(Pracownik* pracownik, int czas);
    Pracownik* pracownik;
    int czas;
};

#endif