#include "Tydzien.h"

#include <iostream>
#include <fstream>
#include <sstream>

Tydzien::Tydzien(int tydzien_id, Data pierwszy_dzien): tydzien_id(tydzien_id), pierwszy_dzien(pierwszy_dzien) {
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 3; ++j)
            dzien[i][j] = nullptr;
}

Tydzien::~Tydzien(){
    for (int i = 0; i<7; ++i) {
        for (int j = 0; j<3; ++j) {
            delete dzien[i][j];
        }
    }
}

Tydzien* Tydzien::wczytajZPliku(const char* nazwaPliku) {
    std::ifstream plik(nazwaPliku);
    if (!plik) {
        std::cerr << "Blad otwierania pliku tydzien" << std::endl;
        return nullptr;
    }

    std::string linia;

    if (!std::getline(plik, linia)) {
        std::cerr << "Blad odczytu naglowkow" << std::endl;
        return nullptr;
    }

    std::string pole;
    int tydzien_id;
    Data data;

    std::getline(plik, linia);
    std::stringstream ss1(linia);
    std::getline(ss1, pole, ';');
    tydzien_id = std::stoi(pole);
    std::getline(ss1, pole, ';');
    std::stringstream(pole) >> data;

    Tydzien* tydzien = new Tydzien(tydzien_id, data);

    for (int i = 0; i < 7 && std::getline(plik, linia); ++i) {
        std::stringstream ss2(linia);
        std::getline(ss2, pole, ';');
        std::getline(ss2, pole, ';');
        std::getline(ss2, pole, ';');
        int ilosc_spektakli = std::stoi(pole);
        for (int j = 0; j < ilosc_spektakli && std::getline(plik, linia); ++j) {
            std::stringstream ss3(linia);
            std::getline(ss3, pole, ';');
            std::getline(ss3, pole, ';');
            std::getline(ss3, pole, ';');
            tydzien->dzien[i][j] = new Spektakl();
            std::getline(ss3, tydzien->dzien[i][j]->nazwa, ';');
            std::getline(ss3, pole, ';');
            std::stringstream(pole) >> tydzien->dzien[i][j]->godzina_rozpoczecia;
            std::getline(ss3, pole, ';');
            tydzien->dzien[i][j]->czas_trwania = std::stoi(pole);
            std::getline(ss3, pole, ';');
            std::stringstream(pole) >> tydzien->dzien[i][j]->scena;
            std::getline(ss3, pole, ';');
            tydzien->dzien[i][j]->il_pracownikow = std::stoi(pole);
            std::getline(ss3, pole);
            tydzien->dzien[i][j]->bufetowy_potrzebny = std::stoi(pole);

            tydzien->dzien[i][j]->data = data;
        }
        for(int k=ilosc_spektakli; k<3; ++k){
                tydzien->dzien[i][k] = nullptr;
            }
        data.nastepny_dzien();
    }
    return tydzien;
}


void Tydzien::wypisz(){
    for(int i=0; i<7; ++i){
        for(int j=0; j<3; ++j){
            if(dzien[i][j] != nullptr){
                dzien[i][j]->wypisz();
            }
        }
    }
    std::cout << std::endl;
}
