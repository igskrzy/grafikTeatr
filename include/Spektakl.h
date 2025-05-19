#ifndef SPEKTAKL_H
#define SPEKTAKL_H

#include "../utils/Godzina.h"
#include "../utils/Data.h"

struct Spektakl{
    Spektakl();
    Spektakl(std::string nazwa, Data data, std::string scena, int il_pracownikow, bool bufetowy_potrzebny, int czas_trwania, Godzina godzina_rozpoczecia);

    std::string nazwa;
    Data data;
    std::string scena;
    int il_pracownikow;
    bool bufetowy_potrzebny;
    int czas_trwania;
    Godzina godzina_rozpoczecia;

    void wypisz();
};

#endif // SPEKTAKL_H
