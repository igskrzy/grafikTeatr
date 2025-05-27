#include "TydzienPracy.h"

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Oczekiwano: " << argv[0] << " <tydzien.csv> <dyspo.csv>\n";
        return 1;
    }
    const char* tydzien = argv[1];
    const char* dyspo = argv[2];
    try {
        TydzienPracy* tydzienPracy = TydzienPracy::zacznijTydzienPracy("data/teatr_test.csv","data/miesiac_test.csv",tydzien,dyspo);
        //tydzienPracy->wczytajZmianyZPliku("out/grafik_test.csv");
        //tydzienPracy->wypiszGrafik();
        tydzienPracy->przydziel_zmiany();
        tydzienPracy->zapiszZmianyDoPliku("out/grafik_test.csv");
        tydzienPracy->zatwierdzZmiany("out/miesiac_test.csv");
    } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
    }
}
