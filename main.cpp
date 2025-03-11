#include <iostream>
#include <string>
using namespace std;

struct Pracownik{
    Pracownik(string imie, string nazwisko, int stanowisko, int il_minut) :
        imie(imie), nazwisko(nazwisko), id(-1), stanowisko(stanowisko), il_minut(il_minut){}
    string imie;
    string nazwisko;
    int id;
    int stanowisko; //0 - szatniarz, 1 - programowy, 2 - bufetowy
    int il_minut;

    void wypisz(){
        cout << imie << " " << nazwisko << ", ";
        if(stanowisko == 0) cout << "Szatniarz, ";
        else if(stanowisko == 1) cout << "Programowy, ";
        else cout << "Bufetowy, ";
        cout << "Ilosc przepracowanych minut: " << il_minut << endl;
    }

    void wypisz_krotko(){
        cout << imie << " " << nazwisko << ", ";
    }
};

struct Teatr{
    Teatr(int il_pracownikow): il_pracownikow(il_pracownikow) {
        pracownicy = new Pracownik*[il_pracownikow];
        for(int i=0; i<il_pracownikow; ++i){
            pracownicy[i] = nullptr;
        }
    }

    ~Teatr(){
        delete[] pracownicy;
    }

    int il_pracownikow;
    Pracownik** pracownicy;

    void dodaj_pracownika(Pracownik* pracownik, int miejsce){
        if(pracownik == nullptr || miejsce >= il_pracownikow) return;
        pracownicy[miejsce] = pracownik;
        pracownik->id = miejsce;
    }

    void zwolnij_pracownika(int identyfikator){
        if(identyfikator >= il_pracownikow) return;
        pracownicy[identyfikator] = nullptr;
    }

    void wypisz(){
        cout << "Pracownicy Teatru Starego: " << endl;
        for(int i=0; i<il_pracownikow; ++i){
            if(pracownicy[i] != nullptr) pracownicy[i]->wypisz();
        }
    }
};

int main(){
    cout.precision(7);
    Pracownik p1("Jan", "Kowalski", 1, 0);
    Pracownik p2("Janina", "Kowalska", 2, 0);
    Pracownik p3("Jakub", "Nowak", 0, 0);
    Pracownik p4("Mateusz", "Nowakowski", 0, 0);
    Pracownik p5("Aleksandra", "Kowal", 0, 0);

    Teatr teatrStary(5);
    teatrStary.dodaj_pracownika(&p1,0);
    teatrStary.dodaj_pracownika(&p2,1);
    teatrStary.dodaj_pracownika(&p3,2);
    teatrStary.dodaj_pracownika(&p4,3);
    teatrStary.dodaj_pracownika(&p5,4);

    teatrStary.wypisz();

    teatrStary.zwolnij_pracownika(3);

    cout << endl;
    teatrStary.wypisz();
}
