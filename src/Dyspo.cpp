#include "Dyspo.h"

#include <iostream>
#include <fstream>
#include <sstream>

Dyspo::Dyspo(): pracownik_id(-1), tydzien_id(-1) {}
Dyspo::Dyspo(int pracownik_id, int tydzien_id) : pracownik_id(pracownik_id), tydzien_id(tydzien_id) {}

Dyspo** wczytajDyspoZPliku(const char* nazwaPliku, int il_pracownikow) {
    std::ifstream plik(nazwaPliku);
    if (!plik) {
        std::cerr << "Blad otwierania pliku dyspo" << std::endl;
        return nullptr;
    }

    std::string linia;
    if (!std::getline(plik, linia)) {
        std::cerr << "Blad odczytu pierwszej linii" << std::endl;
        return nullptr;
    }

    std::stringstream ss(linia);
    std::string label, wartosc;
    if (!std::getline(ss, label, ';') || !std::getline(ss, wartosc)) {
        std::cerr << "Blad formatu pierwszej linii" << std::endl;
        return nullptr;
    }
    if (label != "tydzien_id") {
        std::cerr << "Niepoprawny naglowek: oczekiwano tydzien_id" << std::endl;
        return nullptr;
    }
    int tydzien_id = std::stoi(wartosc);

    if (!std::getline(plik, linia)) {
        std::cerr << "Brak naglowka kolumn" << std::endl;
        return nullptr;
    }

    Dyspo** dyspo = new Dyspo*[il_pracownikow];

    for (int k = 0; k < il_pracownikow && std::getline(plik, linia); ++k) {
        std::stringstream ss(linia);
        std::string pole;
        dyspo[k] = new Dyspo();
        dyspo[k]->tydzien_id = tydzien_id;

        std::getline(ss, pole, ';');
        dyspo[k]->pracownik_id = std::stoi(pole);

        for (int i = 0; i < 7; ++i) {
            for(int j = 0; j < 3; ++j){
                if (!std::getline(ss, pole, ';')) {
                    dyspo[k]->zmiany[i][j] = 0;
                } else if (pole == "1" || pole == "1\n" || pole == "1\r"){
                    dyspo[k]->zmiany[i][j] = 1;
                } else{
                    dyspo[k]->zmiany[i][j] = 0;
                }
            }
        }
    }

    return dyspo;
}
