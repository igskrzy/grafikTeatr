#ifndef TYDZIEN_PRACY_H
#define TYDZIEN_PRACY_H

#include "Teatr.h"
#include "MiesiacPracy.h"
#include "Tydzien.h"
#include "CzasPracy.h"
#include "Dyspo.h"
#include "Pracownik.h"

struct TydzienPracy{
    Teatr* teatr;
    MiesiacPracy* miesiac;
    Tydzien* tydzien;
    Pracownik* pracujacy[7][3][6];
    int il_pracujacych[7][10];
    bool bufetowy[7][3];
    bool programowy[7][3];
    Dyspo** dyspo;
    CzasPracy* tymczasowy_czas_pracy;

    TydzienPracy();

    ~TydzienPracy();

    static TydzienPracy* zacznijTydzienPracy(const char* plikTeatr, const char* plikMiesiac, const char* plikTydzien, const char* plikDyspo);

    void wypiszTeatr();

    void wypiszTydzien();

    int znajdz_pracownika_min(int i, int j, Stanowisko stanowisko);

    bool czy_pracownik_pracuje_w_ten_dzien(Pracownik* pracownik, int i, int j_s);

    void przydziel_zmiany();

    void wypiszGrafik();

    void zapiszZmianyDoPliku(const char* nazwaPliku);

    void wczytajZmianyZPliku(const char* nazwaPliku);

    void zaktualizujTymczasowyCzasPracy();

    void zatwierdzZmiany(const char* nazwaPliku);
};

#endif // TYDZIEN_PRACY_H