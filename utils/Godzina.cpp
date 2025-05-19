#include "Godzina.h"

#include <iomanip>
#include <sstream>

Godzina::Godzina(): godzina(0), minuta(0) {}
Godzina::Godzina(int godzina, int minuta) : godzina(godzina), minuta(minuta) {}

std::istream& operator>>(std::istream& in, Godzina& g){
    std::string czas;
    in >> czas;
    std::stringstream ss(czas);
    char s;
    ss >> g.godzina >> s >> g.minuta;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Godzina& g){
    out << std::setfill('0') << std::setw(2) << g.godzina << ":" << std::setfill('0') << std::setw(2) << g.minuta;
    return out;
}

int Godzina::toMinuty() const {
    return godzina * 60 + minuta;
}

Godzina operator-(const Godzina& g1, const Godzina& g2){
        int min1 = g1.godzina * 60 + g1.minuta;
        int min2 = g2.godzina * 60 + g2.minuta;
        int diff = min1 - min2;
        if (diff < 0) diff += 24 * 60;
        return Godzina(diff / 60, diff % 60);
}