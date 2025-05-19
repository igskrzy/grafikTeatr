#include "Spektakl.h"

#include <iostream>

Spektakl::Spektakl() : nazwa("brak"), data(Data()), scena("brak"), il_pracownikow(0), czas_trwania(0), godzina_rozpoczecia(Godzina()) {}
Spektakl::Spektakl(std::string nazwa, Data data, std::string scena, int il_pracownikow, bool bufetowy_potrzebny, int czas_trwania, Godzina godzina_rozpoczecia)
        : nazwa(nazwa), data(data), scena(scena), il_pracownikow(il_pracownikow), bufetowy_potrzebny(bufetowy_potrzebny), czas_trwania(czas_trwania), godzina_rozpoczecia(godzina_rozpoczecia) {}

void Spektakl::wypisz(){
    std::cout << "========================" << std::endl;
    std::cout << "Spektakl: " << nazwa << std::endl;
    std::cout << "Data: " << data << std::endl;
    std::cout << "Scena: " << scena << std::endl;
    std::cout << "Czas trwania: " << czas_trwania << "min" << std::endl;
    std::cout << "Godzina rozpoczecia: " << godzina_rozpoczecia << std::endl;
    std::cout << "Ilosc pracownikow: " << il_pracownikow << std::endl;
    std::cout << "Bufetowy potrzebny?: " << bufetowy_potrzebny << std::endl;
    std::cout << "========================" << std::endl;
}
