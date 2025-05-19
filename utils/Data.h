#ifndef DATA_H
#define DATA_H

#include <iostream>

struct Data {
    int dzien;
    int miesiac;
    int rok;

    Data();
    Data(int d, int m, int r);

    bool rok_przestepny() const;

    void nastepny_dzien();

    friend std::ostream& operator<<(std::ostream& out, const Data& d);

    friend std::istream& operator>>(std::istream& in, Data& d);
};

bool operator==(const std::string& s_data, const Data& d);

#endif // DATA_H