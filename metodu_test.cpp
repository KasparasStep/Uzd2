// ============================================================
// metodu testas — Studentas/Zmogus klasių vienetų testai (v1.5)
//
// Tikrinami:
//   v1.5 NAUJI:
//     A. Zmogus negali būti instancijuotas (abstrakti klasė)
//     B. Studentas paveldi Zmogus
//     C. Virtualus dispatch per bazinę nuorodą
//     D. Polimorfizmas su konteineriu
//   v1.2 (iš ankstesnės versijos — vis dar veikia):
//     1.  Numatytasis konstruktorius
//     2.  Pilnas konstruktorius
//     3.  Destruktorius
//     4.  Kopijavimo konstruktorius
//     5.  Kopijavimo priskyrimo operatorius
//     6.  Perkėlimo konstruktorius
//     7.  Perkėlimo priskyrimo operatorius
//     8.  operator<<
//     9.  operator>>
//    10.  Getteriai ir setteriai
//    11.  Skaičiavimo metodai
//
// Kompiliavimas:
//   g++ -std=c++17 testas.cpp funkcijos.cpp -o testas
// ============================================================

#include "struktura.h"

// ---- Testų skaitiklis ----

static int praejo = 0;
static int nepraejo = 0;

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

static void skyrius(const string& pavadinimas) {
    cout << "\n--- " << pavadinimas << " ---\n";
}

// ============================================================
// v1.5 NAUJI TESTAI
// ============================================================

// ------------------------------------------------------------
// A. Zmogus yra abstrakti klasė
//
// Negalime padaryti:    Zmogus z;
// Kompiliatorius gražins klaidą:
//   error: cannot declare variable 'z' to be of abstract type 'Zmogus'
//
// Šis testas DEMONSTRUOJA abstraktumą per static_assert.
// std::is_abstract<T> — type trait, kuris kompiliavimo metu
// patikrina ar tipas yra abstraktus.
// ------------------------------------------------------------

static void testZmogusAbstraktus() {
    skyrius("A. Zmogus yra abstrakti klase");

    // Kompiliavimo meto patikrinimas — jei Zmogus nebūtų abstrakti,
    // šis static_assert neleistų kompiliuoti.
    static_assert(std::is_abstract<Zmogus>::value,
        "Zmogus turi buti abstrakti klase!");
    tikrinti("Zmogus yra abstrakti (static_assert praėjo)",
        std::is_abstract<Zmogus>::value);

    // Runtime tikrinimas — Zmogaus objektas negali būti sukurtas tiesiogiai.
    // Jei išvis bandytume `Zmogus z;` — programa nesikompiliuotų.
    // Žemiau eilutė yra užkomentuota — atkomentavę pamatytumėte:
    //     error C2259: 'Zmogus': cannot instantiate abstract class

    // Zmogus z;                          // <-- KOMPILIATORIAUS KLAIDA
    // Zmogus z2("Jonas", "Jonaitis");    // <-- KOMPILIATORIAUS KLAIDA

    // Bet Studentas (paveldėjusi) instancijuojama be problemų:
    Studentas st;
    tikrinti("Studentas instancijuojamas be klaidu", true);

    // Ir Studentas paveldi iš Zmogus:
    static_assert(std::is_base_of<Zmogus, Studentas>::value,
        "Studentas turi paveldėti Zmogus!");
    tikrinti("Studentas paveldi Zmogus (static_assert praėjo)",
        std::is_base_of<Zmogus, Studentas>::value);
}

// ------------------------------------------------------------
// B. Studentas paveldi Zmogus laukus
// ------------------------------------------------------------

static void testPaveldejimas() {
    skyrius("B. Studentas paveldi Zmogaus laukus");

    Studentas st;
    st.setVardas("Jonas");      // <-- Zmogaus setVardas
    st.setPavarde("Jonaitis");  // <-- Zmogaus setPavarde
    st.setEgz(8);               // <-- Studento setEgz

    // Getteriai paveldėti iš Zmogus
    tikrinti("Zmogus::setVardas + Zmogus::vardas() veikia",
        st.vardas() == "Jonas");
    tikrinti("Zmogus::setPavarde + Zmogus::pavarde() veikia",
        st.pavarde() == "Jonaitis");
    tikrinti("Studento egz() veikia",
        st.egz() == 8);
}

// ------------------------------------------------------------
// C. Virtualus dispatch per bazinę nuorodą
//
// Šis testas parodo, kad turint Zmogus& nuorodą į Studentą,
// kviečiant virtualią funkciją (per operator<<), realiai
// iškviečiama Studentas::spausdinti — ne tuščia bazinė versija.
// Tai vadinama POLIMORFIZMU.
// ------------------------------------------------------------

static void testVirtualusDispatch() {
    skyrius("C. Virtualus dispatch per Zmogus& nuoroda");

    Studentas st("Petras", "Petraitis", { 5, 6, 7, 8, 9 }, 8, 3);

    // Bazinė nuoroda į išvestinį objektą
    const Zmogus& z = st;

    // Per nuorodą kviečiame operator<<, kuris kviečia
    // virtualų spausdinti() — turi būti iškviestas Studentas::spausdinti
    stringstream ss;
    ss << z;
    string rezultatas = ss.str();

    tikrinti("Per Zmogus& isvedami vardas",
        rezultatas.find("Petras") != string::npos);
    tikrinti("Per Zmogus& isvedama pavarde",
        rezultatas.find("Petraitis") != string::npos);
    tikrinti("Per Zmogus& isvedami pazymiai (ND)",
        rezultatas.find("ND") != string::npos);
    tikrinti("Per Zmogus& isvedami papildomi laukai (Egz)",
        rezultatas.find("Egz") != string::npos);

    cout << "  [INFO] Per Zmogus& isvesta: " << z << "\n";
}

// ------------------------------------------------------------
// D. Polimorfizmas su konteineriu (vector<unique_ptr<Zmogus>>)
//
// Standartinis būdas saugoti įvairias paveldėjusias klases
// viename konteineryje — per rodykles į bazinę klasę.
// ------------------------------------------------------------

#include <memory>

static void testPolimorfizmas() {
    skyrius("D. Polimorfizmas (vector<unique_ptr<Zmogus>>)");

    // Konteineris bazinių rodyklių
    vector<std::unique_ptr<Zmogus>> zmones;
    zmones.push_back(std::make_unique<Studentas>(
        "Ona", "Onaite", vector<int>{7, 8, 9}, 8, 1));
    zmones.push_back(std::make_unique<Studentas>(
        "Tomas", "Tomaitis", vector<int>{5, 6, 7}, 6, 1));

    tikrinti("Konteineris turi 2 elementus", zmones.size() == 2);

    // Iteracija per Zmogus* — bet virtualus dispatch suranda Studentas::spausdinti
    cout << "  [INFO] Iteruojama per Zmogus rodykles:\n";
    for (const auto& z : zmones) {
        cout << "         " << *z << "\n";
    }

    tikrinti("Pirmas zmogus yra Ona",
        zmones[0]->vardas() == "Ona");
    tikrinti("Antras zmogus yra Tomas",
        zmones[1]->vardas() == "Tomas");
}

// ============================================================
// v1.2 SENI TESTAI (vis dar turi veikti)
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

static void testPilnasKonstruktorius() {
    skyrius("2. Pilnas konstruktorius");
    vector<int> paz = { 6, 7, 8, 9, 10 };
    Studentas st("Jonas", "Jonaitis", paz, 9, 3);

    tikrinti("vardas == Jonas", st.vardas() == "Jonas");
    tikrinti("pavarde == Jonaitis", st.pavarde() == "Jonaitis");
    tikrinti("egz == 9", st.egz() == 9);
    tikrinti("paz dydzis == 5", st.paz().size() == 5);
    tikrinti("galVid apytikslis", std::abs(st.galVid() - 8.6) < 0.01);
    tikrinti("galMed apytikslis", std::abs(st.galMed() - 8.6) < 0.01);
}

static void testDestruktorius() {
    skyrius("3. Destruktorius");
    {
        Studentas st("Laikinas", "Studentas", { 5, 6, 7 }, 8, 1);
    } // ~Studentas() ir ~Zmogus() iškviečiami čia
    tikrinti("destruktorius iskviestas be crash", true);
}

static void testKopijavimoKonstruktorius() {
    skyrius("4. Kopijavimo konstruktorius");
    vector<int> paz = { 5, 6, 7, 8 };
    Studentas originalas("Petras", "Petraitis", paz, 7, 1);
    Studentas kopija(originalas);

    tikrinti("kopija.vardas == originalas.vardas",
        kopija.vardas() == originalas.vardas());
    tikrinti("kopija.pavarde == originalas.pavarde",
        kopija.pavarde() == originalas.pavarde());
    tikrinti("kopija.egz == originalas.egz",
        kopija.egz() == originalas.egz());

    Studentas kopija2(originalas);
    kopija2.addPazymys(10);
    tikrinti("gili kopija: originalas.paz nepakito",
        originalas.paz().size() == 4);
    tikrinti("gili kopija: kopija2.paz padidejo",
        kopija2.paz().size() == 5);
}

static void testKopijavimoPriskyrimas() {
    skyrius("5. Kopijavimo priskyrimo operatorius");
    Studentas a("Ona", "Onaite", { 4, 5, 6 }, 6, 1);
    Studentas b;
    b = a;

    tikrinti("b.vardas == a.vardas", b.vardas() == a.vardas());
    tikrinti("b.pavarde == a.pavarde", b.pavarde() == a.pavarde());
    tikrinti("b.egz == a.egz", b.egz() == a.egz());

    a = a; // savipriskyrimas
    tikrinti("savipriskyrimas: vardas islieka", a.vardas() == "Ona");
    tikrinti("savipriskyrimas: egz islieka", a.egz() == 6);
}

static void testPerkelimoKonstruktorius() {
    skyrius("6. Perkelimo konstruktorius");
    Studentas originalas("Matas", "Mataitis", { 7, 8, 9 }, 8, 1);
    string senoVardas = originalas.vardas();

    Studentas perkeltas(move(originalas));

    tikrinti("perkeltas.vardas == seno", perkeltas.vardas() == senoVardas);
    tikrinti("perkeltas.egz == 8", perkeltas.egz() == 8);
    tikrinti("perkeltas.paz dydzis == 3", perkeltas.paz().size() == 3);
    tikrinti("originalas.vardas tuscias", originalas.vardas() == "");
    tikrinti("originalas.egz == 0 po move", originalas.egz() == 0);
}

static void testPerkelimoPriskyrimas() {
    skyrius("7. Perkelimo priskyrimo operatorius");
    Studentas a("Ieva", "Ievaite", { 6, 7, 8 }, 7, 1);
    Studentas b;
    string senoVardas = a.vardas();

    b = move(a);

    tikrinti("b.vardas == seno a.vardas", b.vardas() == senoVardas);
    tikrinti("b.egz == 7", b.egz() == 7);
    tikrinti("a.vardas tuscias po move", a.vardas() == "");

    b = move(b); // saviperkėlimas
    tikrinti("saviperkelimas necrasha", true);
}

static void testOutputOperatorius() {
    skyrius("8. operator<<");
    Studentas st("Lukas", "Lukaitis", { 7, 8, 9 }, 8, 3);

    stringstream ss;
    ss << st;
    string rezultatas = ss.str();

    tikrinti("operator<< turi varda", rezultatas.find("Lukas") != string::npos);
    tikrinti("operator<< turi pavarde", rezultatas.find("Lukaitis") != string::npos);
    tikrinti("operator<< turi egz", rezultatas.find("Egz") != string::npos);
    tikrinti("operator<< netusti", !rezultatas.empty());

    cout << "  [INFO] " << st << "\n";
}

static void testInputOperatorius() {
    skyrius("9. operator>>");
    string eilute = "Gabija Gabjaite 7 8 9 6 10 8";
    stringstream ss(eilute);
    Studentas st;
    ss >> st;

    tikrinti("operator>> nuskaite varda", st.vardas() == "Gabija");
    tikrinti("operator>> nuskaite pavarde", st.pavarde() == "Gabjaite");
    tikrinti("operator>> paskutinis sk = egz", st.egz() == 8);
    tikrinti("operator>> 5 ND pazymiai", st.paz().size() == 5);
}

static void testGetterSetter() {
    skyrius("10. Getteriai ir setteriai");
    Studentas st;
    st.setVardas("Rokas");
    st.setPavarde("Rokaitis");
    st.setEgz(9);
    st.addPazymys(7);
    st.addPazymys(8);
    st.addPazymys(9);

    tikrinti("setVardas (paveldetas) / vardas()", st.vardas() == "Rokas");
    tikrinti("setPavarde (paveldetas) / pavarde()", st.pavarde() == "Rokaitis");
    tikrinti("setEgz / egz()", st.egz() == 9);
    tikrinti("addPazymys 3x / paz()", st.paz().size() == 3);

    Studentas st2;
    st2.addPazymys(5);
    st2.addPazymys(6);
    st2.addPazymys(10);
    st2.nustatytiEgzIsGalo();
    tikrinti("nustatytiEgzIsGalo: egz == 10", st2.egz() == 10);
    tikrinti("nustatytiEgzIsGalo: paz == 2", st2.paz().size() == 2);
}

static void testSkaiciavimai() {
    skyrius("11. Skaiciavimo metodai");
    Studentas st;
    st.addPazymys(6);
    st.addPazymys(8);
    st.addPazymys(10);
    st.setEgz(9);

    st.apskaiciuoti(1);
    tikrinti("apskaiciuoti(1): galVid == 8.6",
        std::abs(st.galVid() - 8.6) < 0.01);

    st.apskaiciuoti(2);
    tikrinti("apskaiciuoti(2): galMed == 8.6",
        std::abs(st.galMed() - 8.6) < 0.01);
}

// ============================================================
// main
// ============================================================

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    cout << "========================================\n";
    cout << "  Zmogus/Studentas testai v1.5         \n";
    cout << "========================================\n";

    cout << "\n>>> v1.5 NAUJI TESTAI (paveldejimas) <<<\n";
    testZmogusAbstraktus();
    testPaveldejimas();
    testVirtualusDispatch();
    testPolimorfizmas();

    cout << "\n>>> v1.2 TESTAI (ar vis dar veikia) <<<\n";
    testNumatytasisKonstruktorius();
    testPilnasKonstruktorius();
    testDestruktorius();
    testKopijavimoKonstruktorius();
    testKopijavimoPriskyrimas();
    testPerkelimoKonstruktorius();
    testPerkelimoPriskyrimas();
    testOutputOperatorius();
    testInputOperatorius();
    testGetterSetter();
    testSkaiciavimai();

    cout << "\n========================================\n";
    cout << "  Rezultatai: " << praejo << " PASS, "
        << nepraejo << " FAIL\n";
    cout << "========================================\n";

    return (nepraejo == 0) ? 0 : 1;
}