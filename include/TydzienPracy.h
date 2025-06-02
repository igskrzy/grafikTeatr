#ifndef TYDZIEN_PRACY_H
#define TYDZIEN_PRACY_H

#include "Teatr.h"
#include "MiesiacPracy.h"
#include "Tydzien.h"
#include "CzasPracy.h"
#include "Dyspo.h"
#include "Pracownik.h"

#include <vector>

struct TydzienPracy{
    Teatr* teatr;
    MiesiacPracy* miesiac;
    Tydzien* tydzien;
    Pracownik* pracujacy[7][3][6];
    int il_pracujacych[7][10];
    bool bufetowy[7][3];
    bool programowy[7][3];
    bool*** zmiany_pracownikow;
    Dyspo** dyspo;
    CzasPracy* tymczasowy_czas_pracy;

    TydzienPracy();

    ~TydzienPracy();

    static TydzienPracy* zacznijTydzienPracy(const char* plikTeatr, const char* plikMiesiac, const char* plikTydzien, const char* plikDyspo);

    void wypiszTeatr();

    void wypiszTydzien();

    bool czy_spektakle_koliduja(int i, int j1, int j2);

    bool czy_pracownik_dostepny(int pracownik_id, int i, int j);

    void dodaj_pracownika_do_zmiany(int i, int j, int id, int miejsce);

    void przydziel_pracownikow(int i, int j, std::vector<int> dostepni_pracownicy);

    inline void sortuj_pracownikow(std::vector<int>& dostepni_pracownicy);

    void uzupelnij_braki();

    void przydziel_zmiany();

    void zapiszZmianyDoPliku(const char* nazwaPliku);

    void wczytajZmianyZPliku(const char* nazwaPliku);

    void zaktualizujTymczasowyCzasPracy();

    void zatwierdzZmiany(const char* nazwaPliku);
};

#endif // TYDZIEN_PRACY_H
