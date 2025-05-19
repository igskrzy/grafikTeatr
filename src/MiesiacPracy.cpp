#include "MiesiacPracy.h"

#include <iostream>
#include <fstream>
#include <sstream>

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
        std::cerr << "Blad otwierania pliku miesiac" << std::endl;
        return;
    }

    std::string linia;
    if (!std::getline(plik, linia)) {
        std::cerr << "Blad odczytu pierwszej linii" << std::endl;
        return;
    }

    std::stringstream ss(linia);
    std::string label, wartosc;
    if (!std::getline(ss, label, ';') || !std::getline(ss, wartosc)) {
        std::cerr << "Blad formatu pierwszej linii" << std::endl;
        return;
    }
    if (label != "miesiac_id") {
        std::cerr << "Niepoprawny naglowek: oczekiwano miesiac_id" << std::endl;
        return;
    }
    miesiac_id = std::stoi(wartosc);

    if (!std::getline(plik, linia)) {
        std::cerr << "Brak naglowka kolumn" << std::endl;
        return;
    }

    for (int i = 0; i < teatr->il_pracownikow; ++i) {
        if (!std::getline(plik, linia)) {
            std::cerr << "Brak danych dla pracownika nr " << i << std::endl;
            return;
        }

        std::stringstream ss(linia);
        std::string idStr, czasStr;

        if (!std::getline(ss, idStr, ';') || !std::getline(ss, czasStr)) {
            std::cerr << "Blad formatu danych w linii " << i + 3 << std::endl;
            return;
        }

        int id = std::stoi(idStr);

        if (id != teatr->pracownicy[i]->id) {
            std::cerr << "Blad indeksowania pracownikow: id z pliku (" << id
                      << ") != id w pamieci (" << teatr->pracownicy[i]->id << ")" << std::endl;
            return;
        }
        czas_pracy[i].czas = std::stoi(czasStr);
    }
}

void MiesiacPracy::zapiszCzasPracyDoPliku(const char *nazwaPliku) {
    std::ofstream plik(nazwaPliku);
    if (!plik) {
        std::cerr << "Blad otwierania pliku do zapisu" << std::endl;
        return;
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
