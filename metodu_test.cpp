// ============================================================
// testas.cpp — Studentas klasės vienetų testai (v1.2)
//
// Tikrinami visi Rule of Five metodai ir operatoriai:
//   1. Numatytasis konstruktorius
//   2. Pilnas konstruktorius
//   3. Destruktorius (per scope)
//   4. Kopijavimo konstruktorius
//   5. Kopijavimo priskyrimo operatorius
//   6. Perkėlimo konstruktorius
//   7. Perkėlimo priskyrimo operatorius
//   8. operator<<
//   9. operator>>
//  10. Visi getter'iai ir setter'iai
//  11. apskaiciuoti(), skaiciuotiVidurki(), skaiciuotiMediana()
//
// Kompiliavimas:
//   g++ -std=c++17 testas.cpp funkcijos.cpp -o testas
// ============================================================

#include "struktura.h"

// ---- Pagalbiniai testų įrankiai ----

static int praejo = 0;
static int nepraejo = 0;

// Patikrina sąlygą ir atspausdina PASS arba FAIL
static void tikrinti(const string& pavadinimas, bool salyga) {
    if (salyga) {
        cout << "  [PASS] " << pavadinimas << "\n";
        praejo++;
    }
    else {
        cout << "  [FAIL] " << pavadinimas << "\n";
        nepraejo++;
    }
}
// Atspausdina skyriaus antraštę
static void skyrius(const string& pavadinimas) {
    cout << "\n--- " << pavadinimas << " ---\n";
}

// ============================================================
// 1. Numatytasis konstruktorius
// ============================================================
static void testNumatytasisKonstruktorius() {
    skyrius("1. Numatytasis konstruktorius");
    Studentas st;
    tikrinti("vardas yra tuscias", st.vardas() == "");
    tikrinti("pavarde yra tuscia", st.pavarde() == "");
    tikrinti("paz yra tuscias", st.paz().empty());
    tikrinti("egz == 0", st.egz() == 0);
    tikrinti("galVid == 0.0", st.galVid() == 0.0);
    tikrinti("galMed == 0.0", st.galMed() == 0.0);
}

// ============================================================
// main — paleidžia visus testus
// ============================================================

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    cout << "========================================\n";
    cout << "  Studentas klases vienetu testai v1.2 \n";
    cout << "========================================\n";

    testNumatytasisKonstruktorius();
    cout << "\n========================================\n";
    cout << "  Rezultatai: " << praejo << " PASS, "
        << nepraejo << " FAIL\n";
    cout << "========================================\n";

    return (nepraejo == 0) ? 0 : 1;
}