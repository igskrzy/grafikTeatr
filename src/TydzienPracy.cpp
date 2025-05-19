#include "TydzienPracy.h"

#include <iostream>
#include <fstream>
#include <sstream>

TydzienPracy::TydzienPracy(): teatr(nullptr), miesiac(nullptr), tydzien(nullptr), dyspo(nullptr), tymczasowy_czas_pracy(nullptr) {
    for(int i=0; i<7; ++i){
        for(int j=0; j<3; ++j){
            bufetowy[i][j] = false;
            programowy[i][j] = false;
            il_pracujacych[i][j] = 0;
            for(int k=0; k<6; ++k){
                pracujacy[i][j][k] = nullptr;
            }
        }
    }
}

TydzienPracy::~TydzienPracy(){
    delete teatr;
    delete miesiac;
    delete tydzien;
    if (dyspo != nullptr){
        for (int i = 0; i < teatr->il_pracownikow; ++i){
            delete dyspo[i];
        }
        delete[] dyspo;
    }
    delete[] tymczasowy_czas_pracy;
}

TydzienPracy* TydzienPracy::zacznijTydzienPracy(const char* plikTeatr, const char* plikMiesiac, const char* plikTydzien, const char* plikDyspo){
    TydzienPracy* tydzienPracy = new TydzienPracy();
    Teatr* teatr = Teatr::wczytajZPliku(plikTeatr);
    tydzienPracy->teatr = teatr;
    MiesiacPracy* miesiac = new MiesiacPracy(teatr);
    miesiac->wczytajCzasPracyZPliku(plikMiesiac);
    tydzienPracy->miesiac = miesiac;
    tydzienPracy->tymczasowy_czas_pracy = new CzasPracy[teatr->il_pracownikow];
    for (int i = 0; i < teatr->il_pracownikow; ++i){
        tydzienPracy->tymczasowy_czas_pracy[i].czas = miesiac->czas_pracy[i].czas;
    }
    Tydzien* tydzien = Tydzien::wczytajZPliku(plikTydzien);
    tydzienPracy->tydzien = tydzien;
    Dyspo** dyspo = wczytajDyspoZPliku(plikDyspo, teatr->il_pracownikow);
    tydzienPracy->dyspo = dyspo;
    if(tydzien->tydzien_id == dyspo[0]->tydzien_id)
        return tydzienPracy;
    else{
        std::cout << tydzien->tydzien_id << " " << dyspo[0]->tydzien_id << std::endl;
        std::cerr << "Blad: tydzien::tydzien_id != dyspo[0]::tydzien_id" << std::endl;
        return nullptr;
    }
}

void TydzienPracy::wypiszTeatr(){ teatr->wypisz(); }

void TydzienPracy::wypiszTydzien(){ tydzien->wypisz(); }

int TydzienPracy::znajdz_pracownika_min(int i, int j, Stanowisko stanowisko){
    int min_id = -1;
    for(int k=0; k<teatr->il_pracownikow; ++k){
        if((teatr->pracownicy[k]->stanowisko == stanowisko)
            && dyspo[k]->zmiany[i][j] && (min_id == -1 || tymczasowy_czas_pracy[k].czas < tymczasowy_czas_pracy[min_id].czas)
            && !czy_pracownik_pracuje_w_ten_dzien(teatr->pracownicy[k],i,j)){
                min_id = k;
            }
    }
    if(stanowisko == Stanowisko::SZATNIARZ && min_id == -1){
        for(int k=0; k<teatr->il_pracownikow; ++k){
            if(dyspo[k]->zmiany[i][j] && (min_id == -1 || tymczasowy_czas_pracy[k].czas < tymczasowy_czas_pracy[min_id].czas)
                && !czy_pracownik_pracuje_w_ten_dzien(teatr->pracownicy[k],i,j)){
                    min_id = k;
                }
        }
    }
    return min_id;
}

bool TydzienPracy::czy_pracownik_pracuje_w_ten_dzien(Pracownik* pracownik, int i, int j_s){
    bool zmiany[3] = {false, false, false};
    for(int j=0; j<3; ++j){
        if(tydzien->dzien[i][j]!=nullptr){
            for(int k=0; k<tydzien->dzien[i][j]->il_pracownikow; ++k){
                if(pracownik == pracujacy[i][j][k]) zmiany[j] = true;
            }
        }
    }
    if(zmiany[j_s])return true;
    else if(zmiany[0] && !zmiany[j_s % 2 + 1]){
        int czas_trwania0 = tydzien->dzien[i][0]->czas_trwania;
        int godzina_rozpoczecia0 = tydzien->dzien[i][0]->godzina_rozpoczecia.toMinuty();
        int godzina_rozpoczecia1 = tydzien->dzien[i][j_s]->godzina_rozpoczecia.toMinuty();
        int roznica = godzina_rozpoczecia1 - godzina_rozpoczecia0 - czas_trwania0;
        if(roznica < 0)return true;
        return false;
    }
    else if(zmiany[0] || zmiany[1] || zmiany[2]) return true;
    return false;
}

void TydzienPracy::przydziel_zmiany(){
    for(int i=0; i<teatr->il_pracownikow; ++i){
        tymczasowy_czas_pracy[i].czas = miesiac->czas_pracy[i].czas;
    }
    for(int i=0; i<7; ++i){
        for(int j=0; j<3; ++j){
            if(tydzien->dzien[i][j] != nullptr){
                il_pracujacych[i][j] = 0;
                bufetowy[i][j] = 0;
                programowy[i][j] = 0;
                if(!programowy[i][j]){
                    int p_id = znajdz_pracownika_min(i,j,Stanowisko::PROGRAMOWY);
                    if(p_id!=-1){
                        tymczasowy_czas_pracy[p_id].czas += tydzien->dzien[i][j]->czas_trwania;
                        pracujacy[i][j][0] = teatr->pracownicy[p_id];
                        ++il_pracujacych[i][j];
                        programowy[i][j] = true;
                    }
                }
                if(!bufetowy[i][j] && tydzien->dzien[i][j]->bufetowy_potrzebny){
                    int p_id = znajdz_pracownika_min(i,j,Stanowisko::BUFETOWY);
                    if(p_id!=-1){
                        tymczasowy_czas_pracy[p_id].czas += tydzien->dzien[i][j]->czas_trwania;
                        pracujacy[i][j][1] = teatr->pracownicy[p_id];
                        ++il_pracujacych[i][j];
                        bufetowy[i][j] = true;
                    }
                }
            }
        }
    }

    bool koniec = false;
    while(!koniec){
        koniec = true;
        for(int i=0; i<7; ++i){
            for(int j=0; j<3; ++j){
                if(tydzien->dzien[i][j] != nullptr && il_pracujacych[i][j] < tydzien->dzien[i][j]->il_pracownikow){
                    int p_id = znajdz_pracownika_min(i,j,Stanowisko::SZATNIARZ);
                    if(p_id!=-1 && pracujacy[i][j][il_pracujacych[i][j]-1]->id != p_id){
                        koniec = false;
                        if(tydzien->dzien[i][j]!=nullptr && il_pracujacych[i][j] < tydzien->dzien[i][j]->il_pracownikow){
                            tymczasowy_czas_pracy[p_id].czas += tydzien->dzien[i][j]->czas_trwania;
                            pracujacy[i][j][il_pracujacych[i][j]] = teatr->pracownicy[p_id];
                            ++il_pracujacych[i][j];
                        }
                    }
                }
            }
        }
    }
}

void TydzienPracy::wypiszGrafik(){
    for(int i=0; i<7; ++i){
        for(int j=0; j<3; ++j){
            if(tydzien->dzien[i][j]!=nullptr){
                std::cout << tydzien->dzien[i][j]->data << " " << tydzien->dzien[i][j]->nazwa << std::endl;
                for(int k=0; k<tydzien->dzien[i][j]->il_pracownikow; ++k){
                    if(pracujacy[i][j][k] != nullptr)
                        pracujacy[i][j][k]->wypisz();
                    else
                    std::cout << "BRAK!" << std::endl;
                }
            }
        }
    }
}

void TydzienPracy::zapiszZmianyDoPliku(const char* nazwaPliku){
    std::ofstream plik(nazwaPliku);
    if (!plik) {
        std::cerr << "Blad otwierania pliku grafik, proba zapisu niepomyslna" << std::endl;
        return;
    }

    plik << "DATA;SCENA;SPEKTAKL;";
    for (int i = 1; i <= 9; ++i) {
        plik << i << ";";
    }
    plik << "\n";

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 3; ++j) {
            Spektakl* spektakl = tydzien->dzien[i][j];
            if (spektakl != nullptr) {
                plik << spektakl->data << ";"
                        << spektakl->scena << ";"
                        << spektakl->nazwa << ";";

                for (int k = 0; k < spektakl->il_pracownikow; ++k) {
                    Pracownik* pracownik = pracujacy[i][j][k];
                    if (pracownik != nullptr) {
                        plik << pracownik->imie << " "
                                << pracownik->nazwisko << ";";
                    } else {
                        plik << "BRAK;";
                    }
                }
                plik << "\n";
            }
        }
    }

    plik.close();
}

void TydzienPracy::wczytajZmianyZPliku(const char* nazwaPliku){
    std::ifstream plik(nazwaPliku);
    if (!plik) {
        std::cerr << "Blad otwierania pliku grafik, proba odczytu niepomyslna" << std::endl;
        return;
    }

    std::string linia;
    std::string komorka;
    std::getline(plik, linia);
    for(int i=0; i<7; ++i){
        for(int j=0; j<3; ++j){
            Spektakl* spektakl = tydzien->dzien[i][j];
            if(spektakl != nullptr){
                std::string data;
                std::getline(plik, data, ';');
                std::string scena;
                std::getline(plik, scena, ';');
                std::string nazwa;
                std::getline(plik, nazwa, ';');
                if(data == spektakl->data && scena == spektakl->scena && nazwa == spektakl->nazwa){
                    for(int k=0; k<spektakl->il_pracownikow; ++k){
                        std::string imie_i_nazwisko;
                        std::getline(plik, imie_i_nazwisko, ';');
                        if(imie_i_nazwisko == "BRAK")
                            pracujacy[i][j][k] = nullptr;
                        else{
                            size_t spacja = imie_i_nazwisko.find(' ');
                            std::string imie = imie_i_nazwisko.substr(0, spacja);
                            std::string nazwisko = imie_i_nazwisko.substr(spacja + 1);
                            Pracownik* pracownik = teatr->szukajPracownika(imie, nazwisko);
                            if(pracownik != nullptr)
                                pracujacy[i][j][k] = pracownik;
                            else{
                                std::cerr << "Blad wczytajZmianyZPliku - brak pracownika: " << imie << " " << nazwisko << std::endl;
                                pracujacy[i][j][k] = nullptr;
                            }
                        }
                    }
                }
                else{
                    std::cerr << "Blad wczytajZmianyZPliku - dane spektaklu niepoprawne, oczekiwane: " <<
                    spektakl->data << " " << spektakl->scena << " " << spektakl->nazwa << std::endl <<
                    "Na wejsciu: " << data << " " << scena << " " << nazwa << std::endl;
                    return;
                }
                std::getline(plik, linia);
            }
        }
    }
    plik.close();
}

void TydzienPracy::zaktualizujTymczasowyCzasPracy(){
    for(int i=0; i<teatr->il_pracownikow; ++i){
        tymczasowy_czas_pracy[i].czas = miesiac->czas_pracy[i].czas;
    }
    for(int i=0; i<7; ++i){
        for(int j=0; j<3; ++j){
            Spektakl* spektakl = tydzien->dzien[i][j];
            if(spektakl != nullptr){
                for(int k=0; k<spektakl->il_pracownikow; ++k){
                    Pracownik* pracownik = pracujacy[i][j][k];
                    if(pracownik != nullptr){
                        tymczasowy_czas_pracy[pracownik->id].czas += spektakl->czas_trwania;
                    }
                }
            }
        }
    }
}

void TydzienPracy::zatwierdzZmiany(const char* nazwaPliku){
    zaktualizujTymczasowyCzasPracy();
    for(int i=0; i<teatr->il_pracownikow; ++i){
        miesiac->czas_pracy[i].czas = tymczasowy_czas_pracy[i].czas;
    }
    miesiac->zapiszCzasPracyDoPliku(nazwaPliku);
}
