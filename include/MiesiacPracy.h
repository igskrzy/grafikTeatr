#ifndef MIESIAC_PRACY_H
#define MIESIAC_PRACY_H

#include "Teatr.h"
#include "CzasPracy.h"

struct MiesiacPracy{
    int miesiac_id;
    Teatr* teatr;
    CzasPracy* czas_pracy;

    MiesiacPracy(Teatr* teatr);

    ~MiesiacPracy();

    void wczytajCzasPracyZPliku(const char *nazwaPliku);

    void zapiszCzasPracyDoPliku(const char *nazwaPliku);
};

#endif // MIESIAC_PRACY_H