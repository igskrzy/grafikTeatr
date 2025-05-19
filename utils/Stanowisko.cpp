#include "Stanowisko.h"

std::ostream& operator<<(std::ostream& out, const Stanowisko& s) {
    switch (s) {
        case Stanowisko::SZATNIARZ: out << "SZATNIARZ"; break;
        case Stanowisko::PROGRAMOWY: out << "PROGRAMOWY"; break;
        case Stanowisko::BUFETOWY: out << "BUFETOWY"; break;
    }
    return out;
}

std::istream& operator>>(std::istream& in, Stanowisko& s) {
    std::string str;
    in >> str;
    if (str == "SZATNIARZ") s = Stanowisko::SZATNIARZ;
    else if (str == "PROGRAMOWY") s = Stanowisko::PROGRAMOWY;
    else if (str == "BUFETOWY") s = Stanowisko::BUFETOWY;
    return in;
}