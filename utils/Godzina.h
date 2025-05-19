#ifndef GODZINA_H
#define GODZINA_H

#include <iostream>

struct Godzina{
    int godzina;
    int minuta;
    
    Godzina();
    Godzina(int godzina, int minuta);

    friend std::istream& operator>>(std::istream& in, Godzina& g);

    friend std::ostream& operator<<(std::ostream& out, const Godzina& g);

    int toMinuty() const;
};

Godzina operator-(const Godzina& g1, const Godzina& g2);

#endif // GODZINA_H