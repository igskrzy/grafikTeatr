#include <iostream>
#include "TydzienPracy.h"

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Oczekiwano: " << argv[0] << " <teatr.csv> <miesiac.csv> <tydzien.csv> <grafik_in.csv> <grafik_out.csv> <miesiac_out.csv>\n";
        return 1;
    }

    const char* teatr_in = argv[1];
    const char* miesiac_in = argv[2];
    const char* tydzien_in = argv[3];
    const char* grafik_in = argv[4];
    const char* grafik_out = argv[5];
    const char* miesiac_out = argv[6];

    try {
        TydzienPracy* tydzienPracy = TydzienPracy::zacznijTydzienPracy(teatr_in, miesiac_in, tydzien_in, "\0", false);
        tydzienPracy->wczytajZmianyZPliku(grafik_in);
        tydzienPracy->zapiszZmianyDoPliku(grafik_out);
        tydzienPracy->zatwierdzZmiany(miesiac_out);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "Zakonczono pomyslnie.\n";
    return 0;
}
