#include "Pracownik.h"

#include <iostream>

Pracownik::Pracownik(int id, std::string imie, std::string nazwisko, Stanowisko stanowisko) : id(id), imie(imie), nazwisko(nazwisko), stanowisko(stanowisko){}

void Pracownik::wypisz(){
    std::cout << id << ". " << imie << " " << nazwisko << ", " << stanowisko << std::endl;
}

void Pracownik::wypisz_krotko(){
    std::cout << imie << " " << nazwisko << ", ";
}
