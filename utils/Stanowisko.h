#ifndef STANOWISKO_H
#define STANOWISKO_H

#include <iostream>

enum class Stanowisko { SZATNIARZ, PROGRAMOWY, BUFETOWY };

std::ostream& operator<<(std::ostream& out, const Stanowisko& s);
std::istream& operator>>(std::istream& in, Stanowisko& s);

#endif // STANOWISKO_H