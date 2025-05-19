#ifndef PRACOWNIK_H
#define PRACOWNIK_H

#include "../utils/Stanowisko.h"

struct Pracownik{
    int id;
    std::string imie;
    std::string nazwisko;
    Stanowisko stanowisko;

    Pracownik(int id, std::string imie, std::string nazwisko, Stanowisko stanowisko);

    void wypisz();

    void wypisz_krotko();
};

#endif // PRACOWNIK_H