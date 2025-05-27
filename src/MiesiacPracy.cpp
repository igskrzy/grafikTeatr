#include "MiesiacPracy.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

MiesiacPracy::MiesiacPracy(Teatr* teatr): miesiac_id(0), teatr(teatr){
    czas_pracy = new CzasPracy[teatr->il_pracownikow];
    for(int i=0; i<teatr->il_pracownikow; ++i){
        czas_pracy[i].pracownik = teatr->pracownicy[i];
        czas_pracy[i].czas = 0;
    }
}

MiesiacPracy::~MiesiacPracy(){
    delete[] czas_pracy;
}

void MiesiacPracy::wczytajCzasPracyZPliku(const char* nazwaPliku) {
    std::ifstream plik(nazwaPliku);
    if (!plik) {
        throw std::runtime_error("Blad otwierania pliku w MiesiacPracy::wczytajCzasPracyZPliku");
    }

    std::string linia;
    if (!std::getline(plik, linia)) {
        throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: blad odczytu pierwszej linii");
    }

    std::stringstream ss(linia);
    std::string label, wartosc;
    if (!std::getline(ss, label, ';') || !std::getline(ss, wartosc)) {
        throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: Blad formatu pierwszej linii");
    }
    if (label != "miesiac_id") {
        throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: Niepoprawny naglowek: oczekiwano miesiac_id");
    }
    miesiac_id = std::stoi(wartosc);

    if (!std::getline(plik, linia)) {
        std::cerr << "Brak naglowka kolumn" << std::endl;
        throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: Brak naglowka kolumn");
    }

    for (int i = 0; i < teatr->il_pracownikow; ++i) {
        if (!std::getline(plik, linia)) {
            std::cerr << "Brak danych dla pracownika nr " << i << std::endl;
            throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: Brak danych dla pracownika");
        }

        std::stringstream ss(linia);
        std::string idStr, czasStr;

        if (!std::getline(ss, idStr, ';') || !std::getline(ss, czasStr)) {
            std::cerr << "Blad formatu danych w linii " << i + 3 << std::endl;
            throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: Blad formatu danych");
        }

        int id = std::stoi(idStr);

        if (id != teatr->pracownicy[i]->id) {
            std::cerr << "Blad indeksowania pracownikow: id z pliku (" << id
                      << ") != id w pamieci (" << teatr->pracownicy[i]->id << ")" << std::endl;
            throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: Blad indeksowania pracownikow");
        }
        czas_pracy[i].czas = std::stoi(czasStr);
    }
}

void MiesiacPracy::zapiszCzasPracyDoPliku(const char *nazwaPliku) {
    std::ofstream plik(nazwaPliku);
    if (!plik) {
        throw std::runtime_error("Blad w MiesiacPracy::wczytajCzasPracyZPliku: Blad otwierania pliku do zapisu");
    }

    plik << "miesiac_id;" << miesiac_id << '\n';
    plik << "pracownik_id;czas_pracy\n";

    for (int i = 0; i < teatr->il_pracownikow; ++i) {
        plik << teatr->pracownicy[i]->id << ';' << czas_pracy[i].czas << '\n';
    }

    plik.close();
}

int miesiac_id;
Teatr* teatr;
CzasPracy* czas_pracy;
