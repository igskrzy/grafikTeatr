#ifndef DYSPO_H
#define DYSPO_H

struct Dyspo{
    int pracownik_id;
    int tydzien_id;
    bool zmiany[7][3] = {false};

    Dyspo();
    Dyspo(int pracownik_id, int tydzien_id);
};

Dyspo** wczytajDyspoZPliku(const char* nazwaPliku, int il_pracownikow);

#endif // DYSPO_H