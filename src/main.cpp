#include "TydzienPracy.h"

int main(){
    TydzienPracy* tydzienPracy = TydzienPracy::zacznijTydzienPracy("data/teatr_test.csv","data/miesiac_test.csv","data/tydzien_test.csv","data/dyspo_test2.csv");
    tydzienPracy->przydziel_zmiany();
    tydzienPracy->zapiszZmianyDoPliku("out/grafik_test.csv");
    //tydzienPracy->wczytajZmianyZPliku("out/grafik_test.csv");
    //tydzienPracy->wypiszGrafik();
    tydzienPracy->zatwierdzZmiany("out/miesiac_test.csv");
}
