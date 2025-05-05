#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

enum class Stanowisko { SZATNIARZ, PROGRAMOWY, BUFETOWY };

ostream& operator<<(ostream& out, const Stanowisko& s) {
    switch (s){
        case Stanowisko::SZATNIARZ: out << "SZATNIARZ"; break;
        case Stanowisko::PROGRAMOWY: out << "PROGRAMOWY"; break;
        case Stanowisko::BUFETOWY: out << "BUFETOWY"; break;
    }
    return out;
}

istream& operator>>(istream& in, Stanowisko& s) {
    string str;
    in >> str;
    if (str == "SZATNIARZ") s = Stanowisko::SZATNIARZ;
    else if (str == "PROGRAMOWY") s = Stanowisko::PROGRAMOWY;
    else if (str == "BUFETOWY") s = Stanowisko::BUFETOWY;
    return in;
}

struct Godzina{
    int godzina;
    int minuta;
    Godzina(): godzina(0), minuta(0) {}
    Godzina(int godzina, int minuta) : godzina(godzina), minuta(minuta) {}

    friend istream& operator>>(istream& in, Godzina& g){
        string czas;
        in >> czas;
        stringstream ss(czas);
        char s;
        ss >> g.godzina >> s >> g.minuta;
        return in;
    }

    friend ostream& operator<<(ostream& out, const Godzina& g){
        out << setfill('0') << setw(2) << g.godzina << ":"
            << setfill('0') << setw(2) << g.minuta;
        return out;
    }

    int toMinuty() const {
        return godzina * 60 + minuta;
    }
};

Godzina operator-(const Godzina& g1, const Godzina& g2){
        int min1 = g1.godzina * 60 + g1.minuta;
        int min2 = g2.godzina * 60 + g2.minuta;
        int diff = min1 - min2;
        if (diff < 0) diff += 24 * 60;
        return Godzina(diff / 60, diff % 60);
    }

struct Data {
    int dzien;
    int miesiac;
    int rok;

    Data() : dzien(1), miesiac(1), rok(2000) {}
    Data(int d, int m, int r) : dzien(d), miesiac(m), rok(r) {}

    bool rok_przestepny() const {
        return (rok % 4 == 0 && (rok % 100 != 0 || rok % 400 == 0));
    }

    void nastepny_dzien() {
        dzien++;
        bool miesiac31 = (miesiac == 1 || miesiac == 3 || miesiac == 5 || miesiac == 7 || miesiac == 8 || miesiac == 10 || miesiac == 12);
        bool miesiac30 = (miesiac == 4 || miesiac == 6 || miesiac == 9 || miesiac == 11);
        bool luty = (miesiac == 2);
        if (miesiac31 && dzien > 31) {
            dzien = 1;
            miesiac++;
        } else if (miesiac30 && dzien > 30) {
            dzien = 1;
            miesiac++;
        } else if (luty && dzien > (rok_przestepny() ? 29 : 28)) {
            dzien = 1;
            miesiac++;
        }
        if (miesiac > 12) {
            miesiac = 1;
            rok++;
        }
    }
};

ostream& operator<<(ostream& out, const Data& d) {
    out << setfill('0') << setw(2) << d.dzien << "."
        << setw(2) << d.miesiac << "."
        << setw(4) << d.rok;
    return out;
}

istream& operator>>(istream& in, Data& d) {
    char delimiter;
    in >> d.dzien >> delimiter >> d.miesiac >> delimiter >> d.rok;
    return in;
}

struct Pracownik{
    Pracownik(int id, string imie, string nazwisko, Stanowisko stanowisko)
    : id(id), imie(imie), nazwisko(nazwisko), stanowisko(stanowisko){}
    int id;
    string imie;
    string nazwisko;
    Stanowisko stanowisko;

    void wypisz(){
        cout << id << ". " << imie << " " << nazwisko << ", " << stanowisko << endl;
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


    void wypisz(){
        cout << "Pracownicy Teatru Starego: " << endl;
        for(int i=0; i<il_pracownikow; ++i){
            if(pracownicy[i] != nullptr) pracownicy[i]->wypisz();
        }
    }

     static Teatr* wczytajZPliku(const char *nazwaPliku){
        ifstream plik(nazwaPliku);
        if (!plik) {
                cerr << "Blad otwierania pliku teatr" << endl;
                return nullptr;
            }
        int il_pracownikow = 0;
        plik >> il_pracownikow;
        plik.ignore();
        Teatr* teatr = new Teatr(il_pracownikow);
        for(int i=0; i<il_pracownikow; ++i){
            int id;
            string imie, nazwisko;
            Stanowisko stanowisko;
            plik >> id >> imie >> nazwisko >> stanowisko;
            plik.ignore();
            teatr->pracownicy[i] = new Pracownik(id, imie, nazwisko, stanowisko);
        }
        return teatr;
    }

};

struct Spektakl{
    Spektakl() : nazwa("brak"), data(Data()), scena("brak"), il_pracownikow(0), czas_trwania(0), godzina_rozpoczeczia(Godzina()) {}
    Spektakl(string nazwa, Data data, string scena, int il_pracownikow, bool bufetowy_potrzebny, int czas_trwania, Godzina godzina_rozpoczeczia)
        : nazwa(nazwa), data(data), scena(scena), il_pracownikow(il_pracownikow),
        bufetowy_potrzebny(bufetowy_potrzebny), czas_trwania(czas_trwania), godzina_rozpoczeczia(godzina_rozpoczeczia) {}

    string nazwa;
    Data data;
    string scena;
    int il_pracownikow;
    bool bufetowy_potrzebny;
    int czas_trwania; //w minutach
    Godzina godzina_rozpoczeczia; //HH:MM
    void wypisz(){
        cout << "========================" << endl;
        cout << "Spektakl: " << nazwa << endl;
        cout << "Data: " << data << endl;
        cout << "Scena: " << scena << endl;
        cout << "Czas trwania: " << czas_trwania << "min" << endl;
        cout << "Godzina rozpoczecia: " << godzina_rozpoczeczia << endl;
        cout << "Ilosc pracownikow: " << il_pracownikow << endl;
        cout << "Bufetowy potrzebny?: " << bufetowy_potrzebny << endl;
        cout << "========================" << endl;
    }
};

struct CzasPracy{
    CzasPracy(): pracownik(nullptr), czas(0) {}
    CzasPracy(Pracownik* pracownik,int czas_pracy): pracownik(pracownik), czas(czas) {}
    Pracownik* pracownik;
    int czas;
};

struct MiesiacPracy{
    MiesiacPracy(Teatr* teatr): miesiac_id(0), teatr(teatr){
        czas_pracy = new CzasPracy[teatr->il_pracownikow];
        for(int i=0; i<teatr->il_pracownikow; ++i){
            czas_pracy[i].pracownik = teatr->pracownicy[i];
            czas_pracy[i].czas = 0;
        }
    }

    ~MiesiacPracy(){
        delete[] czas_pracy;
    }

    void wczytajCzasPracyZPliku(const char *nazwaPliku){
        ifstream plik(nazwaPliku);
        if (!plik) {
            cerr << "Blad otwierania pliku miesiac" << endl;
            return;
        }
        int miesiac_id;
        plik >> miesiac_id;
        plik.ignore();
        for(int i=0; i<teatr->il_pracownikow; ++i){
            int id;
            plik >> id;
            if(id == teatr->pracownicy[i]->id)
                plik >> czas_pracy[i].czas;
            else{
                cerr << "Blad indeksowania pracownikow" << endl;
                return;
            }
            plik.ignore();
        }
    }

    int miesiac_id;
    Teatr* teatr;
    CzasPracy* czas_pracy;
};

struct Tydzien{
    int tydzien_id;
    Data pierwszy_dzien;
    Spektakl* dzien[7][3];

    Tydzien(int tydzien_id, Data pierwszy_dzien): tydzien_id(tydzien_id), pierwszy_dzien(pierwszy_dzien) {
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 3; ++j)
                dzien[i][j] = nullptr;
    }

    ~Tydzien(){
        for (int i = 0; i<7; ++i) {
            for (int j = 0; j<3; ++j) {
                delete dzien[i][j];
            }
        }
    }
    static Tydzien* wczytajZPliku(const char *nazwaPliku){
        ifstream plik(nazwaPliku);
        if (!plik) {
                cerr << "Blad otwierania pliku" << endl;
                return nullptr;
        }
        int tydzien_id;
        plik >> tydzien_id;
        plik.ignore();
        Data data;
        plik >> data;
        plik.ignore();
        Tydzien* tydzien = new Tydzien(tydzien_id, data);

        for(int i=0; i<7; ++i){
            int il = 0;
            plik >> il;
            plik.ignore();
            for(int j=0; j<3; ++j){
                if(j<il){
                    tydzien->dzien[i][j] = new Spektakl();
                    getline(plik, tydzien->dzien[i][j]->nazwa);
                    plik >> tydzien->dzien[i][j]->godzina_rozpoczeczia
                    >> tydzien->dzien[i][j]->czas_trwania
                    >> tydzien->dzien[i][j]->scena
                    >> tydzien->dzien[i][j]->il_pracownikow
                    >> tydzien->dzien[i][j]->bufetowy_potrzebny;
                    plik.ignore();
                    tydzien->dzien[i][j]->data = data;
                }
                else
                    tydzien->dzien[i][j] = nullptr;
            }
            data.nastepny_dzien();
        }

    return tydzien;
    }

    void wypisz(){
        for(int i=0; i<7; ++i){
            for(int j=0; j<3; ++j){
                if(dzien[i][j] != nullptr){
                    dzien[i][j]->wypisz();
                }
            }
        }
        cout << endl;
    }

};

struct Dyspo{
    Dyspo(): pracownik_id(-1), tydzien_id(-1) {}
    Dyspo(int pracownik_id, int tydzien_id) : pracownik_id(pracownik_id), tydzien_id(tydzien_id) {}
    int pracownik_id;
    int tydzien_id;
    bool zmiany[7] = {false};
};

Dyspo** wczytajDyspoZPliku(const char* nazwaPliku, int il_pracownikow){
    ifstream plik(nazwaPliku);
    if (!plik) {
        cerr << "Blad otwierania pliku dyspo" << endl;
        return nullptr;
    }
    Dyspo** dyspo = new Dyspo*[il_pracownikow];
    int tydzien_id;
    plik >> tydzien_id;
    plik.ignore();
    for(int k=0; k<il_pracownikow; ++k){
        dyspo[k] = new Dyspo();
        dyspo[k]->tydzien_id = tydzien_id;
        plik >> dyspo[k]->pracownik_id;
        for(int i=0; i<7; ++i){
            plik >> dyspo[k]->zmiany[i];
            cout << dyspo[k]->zmiany[i];
        }
        plik.ignore();
        cout << endl;
    }
    return dyspo;
}

struct TydzienPracy{
    Teatr* teatr;
    MiesiacPracy* miesiac;
    Tydzien* tydzien;
    Pracownik* pracujacy[7][3][6];
    int il_pracujacych[7][10];
    bool bufetowy[7][3];
    bool programowy[7][3];
    Dyspo** dyspo;
    CzasPracy* tymczasowy_czas_pracy;

    TydzienPracy(): teatr(nullptr), miesiac(nullptr), tydzien(nullptr), dyspo(nullptr), tymczasowy_czas_pracy(nullptr) {
        for(int i=0; i<7; ++i){
            for(int j=0; j<3; ++j){
                bufetowy[i][j] = false;
                programowy[i][j] = false;
                il_pracujacych[i][j] = 0;
                for(int k=0; k<6; ++k){
                    pracujacy[i][j][k] = nullptr;
                }
            }
        }
    }

    ~TydzienPracy(){
        delete teatr;
        delete miesiac;
        delete tydzien;
        if (dyspo != nullptr){
            for (int i = 0; i < teatr->il_pracownikow; ++i){
                delete dyspo[i];
            }
            delete[] dyspo;
        }
        delete[] tymczasowy_czas_pracy;
    }

    static TydzienPracy* zacznijTydzienPracy(const char* plikTeatr, const char* plikMiesiac, const char* plikTydzien, const char* plikDyspo){
        TydzienPracy* tydzienPracy = new TydzienPracy();
        Teatr* teatr = Teatr::wczytajZPliku(plikTeatr);
        tydzienPracy->teatr = teatr;
        MiesiacPracy* miesiac = new MiesiacPracy(teatr);
        miesiac->wczytajCzasPracyZPliku(plikMiesiac);
        tydzienPracy->miesiac = miesiac;
        tydzienPracy->tymczasowy_czas_pracy = new CzasPracy[teatr->il_pracownikow];
        for (int i = 0; i < teatr->il_pracownikow; ++i){
            tydzienPracy->tymczasowy_czas_pracy[i] = miesiac->czas_pracy[i];
        }
        Tydzien* tydzien = Tydzien::wczytajZPliku(plikTydzien);
        tydzienPracy->tydzien = tydzien;
        Dyspo** dyspo = wczytajDyspoZPliku(plikDyspo, teatr->il_pracownikow);
        tydzienPracy->dyspo = dyspo;
        if(tydzien->tydzien_id == dyspo[0]->tydzien_id)
            return tydzienPracy;
        else{
            cout << tydzien->tydzien_id << " " << dyspo[0]->tydzien_id << endl;
            cerr << "Blad: tydzien::tydzien_id != dyspo[0]::tydzien_id" << endl;
            return nullptr;
        }
    }

    void wypiszTeatr(){ teatr->wypisz(); }

    void wypiszTydzien(){ tydzien->wypisz(); }

    int znajdz_pracownika_min(int i, int j, Stanowisko stanowisko){
        int min_id = -1;
        for(int k=0; k<teatr->il_pracownikow; ++k){
            if((stanowisko == Stanowisko::SZATNIARZ || teatr->pracownicy[k]->stanowisko == stanowisko)
               && dyspo[k]->zmiany[i] && (min_id == -1 || tymczasowy_czas_pracy[k].czas < tymczasowy_czas_pracy[min_id].czas)
                && !czy_pracownik_pracuje_w_ten_dzien(teatr->pracownicy[k],i,j))
                min_id = k;
        }
        return min_id;
    }

    bool czy_pracownik_pracuje_w_ten_dzien(Pracownik* pracownik, int i, int j_s){
        bool zmiany[3] = {false, false, false};
        for(int j=0; j<3; ++j){
            if(tydzien->dzien[i][j]!=nullptr){
                for(int k=0; k<tydzien->dzien[i][j]->il_pracownikow; ++k){
                    if(pracownik == pracujacy[i][j][k]) zmiany[j] = true;
                }
            }
        }
        if(zmiany[j_s])return true;
        if(zmiany[0] && tydzien->dzien[i][0]->nazwa == "Salon Poezji" && !zmiany[1] && !zmiany[2])return false;
        else if(zmiany[0] || zmiany[1] || zmiany[2]) return true;
        return false;
    }

    void przydziel_zmiany(){
        tymczasowy_czas_pracy = miesiac->czas_pracy;
        for(int i=0; i<7; ++i){
            for(int j=0; j<3; ++j){
                if(tydzien->dzien[i][j] != nullptr){
                    il_pracujacych[i][j] = 0;
                    bufetowy[i][j] = 0;
                    programowy[i][j] = 0;
                    if(!programowy[i][j]){
                        int p_id = znajdz_pracownika_min(i,j,Stanowisko::PROGRAMOWY);
                        if(p_id!=-1){
                            tymczasowy_czas_pracy[p_id].czas += tydzien->dzien[i][j]->czas_trwania;
                            pracujacy[i][j][0] = teatr->pracownicy[p_id];
                            ++il_pracujacych[i][j];
                            programowy[i][j] = true;
                        }
                    }
                    if(!bufetowy[i][j] && tydzien->dzien[i][j]->bufetowy_potrzebny){
                        int p_id = znajdz_pracownika_min(i,j,Stanowisko::BUFETOWY);
                        if(p_id!=-1){
                            tymczasowy_czas_pracy[p_id].czas += tydzien->dzien[i][j]->czas_trwania;
                            pracujacy[i][j][1] = teatr->pracownicy[p_id];
                            ++il_pracujacych[i][j];
                            bufetowy[i][j] = true;
                        }
                    }
                }
            }
        }

        bool koniec = false;
        while(!koniec){
            koniec = true;
            for(int i=0; i<7; ++i){
                for(int j=0; j<3; ++j){
                    if(tydzien->dzien[i][j] != nullptr && il_pracujacych[i][j] < tydzien->dzien[i][j]->il_pracownikow){
                        int p_id = znajdz_pracownika_min(i,j,Stanowisko::SZATNIARZ);
                        if(p_id!=-1 && pracujacy[i][j][il_pracujacych[i][j]-1]->id != p_id){
                            koniec = false;
                            if(tydzien->dzien[i][j]!=nullptr && il_pracujacych[i][j] < tydzien->dzien[i][j]->il_pracownikow){
                                tymczasowy_czas_pracy[p_id].czas += tydzien->dzien[i][j]->czas_trwania;
                                pracujacy[i][j][il_pracujacych[i][j]] = teatr->pracownicy[p_id];
                                ++il_pracujacych[i][j];
                            }
                        }
                    }
                }
            }
        }
    }

    void wypiszGrafik(){
        for(int i=0; i<7; ++i){
            for(int j=0; j<3; ++j){
                if(tydzien->dzien[i][j]!=nullptr){
                    cout << tydzien->dzien[i][j]->nazwa << endl;
                    for(int k=0; k<6; ++k){
                        if(pracujacy[i][j][k] != nullptr)
                            pracujacy[i][j][k]->wypisz();
                    }
                }
            }
        }
    }
};

int main(){
    TydzienPracy* tydzienPracy = TydzienPracy::zacznijTydzienPracy("teatr.txt", "miesiac.txt", "tydzien.txt", "dyspo_test1.txt");
    tydzienPracy->wypiszTeatr();
    tydzienPracy->wypiszTydzien();
    tydzienPracy->przydziel_zmiany();
    tydzienPracy->wypiszGrafik();
}
