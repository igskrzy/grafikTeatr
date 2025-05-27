#ifndef DYSPO_H
#define DYSPO_H

#include "Tydzien.h"

struct Dyspo{
    int pracownik_id;
    int tydzien_id;
    bool zmiany[7][3] = {false};

    Dyspo();
    Dyspo(int pracownik_id, int tydzien_id);
};

Dyspo** wczytajDyspoZPliku(const char* nazwaPliku, int il_pracownikow, Tydzien* tydzien);

#endif // DYSPO_H
