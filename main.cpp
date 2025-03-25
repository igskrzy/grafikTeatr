#include <iostream>
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
    Teatr* teatr;
    double pierwszy_dzien;
    Spektakl* dzien[7][3];

    Tydzien(Teatr* teatr, double pierwszy_dzien) : teatr(teatr), pierwszy_dzien(pierwszy_dzien) {
         for(int i = 0; i < 7; ++i) {
            for(int j = 0; j < 3; ++j) {
                dzien[i][j] = nullptr;
            }
        }
    }

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

};

int main(){
    Pracownik p1("Antoni", "Skrzyniarz", PROGRAMOWY, 0);
    Pracownik p2("Weronika", "Nagawiecka", PROGRAMOWY, 450);
    Pracownik p3("Szymon", "Kuczek", PROGRAMOWY, 605);
    Pracownik p4("Paulina", "Filimon", PROGRAMOWY, 520);
    Pracownik p5("Julia", "Pietron", BUFETOWY, 480);
    Pracownik p6("Aleksandra", "Sek", BUFETOWY, 295);
    Pracownik p7("Aleksandra", "Guzy", BUFETOWY, 300);
    Pracownik p8("Paulina", "Sokolowska", BUFETOWY, 410);
    Pracownik p9("Julia", "Ciembka", BUFETOWY, 380);
    Pracownik p10("Ignacy", "Skrzyniarz", SZATNIARZ, 390);
    Pracownik p11("Olga", "Wiecek", SZATNIARZ, 470);
    Pracownik p12("Iga", "Kozlowska", SZATNIARZ, 410);
    Pracownik p13("Zofia", "Watroba", SZATNIARZ, 450);
    Pracownik p14("Barbara", "Stojek", SZATNIARZ, 700);
    Pracownik p15("Dominika", "Piatek", SZATNIARZ, 540);
    Pracownik p16("Szczepan", "Lompe", SZATNIARZ, 290);
    Pracownik p17("Katarzyna", "Pikul", SZATNIARZ, 290);
    Pracownik p18("Laura", "Kamasz", SZATNIARZ, 370);
    Pracownik p19("Lilianna", "Noga", SZATNIARZ, 320);
    Pracownik p20("Celina", "Wojtanowska", SZATNIARZ, 370);
    Pracownik p21("Jakub", "Drewnicki", SZATNIARZ, 270);
    Pracownik p22("Julia", "Krawczyk", SZATNIARZ, 270);
    Pracownik p23("Maja", "Duszak", SZATNIARZ, 410);
    Pracownik p24("Patryk", "Kozlowski", SZATNIARZ, 350);
    Pracownik p25("Tosia", "Malecka", SZATNIARZ, 550);
    Pracownik p26("Zuzanna", "Adamowicz", SZATNIARZ, 280);

    Teatr teatrStary(26);
    teatrStary.dodaj_pracownika(&p1, 0);
    teatrStary.dodaj_pracownika(&p2, 1);
    teatrStary.dodaj_pracownika(&p3, 2);
    teatrStary.dodaj_pracownika(&p4, 3);
    teatrStary.dodaj_pracownika(&p5, 4);
    teatrStary.dodaj_pracownika(&p6, 5);
    teatrStary.dodaj_pracownika(&p7, 6);
    teatrStary.dodaj_pracownika(&p8, 7);
    teatrStary.dodaj_pracownika(&p9, 8);
    teatrStary.dodaj_pracownika(&p10, 9);
    teatrStary.dodaj_pracownika(&p11, 10);
    teatrStary.dodaj_pracownika(&p12, 11);
    teatrStary.dodaj_pracownika(&p13, 12);
    teatrStary.dodaj_pracownika(&p14, 13);
    teatrStary.dodaj_pracownika(&p15, 14);
    teatrStary.dodaj_pracownika(&p16, 15);
    teatrStary.dodaj_pracownika(&p17, 16);
    teatrStary.dodaj_pracownika(&p18, 17);
    teatrStary.dodaj_pracownika(&p19, 18);
    teatrStary.dodaj_pracownika(&p20, 19);
    teatrStary.dodaj_pracownika(&p21, 20);
    teatrStary.dodaj_pracownika(&p22, 21);
    teatrStary.dodaj_pracownika(&p23, 22);
    teatrStary.dodaj_pracownika(&p24, 23);
    teatrStary.dodaj_pracownika(&p25, 24);
    teatrStary.dodaj_pracownika(&p26, 25);

    teatrStary.wypisz();
    Tydzien t1(&teatrStary, 2.40325);
    t1.wpisz_spektakle();
    t1.wypisz_grafik();
}
