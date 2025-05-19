#include "Data.h"

#include <iomanip>
#include <sstream>

Data::Data() : dzien(1), miesiac(1), rok(2000) {}
Data::Data(int d, int m, int r) : dzien(d), miesiac(m), rok(r) {}

bool Data::rok_przestepny() const{
    return (rok % 4 == 0 && (rok % 100 != 0 || rok % 400 == 0));
}

void Data::nastepny_dzien() {
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

std::ostream& operator<<(std::ostream& out, const Data& d) {
    out << std::setfill('0') << std::setw(2) << d.dzien << "."
        << std::setw(2) << d.miesiac << "."
        << std::setw(4) << d.rok;
    return out;
}

std::istream& operator>>(std::istream& in, Data& d) {
    char delimiter;
    in >> d.dzien >> delimiter >> d.miesiac >> delimiter >> d.rok;
    return in;
}

bool operator==(const std::string& s_data, const Data& d) {
    std::istringstream ss(s_data);
    int dzien, miesiac, rok;
    char dot1, dot2;

    ss >> dzien >> dot1 >> miesiac >> dot2 >> rok;

    if (ss.fail() || dot1 != '.' || dot2 != '.') {
        return false;
    }

    return (d.dzien == dzien && d.miesiac == miesiac && d.rok == rok);
}