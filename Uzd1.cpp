//Uzd1.cpp
#include "struktura.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    try {
        vykdytiVector();
    }
    catch (const std::exception& e) {
        cerr << "Programos veikimo klaida: " << e.what() << endl;
        return 1;
    }
    return 0;
}
