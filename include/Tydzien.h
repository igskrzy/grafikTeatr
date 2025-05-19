#ifndef TYDZIEN_H
#define TYDZIEN_H

#include "../utils/Data.h"
#include "Spektakl.h"

struct Tydzien{
    int tydzien_id;
    Data pierwszy_dzien;
    Spektakl* dzien[7][3];

    Tydzien(int tydzien_id, Data pierwszy_dzien);

    ~Tydzien();

    static Tydzien* wczytajZPliku(const char *nazwaPliku);

    void wypisz();
};

#endif // TYDZIEN_H