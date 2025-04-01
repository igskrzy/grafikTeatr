#include <iostream>
#include <fstream>
using namespace std;

enum Stanowisko { SZATNIARZ, PROGRAMOWY, BUFETOWY };
struct Pracownik{
    Pracownik(string imie, string nazwisko, Stanowisko stanowisko, int il_minut) :  imie(imie), nazwisko(nazwisko), id(-1), stanowisko(stanowisko), il_minut(il_minut){}
    string imie;
    string nazwisko;
    int id;
    Stanowisko stanowisko;
    int il_minut;

    void wypisz(){
        cout << imie << " " << nazwisko << ", ";
        if(stanowisko == SZATNIARZ) cout << "Szatniarz, ";
        else if(stanowisko == PROGRAMOWY) cout << "Programowy, ";
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

    void zapiszDoPliku(const char *nazwaPliku) {
        ofstream plik(nazwaPliku);
        if (!plik) {
            cerr << "Blad otwierania pliku" << endl;
            return;
        }

        plik << il_pracownikow << "\n";
        for(int i=0; i<il_pracownikow; ++i){
            if (pracownicy[i] != nullptr) {
                plik << pracownicy[i]->imie << " "
                     << pracownicy[i]->nazwisko << " "
                     << pracownicy[i]->stanowisko << " "
                     << pracownicy[i]->il_minut << "\n";
            }
        }

    }

    static Teatr* wczytajZPliku(const char *nazwaPliku){
        ifstream plik(nazwaPliku);
        if (!plik) {
                cerr << "Blad otwierania pliku" << endl;
                return nullptr;
            }
        int il_p = 0;
        plik >> il_p;
        plik.ignore();
        Teatr* teatr = new Teatr(il_p);
        for(int i=0; i<il_p; ++i){
            string imie, nazwisko;
            int il_minut;
            string stanowisko;
            plik >> imie >> nazwisko >> stanowisko >> il_minut;
            plik.ignore();
            Stanowisko s;
            if(stanowisko == "SZATNIARZ") s = Stanowisko::SZATNIARZ;
            else if(stanowisko == "PROGRAMOWY") s = Stanowisko::PROGRAMOWY;
            else if(stanowisko == "BUFETOWY") s = Stanowisko::BUFETOWY;
            teatr->pracownicy[i] = new Pracownik(imie, nazwisko, s, il_minut);
        }
        return teatr;
    }
};

struct Spektakl{
    Spektakl() : nazwa("brak"), data(0), scena("brak"), il_pracownikow(0), czas_trwania(0), godzina_rozpoczeczia("brak") {}
    Spektakl(string nazwa, double data, string scena, int il_pracownikow, bool bufetowy_potrzebny, int czas_trwania, string godzina_rozpoczeczia)
        : nazwa(nazwa), data(data), scena(scena), il_pracownikow(il_pracownikow), il_pracujacych(0), programowy(false), bufetowy(false), bufetowy_potrzebny(true), czas_trwania(czas_trwania), godzina_rozpoczeczia(godzina_rozpoczeczia) {
        pracownicy = new Pracownik*[il_pracownikow];
        for(int i=0; i<il_pracownikow; ++i){
            pracownicy[i] = nullptr;
        }
    }

    ~Spektakl(){
        delete[] pracownicy;
    }

    string nazwa;
    double data; //D.DMMYYYY
    string scena;
    int il_pracownikow;
    Pracownik** pracownicy;
    int il_pracujacych;
    bool programowy;
    bool bufetowy;
    bool bufetowy_potrzebny;
    int czas_trwania; //w minutach
    string godzina_rozpoczeczia; //HH:MM

    static Spektakl* stworz(){
        Spektakl* s = new Spektakl();
        cout << "Nazwa spektaklu: ";
        getline(cin >> ws, s->nazwa);
        cout << "Godzina rozpoczecia: ";
        cin >> s->godzina_rozpoczeczia;
        cout << "Czas trwania: (w minutach): ";
        cin >> s->czas_trwania;
        cout << "Scena: ";
        cin >> s->scena;
        cout << "Ilosc pracownikow na zmianie: ";
        cin >> s->il_pracownikow;
        cout << "Czy bufetowy potrzebny?: ";
        cin >> s->bufetowy_potrzebny;
        if (s->il_pracownikow > 0) {
            s->pracownicy = new Pracownik*[s->il_pracownikow];
            for (int i = 0; i < s->il_pracownikow; ++i) {
                s->pracownicy[i] = nullptr;
            }
        } else s->pracownicy = nullptr;

        return s;
    }

    void wypisz(){
        cout << "Spektakl: \"" << nazwa << "\"" << endl;
        cout << "Data: " << data << endl;
        cout << "Scena: " << scena << endl;
        cout << endl;
        cout << "Czas trwania: " << czas_trwania << "min" << endl;
        cout << "Godzina rozpoczecia: " << godzina_rozpoczeczia << endl;
        cout << "Pracownicy: " << endl;
        for(int i=0; i<il_pracownikow; ++i){
            if(pracownicy[i]!=nullptr)pracownicy[i]->wypisz_krotko();
            else cout << "!!!BRAK!!!, ";
        }
        cout << endl;
    }

    void wypisz_krotko(){
        cout << data << " " << nazwa << " " << scena << endl;
    }

    void wypisz_krotko_z_pracownikami(){
        cout << data << " " << nazwa << " " << scena << endl;
        for(int i=0; i<il_pracownikow; ++i){
            if(pracownicy[i]!=nullptr){
                pracownicy[i]->wypisz_krotko();
            }
            else cout << "!!!BRAK!!!, ";
        }
        cout << endl;
    }

    void dodaj_pracownika(Pracownik* pracownik, int miejsce){
        if(pracownik == nullptr || miejsce >= il_pracownikow) return;
        pracownicy[miejsce] = pracownik;
    }

};

struct Tydzien{
    struct Dyspo{
        Dyspo(Pracownik* pracownik, Tydzien* tydzien) : pracownik(pracownik), tydzien(tydzien) {}
        Pracownik* pracownik;
        Tydzien* tydzien;
        bool zmiany[7][3] = {false};

        void wpisz_dyspo(){
            cout << "Dyspo dla: ";
            pracownik->wypisz_krotko();
            for(int i=0; i<7; ++i){
                for(int j=0; j<3; ++j){
                    if(tydzien->dzien[i][j] != nullptr){
                        tydzien->dzien[i][j]->wypisz_krotko();
                        cin >> zmiany[i][j];
                    }
                }
            }
            cout << endl;
            tydzien->dyspo[pracownik->id] = this;
        }
    };

    Teatr* teatr;
    double pierwszy_dzien;
    Spektakl* dzien[7][3];
    Dyspo** dyspo;

    Tydzien(Teatr* teatr, double pierwszy_dzien) : teatr(teatr), pierwszy_dzien(pierwszy_dzien) {
        dyspo = new Dyspo*[teatr->il_pracownikow];
        for(int i=0; i<teatr->il_pracownikow; ++i){
            dyspo[i] = new Dyspo(teatr->pracownicy[i], this);
        }

         for(int i = 0; i < 7; ++i) {
            for(int j = 0; j < 3; ++j) {
                dzien[i][j] = nullptr;
            }
        }
    }

//    ~Tydzien() {
//        for(int i = 0; i < teatr->il_pracownikow; ++i) {
//            if(dyspo[i] != nullptr)delete dyspo[i];
//        }
//        delete[] dyspo;
//    }

    void wpisz_spektakle(){
        double data = pierwszy_dzien;
        for(int i=0; i<7; ++i){
            int il = 0;
            cout << "Liczba spektakli dnia " << data << ": ";
            cin >> il;
            for(int j=0; j<il; ++j){
                Spektakl* s = Spektakl::stworz();
                s->data = data;
                dzien[i][j] = s;
            }
            data += 0.1;
        }
    }

    void wpisz_dyspo(){
        for(int i=0; i<teatr->il_pracownikow; ++i){
            dyspo[i]->wpisz_dyspo();
        }
    }

    void wypisz_grafik(){
        for(int i=0; i<7; ++i){
            for(int j=0; j<3; ++j){
                if(dzien[i][j] != nullptr){
                    dzien[i][j]->wypisz_krotko_z_pracownikami();
                }
            }
        }
        cout << endl;
    }

    static Tydzien* wczytajZPliku(const char *nazwaPliku, Teatr* teatr){
        ifstream plik(nazwaPliku);
        if (!plik) {
                cerr << "Blad otwierania pliku" << endl;
                return nullptr;
        }

        double data;
        plik >> data;
        plik.ignore();
        Tydzien* tydzien = new Tydzien(teatr, data);

        for(int i=0; i<7; ++i){
            int il = 0;
            plik >> il;
            plik.ignore();
            for(int j=0; j<il && j<3; ++j){
                tydzien->dzien[i][j] = new Spektakl();
                getline(plik, tydzien->dzien[i][j]->nazwa);
                plik >> tydzien->dzien[i][j]->godzina_rozpoczeczia
                >> tydzien->dzien[i][j]->czas_trwania
                >> tydzien->dzien[i][j]->scena
                >> tydzien->dzien[i][j]->il_pracownikow
                >> tydzien->dzien[i][j]->bufetowy_potrzebny;
                plik.ignore();
                tydzien->dzien[i][j]->data = data;
                if (tydzien->dzien[i][j]->il_pracownikow > 0){
                    tydzien->dzien[i][j]->pracownicy = new Pracownik*[tydzien->dzien[i][j]->il_pracownikow];
                    for (int k = 0; k < tydzien->dzien[i][j]->il_pracownikow; ++k) {
                        tydzien->dzien[i][j]->pracownicy[k] = nullptr;
                    }
                } else tydzien->dzien[i][j]->pracownicy = nullptr;
            }
            data += 0.1;
        }

    return tydzien;
    }

};

int main(){
    Teatr* teatrStary = Teatr::wczytajZPliku("teatrStary.txt");
    teatrStary->wypisz();
    Tydzien* t1 = Tydzien::wczytajZPliku("tydzien.txt", teatrStary);
    t1->wypisz_grafik();
}
