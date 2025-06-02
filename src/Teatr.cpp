#include "Teatr.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

Teatr::Teatr(int il_pracownikow): il_pracownikow(il_pracownikow) {
    pracownicy = new Pracownik*[il_pracownikow];
    for(int i=0; i<il_pracownikow; ++i){
        pracownicy[i] = nullptr;
    }
}

Teatr::~Teatr(){ delete[] pracownicy; }

void Teatr::wypisz(){
    std::cout << "Pracownicy Teatru Starego: " << std::endl;
    for(int i=0; i<il_pracownikow; ++i){
        if(pracownicy[i] != nullptr) pracownicy[i]->wypisz();
    }
}

Teatr* Teatr::wczytajZPliku(const char* nazwaPliku) {
    std::ifstream plik(nazwaPliku);
    if (!plik) {
        throw std::runtime_error("Teatr::wczytajZPliku: Blad otwierania pliku teatr");
    }

    std::string linia, pole;

    if (!std::getline(plik, linia)) {
        throw std::runtime_error("Teatr::wczytajZPliku: Blad odczytu pierwszej linii");
    }
    std::stringstream ss_pierwsza(linia);
    std::getline(ss_pierwsza, pole, ';');
    if (pole != "il_pracownikow"){
        std::cerr << "Naglowek: \"" << pole << "\", oczekiwano: \"il_pracownikow\"" << std::endl;
        throw std::runtime_error("Teatr::wczytajZPliku: Blad odczytu naglowka");
    }
    std::getline(ss_pierwsza, pole, ';');
    int il_pracownikow;
    try{
        il_pracownikow = std::stoi(pole);
    } catch (const std::exception& e) {
        std::cerr << "Proba zamiany \"" << pole << "\" na int" << std::endl;
        throw e;
    }

    if (!std::getline(plik, linia)) {
        throw std::runtime_error("Teatr::wczytajZPliku: Brak nagłówków kolumn");
    }

    Teatr* teatr = new Teatr(il_pracownikow);

    for (int i = 0; i < il_pracownikow && std::getline(plik, linia); ++i) {
        std::stringstream ss(linia);
        int id;
        std::string imie, nazwisko;
        Stanowisko stanowisko;

        std::getline(ss, pole, ';');
        id = std::stoi(pole);

        std::getline(ss, imie, ';');
        std::getline(ss, nazwisko, ';');

        std::getline(ss, pole, ';');
        std::stringstream stanowisko_ss(pole);
        stanowisko_ss >> stanowisko;

        teatr->pracownicy[i] = new Pracownik(id, imie, nazwisko, stanowisko);
    }

    return teatr;
}


Pracownik* Teatr::szukajPracownika(std::string imie, std::string nazwisko){
    for(int i=0; i<il_pracownikow; ++i){
        Pracownik* pracownik = pracownicy[i];
        if(pracownik->imie == imie && pracownik->nazwisko == nazwisko)
            return pracownik;
    }
    return nullptr;
}
