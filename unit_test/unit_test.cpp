/**
 * @file unit_testai.cpp
 * @brief Unit testai Studentas/Zmogus klasėms naudojant Catch2 framework'ą.
 *
 * REIKALAVIMAI:
 *   Atsisiųskite catch.hpp iš:
 *   https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp
 *   ir įdėkite šalia šio failo (arba į projekto šakninį katalogą).
 *
 * KOMPILIAVIMAS:
 *   g++ -std=c++17 unit_testai.cpp funkcijos.cpp -o unit_testai
 *
 * PALEIDIMAS:
 *   ./unit_testai              # paleidžia visus testus
 *   ./unit_testai -s           # rodo ir praėjusius testus
 *   ./unit_testai "[copy]"     # paleidžia tik tam tikras tag'ų grupes
 *
 * KAS YRA UNIT TESTAI:
 *   Kiekvienas TEST_CASE — vienas izoliuotas testas.
 *   REQUIRE — sąlyga turi būti true; jei ne, testas FAIL ir sustabdomas.
 *   CHECK — kaip REQUIRE, bet testas tęsiamas net jei nepavyko.
 *   SECTION — atskira testų grupės dalis su bendru setup.
 */

#define CATCH_CONFIG_MAIN   // Catch2 sukuria savo main() — mums jo nereikia
#include "catch.hpp"
#include "struktura.h"

 // ============================================================
 // 1. KONSTRUKTORIAI
 // ============================================================

TEST_CASE("Numatytasis konstruktorius sukuria tuscia studenta", "[konstruktoriai]") {
    Studentas st;

    REQUIRE(st.vardas() == "");
    REQUIRE(st.pavarde() == "");
    REQUIRE(st.paz().empty());
    REQUIRE(st.egz() == 0);
    REQUIRE(st.galVid() == 0.0);
    REQUIRE(st.galMed() == 0.0);
}

TEST_CASE("Pilnas konstruktorius su skaiciavimu", "[konstruktoriai]") {
    Studentas st("Jonas", "Jonaitis", { 6, 7, 8, 9, 10 }, 9, 3);

    REQUIRE(st.vardas() == "Jonas");
    REQUIRE(st.pavarde() == "Jonaitis");
    REQUIRE(st.egz() == 9);
    REQUIRE(st.paz().size() == 5);
    // (6+7+8+9+10)/5 = 8.0;  8.0*0.4 + 9*0.6 = 8.6
    REQUIRE(st.galVid() == Approx(8.6).epsilon(0.01));
    REQUIRE(st.galMed() == Approx(8.6).epsilon(0.01));
}

// ============================================================
// 2. RULE OF FIVE — privalomas pagal užduotį
// ============================================================

TEST_CASE("Rule of Five: kopijavimo konstruktorius", "[rule_of_five][copy]") {
    Studentas originalas("Petras", "Petraitis", { 5, 6, 7, 8 }, 7, 1);
    Studentas kopija(originalas);

    SECTION("Kopija turi tas pacias reiksmes") {
        REQUIRE(kopija.vardas() == originalas.vardas());
        REQUIRE(kopija.pavarde() == originalas.pavarde());
        REQUIRE(kopija.egz() == originalas.egz());
        REQUIRE(kopija.paz() == originalas.paz());
        REQUIRE(kopija.galVid() == Approx(originalas.galVid()));
    }

    SECTION("Tai yra GILI kopija (atskiri vector'iai)") {
        kopija.addPazymys(10);
        REQUIRE(originalas.paz().size() == 4);  // originalas nepakito
        REQUIRE(kopija.paz().size() == 5);
    }
}

TEST_CASE("Rule of Five: kopijavimo priskyrimas", "[rule_of_five][copy]") {
    Studentas a("Ona", "Onaite", { 4, 5, 6 }, 6, 1);
    Studentas b;
    b = a;

    REQUIRE(b.vardas() == a.vardas());
    REQUIRE(b.pavarde() == a.pavarde());
    REQUIRE(b.egz() == a.egz());

    SECTION("Savipriskyrimas (a = a) nesugadina objekto") {
        a = a;
        REQUIRE(a.vardas() == "Ona");
        REQUIRE(a.egz() == 6);
    }
}

TEST_CASE("Rule of Five: perkelimo konstruktorius", "[rule_of_five][move]") {
    Studentas originalas("Matas", "Mataitis", { 7, 8, 9 }, 8, 1);
    string senoVardas = originalas.vardas();

    Studentas perkeltas(move(originalas));

    SECTION("Naujas objektas paveldejo duomenis") {
        REQUIRE(perkeltas.vardas() == senoVardas);
        REQUIRE(perkeltas.egz() == 8);
        REQUIRE(perkeltas.paz().size() == 3);
    }

    SECTION("Originalas paliktas tuscios busenos") {
        REQUIRE(originalas.vardas() == "");
        REQUIRE(originalas.egz() == 0);
    }
}

TEST_CASE("Rule of Five: perkelimo priskyrimas", "[rule_of_five][move]") {
    Studentas a("Ieva", "Ievaite", { 6, 7, 8 }, 7, 1);
    Studentas b;
    b = move(a);

    REQUIRE(b.vardas() == "Ieva");
    REQUIRE(b.egz() == 7);
    REQUIRE(a.vardas() == "");

    SECTION("Saviperkelimas (b = move(b)) nesukelia avarijos") {
        b = move(b);
        // Pakanka, kad programa nesukrito
        REQUIRE(true);
    }
}

TEST_CASE("Rule of Five: destruktorius automatiskai isvalo atminti", "[rule_of_five]") {
    // Sukuriame ir leidziame sunaikinti — vector ir string turi automatiskai issivalyti.
    // Jei butu memory leak — sanitizer'iai tai pamatytu.
    {
        Studentas st("Test", "Testaitis", { 1, 2, 3 }, 5, 1);
    } // ~Studentas() iskviestas cia
    REQUIRE(true);
}

// ============================================================
// 3. ABSTRAKTI BAZINE KLASE (v1.5)
// ============================================================

TEST_CASE("Zmogus yra abstrakti — negali buti instancijuotas", "[abstrakti]") {
    // Kompiliavimo metu:
    static_assert(std::is_abstract<Zmogus>::value,
        "Zmogus turi buti abstrakti!");
    REQUIRE(std::is_abstract<Zmogus>::value);

    // Studentas — paveldejusi, NEra abstrakti:
    REQUIRE_FALSE(std::is_abstract<Studentas>::value);
    REQUIRE(std::is_base_of<Zmogus, Studentas>::value);
}

TEST_CASE("Virtualus dispatch per Zmogus& nuoroda", "[abstrakti][polimorfizmas]") {
    Studentas st("Lukas", "Lukaitis", { 7, 8, 9 }, 8, 3);
    const Zmogus& z = st;  // bazine nuoroda

    stringstream ss;
    ss << z;  // virtualus dispatch -> Studentas::spausdinti

    string rez = ss.str();
    REQUIRE(rez.find("Lukas") != string::npos);
    REQUIRE(rez.find("Lukaitis") != string::npos);
    REQUIRE(rez.find("Egz") != string::npos);  // <-- studentui specifinis laukas
}

// ============================================================
// 4. I/O OPERATORIAI
// ============================================================

TEST_CASE("operator<< isveda visus laukus", "[io]") {
    Studentas st("Rokas", "Rokaitis", { 6, 7, 8 }, 9, 3);
    stringstream ss;
    ss << st;
    string rez = ss.str();

    CHECK(rez.find("Rokas") != string::npos);
    CHECK(rez.find("Rokaitis") != string::npos);
    CHECK(rez.find("ND") != string::npos);
    CHECK(rez.find("Egz") != string::npos);
    CHECK(rez.find("Vid") != string::npos);
    CHECK(rez.find("Med") != string::npos);
}

TEST_CASE("operator>> teisingai nuskaito formata", "[io]") {
    stringstream ss("Gabija Gabjaite 7 8 9 6 10 8");
    Studentas st;
    ss >> st;

    REQUIRE(st.vardas() == "Gabija");
    REQUIRE(st.pavarde() == "Gabjaite");
    REQUIRE(st.egz() == 8);          // paskutinis skaicius
    REQUIRE(st.paz().size() == 5);          // visi pries jį
}

// ============================================================
// 5. SKAICIAVIMO METODAI
// ============================================================

TEST_CASE("apskaiciuoti() teisingai skaiciuoja galutini pazymi", "[skaiciavimas]") {
    Studentas st;
    st.addPazymys(6);
    st.addPazymys(8);
    st.addPazymys(10);
    st.setEgz(9);

    SECTION("Metodas 1: vidurkis") {
        st.apskaiciuoti(1);
        // vidurkis = (6+8+10)/3 = 8.0;  8.0*0.4 + 9*0.6 = 8.6
        REQUIRE(st.galVid() == Approx(8.6).epsilon(0.01));
    }

    SECTION("Metodas 2: mediana (nelyginis kiekis)") {
        st.apskaiciuoti(2);
        // surikiuota: {6,8,10}; mediana = 8;  8*0.4 + 9*0.6 = 8.6
        REQUIRE(st.galMed() == Approx(8.6).epsilon(0.01));
    }
}

TEST_CASE("mediana lyginiu paz kiekiu", "[skaiciavimas]") {
    Studentas st;
    st.addPazymys(6);
    st.addPazymys(10);
    st.setEgz(7);
    st.apskaiciuoti(2);
    // mediana = (6+10)/2 = 8;  8*0.4 + 7*0.6 = 3.2 + 4.2 = 7.4
    REQUIRE(st.galMed() == Approx(7.4).epsilon(0.01));
}

// ============================================================
// 6. KRASTINIAI ATVEJAI
// ============================================================

TEST_CASE("Studentas be pazymiu", "[krastiniai_atvejai]") {
    Studentas st;
    st.setEgz(10);
    st.apskaiciuoti(3);

    // Vidurkis ir mediana = 0 (nera pazymiu); galutinis = 0*0.4 + 10*0.6 = 6.0
    REQUIRE(st.galVid() == Approx(6.0));
    REQUIRE(st.galMed() == Approx(6.0));
}

TEST_CASE("nustatytiEgzIsGalo() perkelia paskutini pazymi", "[krastiniai_atvejai]") {
    Studentas st;
    st.addPazymys(5);
    st.addPazymys(6);
    st.addPazymys(9);  // bus egzaminas

    st.nustatytiEgzIsGalo();

    REQUIRE(st.egz() == 9);
    REQUIRE(st.paz().size() == 2);
    REQUIRE(st.paz()[0] == 5);
    REQUIRE(st.paz()[1] == 6);
}