#ifndef TEATR_H
#define TEATR_H

#include "Pracownik.h"

struct Teatr{
    int il_pracownikow;
    Pracownik** pracownicy;

    Teatr(int il_pracownikow);
    ~Teatr();

    void wypisz();

    static Teatr* wczytajZPliku(const char *nazwaPliku);

    Pracownik* szukajPracownika(std::string imie, std::string nazwisko);
};

#endif // TEATR_H