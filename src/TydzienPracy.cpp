#include "TydzienPracy.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

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
    delete miesiac;
    delete tydzien;
    if (dyspo != nullptr){
        for (int i = 0; i < teatr->il_pracownikow; ++i){
            delete dyspo[i];
            for (int j = 0; j < 7; ++j) {
                delete[] zmiany_pracownikow[i][j];
            }
            delete[] zmiany_pracownikow[i];
        }
        delete[] dyspo;
    }
    delete[] tymczasowy_czas_pracy;
    delete teatr;
}

TydzienPracy* TydzienPracy::zacznijTydzienPracy(const char* plikTeatr, const char* plikMiesiac, const char* plikTydzien, const char* plikDyspo){
    TydzienPracy* tydzienPracy = new TydzienPracy();
    Teatr* teatr = Teatr::wczytajZPliku(plikTeatr);
    tydzienPracy->teatr = teatr;
    tydzienPracy->zmiany_pracownikow = new bool**[teatr->il_pracownikow];
    for (int i = 0; i < teatr->il_pracownikow; ++i) {
        tydzienPracy->zmiany_pracownikow[i] = new bool*[7];
        for (int j = 0; j < 7; ++j) {
            tydzienPracy->zmiany_pracownikow[i][j] = new bool[3]{false, false, false};
        }
    }
    MiesiacPracy* miesiac = new MiesiacPracy(teatr);
    miesiac->wczytajCzasPracyZPliku(plikMiesiac);
    tydzienPracy->miesiac = miesiac;
    tydzienPracy->tymczasowy_czas_pracy = new CzasPracy[teatr->il_pracownikow];
    for (int i = 0; i < teatr->il_pracownikow; ++i){
        tydzienPracy->tymczasowy_czas_pracy[i].czas = miesiac->czas_pracy[i].czas;
    }
    Tydzien* tydzien = Tydzien::wczytajZPliku(plikTydzien);
    tydzienPracy->tydzien = tydzien;
    Dyspo** dyspo = wczytajDyspoZPliku(plikDyspo, teatr->il_pracownikow, tydzien);
    tydzienPracy->dyspo = dyspo;
    if(tydzien->tydzien_id == dyspo[0]->tydzien_id)
        return tydzienPracy;
    else{
        std::cout << tydzien->tydzien_id << " " << dyspo[0]->tydzien_id << std::endl;
        throw std::runtime_error("tydzien::tydzien_id != dyspo[0]::tydzien_id");
    }
}

void TydzienPracy::wypiszTeatr(){ teatr->wypisz(); }

void TydzienPracy::wypiszTydzien(){ tydzien->wypisz(); }

bool TydzienPracy::czy_spektakle_koliduja(int i, int j1, int j2){
    int godzina_rozpoczecia1 = tydzien->dzien[i][j1]->godzina_rozpoczecia.toMinuty();
    int godzina_rozpoczecia2 = tydzien->dzien[i][j2]->godzina_rozpoczecia.toMinuty();
    int czas_trwania;
    int roznica;
    if(godzina_rozpoczecia1 <= godzina_rozpoczecia2){
        czas_trwania = tydzien->dzien[i][j1]->czas_trwania;
        roznica = godzina_rozpoczecia2 - godzina_rozpoczecia1 - czas_trwania;
    }
    else{
        czas_trwania = tydzien->dzien[i][j2]->czas_trwania;
        roznica = godzina_rozpoczecia1 - godzina_rozpoczecia2 - czas_trwania;
    }
    if(roznica < 0)return true;
    return false;
}

bool TydzienPracy::czy_pracownik_dostepny(int pracownik_id, int i, int j){
    if(zmiany_pracownikow[pracownik_id][i][j]) return false;
    int j2 = (j+1) % 3;
    int j3 = (j+2) % 3;
    if(zmiany_pracownikow[pracownik_id][i][j2] && czy_spektakle_koliduja(i,j,j2)) return false;
    else if(zmiany_pracownikow[pracownik_id][i][j3] && czy_spektakle_koliduja(i,j,j3)) return false;
    return true;
}

void TydzienPracy::dodaj_pracownika_do_zmiany(int i, int j, int id, int miejsce){
    pracujacy[i][j][miejsce] = teatr->pracownicy[id];
    tymczasowy_czas_pracy[id].czas += tydzien->dzien[i][j]->czas_trwania;
    zmiany_pracownikow[id][i][j] = true;
    il_pracujacych[i][j] += 1;
}

void TydzienPracy::przydziel_pracownikow(int i, int j, std::vector<int> dostepni_pracownicy){
    il_pracujacych[i][j] = 0;
    programowy[i][j] = false;
    bufetowy[i][j] = false;

    for (int id : dostepni_pracownicy) {
        Stanowisko s = teatr->pracownicy[id]->stanowisko;
        if (!programowy[i][j] && s == Stanowisko::PROGRAMOWY) {
            programowy[i][j] = true;
            dodaj_pracownika_do_zmiany(i, j, id, 0);
        }
        else if (!bufetowy[i][j] && s == Stanowisko::BUFETOWY) {
            bufetowy[i][j] = true;
            dodaj_pracownika_do_zmiany(i, j, id, 1);
        }
        if (programowy[i][j] && bufetowy[i][j]) break;
    }

    int miejsce_szatniarza = 2;
    for (int id : dostepni_pracownicy) {
        Stanowisko s = teatr->pracownicy[id]->stanowisko;
        if (s == Stanowisko::SZATNIARZ && il_pracujacych[i][j] < tydzien->dzien[i][j]->il_pracownikow) {
            dodaj_pracownika_do_zmiany(i, j, id, miejsce_szatniarza);
            ++miejsce_szatniarza;
        }
        if (il_pracujacych[i][j] >= tydzien->dzien[i][j]->il_pracownikow){
            break;
        }
    }
}

inline void TydzienPracy::sortuj_pracownikow(std::vector<int>& pracownicy) {
    std::sort(pracownicy.begin(), pracownicy.end(),
        [this](int a, int b) {
            return tymczasowy_czas_pracy[a].czas < tymczasowy_czas_pracy[b].czas;
        });
}

void TydzienPracy::uzupelnij_braki() {
    struct Zmiana{ int i, j, brak, czas_trwania; };
    std::vector<Zmiana> zmiany_z_brakami;
    int suma_brakow = 0;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!tydzien->dzien[i][j]) continue;
            int potrzebni = tydzien->dzien[i][j]->il_pracownikow;
            int obecni = il_pracujacych[i][j];
            if (obecni < potrzebni) {
                int brak = potrzebni - obecni;
                zmiany_z_brakami.push_back({i, j, brak, tydzien->dzien[i][j]->czas_trwania});
                suma_brakow += brak;
            }
        }
    }

    if (zmiany_z_brakami.empty()) return;

    std::sort(zmiany_z_brakami.begin(), zmiany_z_brakami.end(),
    [](const Zmiana& a, const Zmiana& b) {
        if (a.brak != b.brak) return a.brak > b.brak;
        return a.czas_trwania > b.czas_trwania;
    });

    std::vector<int> pracownicy;
    for (int k = 0; k < teatr->il_pracownikow; ++k) {
        Stanowisko s = teatr->pracownicy[k]->stanowisko;
        if (s != Stanowisko::PROGRAMOWY && s != Stanowisko::BUFETOWY) continue;
        pracownicy.push_back(k);
    }

    sortuj_pracownikow(pracownicy);

    for (auto& zm : zmiany_z_brakami) {
       for (int pracownik : pracownicy) {
            if (il_pracujacych[zm.i][zm.j] >= tydzien->dzien[zm.i][zm.j]->il_pracownikow)
                break;
            if (!dyspo[pracownik]->zmiany[zm.i][zm.j] || zmiany_pracownikow[pracownik][zm.i][zm.j] ||
                !czy_pracownik_dostepny(pracownik, zm.i, zm.j)) continue;
            dodaj_pracownika_do_zmiany(zm.i, zm.j, pracownik, il_pracujacych[zm.i][zm.j]);
        }
        sortuj_pracownikow(pracownicy);
    }
}

void TydzienPracy::przydziel_zmiany(){
    std::vector<std::pair<int, int>> zmiany;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tydzien->dzien[i][j] != nullptr) {
                zmiany.emplace_back(i, j);
            }
        }
    }

    std::sort(zmiany.begin(), zmiany.end(), [&](const auto& a, const auto& b) {
        return tydzien->dzien[a.first][a.second]->czas_trwania > tydzien->dzien[b.first][b.second]->czas_trwania;
    });

    for (const auto& [i, j] : zmiany) {
        std::vector<int> dostepni_pracownicy;
        for (int k = 0; k < teatr->il_pracownikow; ++k) {
            if (dyspo[k]->zmiany[i][j] && czy_pracownik_dostepny(k, i, j)) {
                dostepni_pracownicy.push_back(k);
            }
        }

        sortuj_pracownikow(dostepni_pracownicy);

        przydziel_pracownikow(i, j, dostepni_pracownicy);
    }
    uzupelnij_braki();
}

void TydzienPracy::zapiszZmianyDoPliku(const char* nazwaPliku){
    std::ofstream plik(nazwaPliku);
    if (!plik) {
        throw std::runtime_error("Blad otwierania pliku w TydzienPracy::zapiszZmianyDoPliku, proba zapisu niepomyslna");
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
    std::cout << "Zapisano grafik do pliku " << nazwaPliku << std::endl;
    plik.close();
}

void TydzienPracy::wczytajZmianyZPliku(const char* nazwaPliku){
    std::ifstream plik(nazwaPliku);
    if (!plik) {
        throw std::runtime_error("Blad otwierania pliku w TydzienPracy::wczytajZmianyZPliku, proba odczytu niepomyslna");
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
                                std::cerr << "Brak pracownika: " << imie << " " << nazwisko << std::endl;
                                throw std::runtime_error("Blad w TydzienPracy::wczytajZmianyZPliku: brak pracownika");
                            }
                        }
                    }
                }
                else{
                    std::cerr << "Dane spektaklu niepoprawne, oczekiwane: " <<
                    spektakl->data << " " << spektakl->scena << " " << spektakl->nazwa << std::endl <<
                    "Na wejsciu: " << data << " " << scena << " " << nazwa << std::endl;
                    throw std::runtime_error("TydzienPracy::wczytajZmianyZPliku - dane spektaklu niepoprawne");
                }
                std::getline(plik, linia);
            }
        }
    }
    std::cout << "Wczytano grafik z pliku " << nazwaPliku << std::endl;
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
    if(teatr == nullptr || tydzien == nullptr || miesiac == nullptr || dyspo == nullptr)
        return;
    zaktualizujTymczasowyCzasPracy();
    for(int i=0; i<teatr->il_pracownikow; ++i){
        miesiac->czas_pracy[i].czas = tymczasowy_czas_pracy[i].czas;
    }
    miesiac->zapiszCzasPracyDoPliku(nazwaPliku);
    std::cout << "Zapisano godziny pracy do pliku " << nazwaPliku << std::endl;
}
