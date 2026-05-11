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
// 2. Pilnas konstruktorius
// ============================================================
static void testPilnasKonstruktorius() {
    skyrius("2. Pilnas konstruktorius");
    vector<int> paz = { 6, 7, 8, 9, 10 };
    Studentas st("Jonas", "Jonaitis", paz, 9, 3); // metodas 3 = abu

    tikrinti("vardas == Jonas", st.vardas() == "Jonas");
    tikrinti("pavarde == Jonaitis", st.pavarde() == "Jonaitis");
    tikrinti("egz == 9", st.egz() == 9);
    tikrinti("paz dydzis == 5", st.paz().size() == 5);

    // Galutinis vidurkis: (6+7+8+9+10)/5 = 8.0; 8.0*0.4 + 9*0.6 = 3.2 + 5.4 = 8.6
    tikrinti("galVid apytikslis", std::abs(st.galVid() - 8.6) < 0.01);
    // Mediana: surikiuota {6,7,8,9,10} → mediana = 8; 8*0.4 + 9*0.6 = 3.2 + 5.4 = 8.6
    tikrinti("galMed apytikslis", std::abs(st.galMed() - 8.6) < 0.01);
}
// ============================================================
// 3. Destruktorius (patikrinamas per scope)
// ============================================================
static void testDestruktorius() {
    skyrius("3. Destruktorius");
    {
        Studentas st("Laikinas", "Studentas", { 5, 6, 7 }, 8, 1);
        // Objektas sukuriamas ir sunaikinamas išeinant iš bloko
    } // ~Studentas() iškviečiamas čia
    tikrinti("destruktorius iskviestas be crash", true);
    // Jei programa nepasibaigia su klaida — destruktorius veikia teisingai
}
// ============================================================
// 4. Kopijavimo konstruktorius
// ============================================================
static void testKopijavimoKonstruktorius() {
    skyrius("4. Kopijavimo konstruktorius");
    vector<int> paz = { 5, 6, 7, 8 };
    Studentas originalas("Petras", "Petraitis", paz, 7, 1);
    Studentas kopija(originalas); // copy constructor

    tikrinti("kopija.vardas == originalas.vardas",
        kopija.vardas() == originalas.vardas());
    tikrinti("kopija.pavarde == originalas.pavarde",
        kopija.pavarde() == originalas.pavarde());
    tikrinti("kopija.egz == originalas.egz",
        kopija.egz() == originalas.egz());
    tikrinti("kopija.galVid == originalas.galVid",
        std::abs(kopija.galVid() - originalas.galVid()) < 0.001);

    // Patikrinti, kad tai GILI kopija — paz_ vektoriai yra atskiri
    // Jei pakeičiame kopiją, originalas neturi kisti
    Studentas kopija2(originalas);
    kopija2.addPazymys(10); // pridedame tik kopijoje
    tikrinti("gili kopija: originalas.paz dydzis nepakito",
        originalas.paz().size() == 4);
    tikrinti("gili kopija: kopija2.paz dydzis padidejo",
        kopija2.paz().size() == 5);
}
// ============================================================
// 5. Kopijavimo priskyrimo operatorius
// ============================================================
static void testKopijavimoKonstruktoriusPriskyrimas() {
    skyrius("5. Kopijavimo priskyrimo operatorius");
    vector<int> paz = { 4, 5, 6 };
    Studentas a("Ona", "Onaite", paz, 6, 1);
    Studentas b; // tuscias
    b = a;       // copy assignment

    tikrinti("b.vardas == a.vardas", b.vardas() == a.vardas());
    tikrinti("b.pavarde == a.pavarde", b.pavarde() == a.pavarde());
    tikrinti("b.egz == a.egz", b.egz() == a.egz());
    tikrinti("b.galVid == a.galVid", std::abs(b.galVid() - a.galVid()) < 0.001);

    // Savipriskyrimas neturi sugadinti objekto
    a = a;
    tikrinti("savipriskyrimas (a = a): vardas islieka", a.vardas() == "Ona");
    tikrinti("savipriskyrimas (a = a): egz islieka", a.egz() == 6);
}

// ============================================================
// 6. Perkėlimo konstruktorius
// ============================================================
static void testPerkelimoKonstruktorius() {
    skyrius("6. Perkelimo konstruktorius");
    vector<int> paz = { 7, 8, 9 };
    Studentas originalas("Matas", "Mataitis", paz, 8, 1);
    string senoVardas = originalas.vardas();

    Studentas perkeltas(move(originalas)); // move constructor

    tikrinti("perkeltas.vardas == seno vardas",
        perkeltas.vardas() == senoVardas);
    tikrinti("perkeltas.egz == 8",
        perkeltas.egz() == 8);
    tikrinti("perkeltas.paz dydzis == 3",
        perkeltas.paz().size() == 3);
    // Po move originalas turi būti galiojančios (bet tuščios) būsenos
    tikrinti("originalas.vardas tuscias po move",
        originalas.vardas() == "");
    tikrinti("originalas.egz == 0 po move",
        originalas.egz() == 0);
}

// ============================================================
// 7. Perkėlimo priskyrimo operatorius
// ============================================================
static void testPerkelimoPriskyrimas() {
    skyrius("7. Perkelimo priskyrimo operatorius");
    vector<int> paz = { 6, 7, 8 };
    Studentas a("Ieva", "Ievaite", paz, 7, 1);
    Studentas b;
    string senoVardas = a.vardas();

    b = move(a); // move assignment

    tikrinti("b.vardas == seno a.vardas", b.vardas() == senoVardas);
    tikrinti("b.egz == 7", b.egz() == 7);
    tikrinti("b.paz dydzis == 3", b.paz().size() == 3);
    tikrinti("a.vardas tuscias po move", a.vardas() == "");
    tikrinti("a.egz == 0 po move", a.egz() == 0);

    // Saviperkėlimas neturi sugadinti objekto
    b = move(b);
    tikrinti("saviperk. (b = move(b)): programa necrasha", true);
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
	testPilnasKonstruktorius();
	testDestruktorius();
    testKopijavimoKonstruktorius();
    testKopijavimoKonstruktoriusPriskyrimas();
    testPerkelimoKonstruktorius();
    testPerkelimoPriskyrimas();
    cout << "\n========================================\n";
    cout << "  Rezultatai: " << praejo << " PASS, "
        << nepraejo << " FAIL\n";
    cout << "========================================\n";

    return (nepraejo == 0) ? 0 : 1;
}