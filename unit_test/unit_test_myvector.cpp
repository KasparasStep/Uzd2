/**
 * @file unit_test_myvector.cpp
 * @brief Unit testai MyVector klasei naudojant Catch2 framework'ą.
 *
 * Sis failas NETURI CATCH_CONFIG_MAIN — main() yra unit_test.cpp.
 * Abu failai sukompiliuojami i ta pati vykdomaji faila `unit_testai`.
 *
 * PALEIDIMAS:
 *   ./unit_testai                    # paleidzia VISUS testus (Studentas + MyVector)
 *   ./unit_testai "[myvector]"       # tik MyVector testus
 *   ./unit_testai "[rule_of_five]"   # Rule of Five testus is abieju klasiu
 *
 * STRUKTURA (analogiska unit_test.cpp):
 *   1. Konstruktoriai
 *   2. Rule of Five
 *   3. Elementu prieiga
 *   4. Iteratoriai
 *   5. Talpa (capacity / reserve)
 *   6. Modifikatoriai (push_back, insert, erase, ...)
 *   7. Palyginimo operatoriai
 *   8. STL suderinamumas (std::sort, std::accumulate)
 *   9. Krastiniai atvejai
 */

 // CATCH_CONFIG_FAST_COMPILE mazina kompiliavimo laika ir pagerina
 // VS IntelliSense suderinamuma su Catch2 makrosais.
 // Sita define TURI buti PRIES catch.hpp include.
#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE
#endif

#include "catch.hpp"

// Tiesioginis MyVector.h itraukimas (apsauga jei struktura.h
// dar neatnaujinta ir neturi #include "MyVector.h")
#include "struktura.h"
#ifdef _WIN32
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#endif
#include "MyVector.h"

#include <string>
#include <algorithm>
#include <numeric>
#include <sstream>

using std::string;

// ============================================================
// 1. KONSTRUKTORIAI
// ============================================================

TEST_CASE("MyVector: Numatytasis konstruktorius sukuria tuscia vektoriu", "[myvector][konstruktoriai]") {
    MyVector<int> v;

    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
    REQUIRE(v.empty());
    REQUIRE(v.data() == nullptr);
}

TEST_CASE("MyVector: Konstruktorius su elementu skaiciumi", "[myvector][konstruktoriai]") {
    MyVector<int> v(5);

    SECTION("Dydis ir talpa nustatyti teisingai") {
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
        REQUIRE_FALSE(v.empty());
    }

    SECTION("Elementai inicializuoti numatytaja reiksme (int -> 0)") {
        for (size_t i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == 0);
        }
    }
}

TEST_CASE("MyVector: Konstruktorius su reiksme: MyVector(count, value)", "[myvector][konstruktoriai]") {
    MyVector<int> v(4, 99);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 99);
    REQUIRE(v[3] == 99);
}

TEST_CASE("MyVector: Konstruktorius is initializer_list", "[myvector][konstruktoriai]") {
    MyVector<int> v{ 1, 2, 3, 4, 5 };

    REQUIRE(v.size() == 5);
    REQUIRE(v.front() == 1);
    REQUIRE(v.back() == 5);
    REQUIRE(v[2] == 3);
}

TEST_CASE("MyVector: Konstruktorius is iteratoriu intervalo", "[myvector][konstruktoriai]") {
    std::vector<int> saltinis{ 10, 20, 30, 40 };
    MyVector<int> v(saltinis.begin(), saltinis.end());

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 10);
    REQUIRE(v[3] == 40);
}

// ============================================================
// 2. RULE OF FIVE — privalomas pagal uzduoti
// ============================================================

TEST_CASE("MyVector: Rule of Five: kopijavimo konstruktorius", "[myvector][rule_of_five][copy]") {
    MyVector<int> originalas{ 1, 2, 3, 4, 5 };
    MyVector<int> kopija(originalas);

    SECTION("Kopija turi tuos pacius elementus") {
        REQUIRE(kopija.size() == originalas.size());
        for (size_t i = 0; i < kopija.size(); ++i) {
            REQUIRE(kopija[i] == originalas[i]);
        }
    }

    SECTION("Tai yra GILI kopija (atskira atmintis)") {
        kopija[0] = 999;
        REQUIRE(originalas[0] == 1);   // originalas nepakito
        REQUIRE(kopija[0] == 999);
    }

    SECTION("Kopijos data() rodo i KITA atminties bloka") {
        REQUIRE(kopija.data() != originalas.data());
    }
}

TEST_CASE("MyVector: Rule of Five: kopijavimo priskyrimas", "[myvector][rule_of_five][copy]") {
    MyVector<int> a{ 1, 2, 3 };
    MyVector<int> b{ 9, 9 };
    b = a;

    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
    REQUIRE(b[2] == 3);

    SECTION("Savipriskyrimas (a = a) nesugadina objekto") {
        a = a;
        REQUIRE(a.size() == 3);
        REQUIRE(a[1] == 2);
    }
}

TEST_CASE("MyVector: Rule of Five: perkelimo konstruktorius", "[myvector][rule_of_five][move]") {
    MyVector<int> originalas{ 1, 2, 3, 4 };
    int* senasPointer = originalas.data();

    MyVector<int> perkeltas(std::move(originalas));

    SECTION("Naujas objektas paveldejo duomenis") {
        REQUIRE(perkeltas.size() == 4);
        REQUIRE(perkeltas[0] == 1);
        REQUIRE(perkeltas[3] == 4);
        // Tas pats atminties blokas (perkelta, ne kopijuota)
        REQUIRE(perkeltas.data() == senasPointer);
    }

    SECTION("Originalas paliktas tuscios busenos") {
        REQUIRE(originalas.size() == 0);
        REQUIRE(originalas.capacity() == 0);
        REQUIRE(originalas.data() == nullptr);
    }
}

TEST_CASE("MyVector: Rule of Five: perkelimo priskyrimas", "[myvector][rule_of_five][move]") {
    MyVector<int> a{ 1, 2, 3 };
    MyVector<int> b{ 7, 8 };
    b = std::move(a);

    REQUIRE(b.size() == 3);
    REQUIRE(b[2] == 3);
    REQUIRE(a.size() == 0);

    SECTION("Saviperkelimas (b = move(b)) nesukelia avarijos") {
        b = std::move(b);
        REQUIRE(true);  // pakanka, kad nesukrito
    }
}

TEST_CASE("MyVector: Rule of Five: destruktorius issivalo atminti", "[myvector][rule_of_five]") {
    // Sukuriame ir leidziame sunaikinti — atmintis turi issivalyti.
    // Memory leak'ai butu pagauti sanitizer'iu (-fsanitize=address).
    {
        MyVector<std::string> v;
        for (int i = 0; i < 100; ++i)
            v.push_back("studentas " + std::to_string(i));
    } // ~MyVector() iskviestas cia
    REQUIRE(true);
}

// ============================================================
// 3. ELEMENTU PRIEIGA
// ============================================================

TEST_CASE("MyVector: operator[] suteikia prieiga prie elementu", "[myvector][prieiga]") {
    MyVector<int> v{ 10, 20, 30 };

    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);

    SECTION("operator[] leidzia keisti elementa") {
        v[1] = 999;
        REQUIRE(v[1] == 999);
    }
}

TEST_CASE("MyVector: at() meta out_of_range, jei indeksas virsija ribas", "[myvector][prieiga]") {
    MyVector<int> v{ 1, 2, 3 };

    REQUIRE(v.at(0) == 1);
    REQUIRE(v.at(2) == 3);
    REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
    REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
}

TEST_CASE("MyVector: front() ir back() grazina pirma/paskutini elementa", "[myvector][prieiga]") {
    MyVector<int> v{ 5, 10, 15, 20 };

    REQUIRE(v.front() == 5);
    REQUIRE(v.back() == 20);

    SECTION("front() ir back() leidzia keisti elementus") {
        v.front() = 99;
        v.back() = -1;
        REQUIRE(v[0] == 99);
        REQUIRE(v[3] == -1);
    }
}

// ============================================================
// 4. ITERATORIAI
// ============================================================

TEST_CASE("MyVector: begin/end leidzia eiti per elementus", "[myvector][iteratoriai]") {
    MyVector<int> v{ 1, 2, 3, 4, 5 };

    int suma = 0;
    for (auto it = v.begin(); it != v.end(); ++it) suma += *it;
    REQUIRE(suma == 15);
}

TEST_CASE("MyVector: range-for veikia su MyVector", "[myvector][iteratoriai]") {
    MyVector<int> v{ 2, 4, 6 };

    int sandauga = 1;
    for (int x : v) sandauga *= x;
    REQUIRE(sandauga == 48);
}

TEST_CASE("MyVector: rbegin/rend leidzia eiti atvirkstine tvarka", "[myvector][iteratoriai]") {
    MyVector<int> v{ 1, 2, 3 };

    std::string rez;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        rez += std::to_string(*it);
    REQUIRE(rez == "321");
}

// ============================================================
// 5. TALPA (capacity, reserve)
// ============================================================

TEST_CASE("MyVector: reserve() padidina talpa nekeicdamas size()", "[myvector][talpa]") {
    MyVector<int> v;
    v.reserve(100);

    REQUIRE(v.capacity() >= 100);
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());
}

TEST_CASE("MyVector: push_back automatiskai augina talpa", "[myvector][talpa]") {
    MyVector<int> v;
    size_t paskutineCapacity = v.capacity();
    int augimu = 0;

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
        if (v.capacity() != paskutineCapacity) {
            ++augimu;
            paskutineCapacity = v.capacity();
        }
    }

    REQUIRE(v.size() == 100);
    REQUIRE(augimu > 0);             // bent karta turejo augti
    REQUIRE(v.capacity() >= 100);
}

TEST_CASE("MyVector: shrink_to_fit() sumazina talpa iki size()", "[myvector][talpa]") {
    MyVector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);

    REQUIRE(v.capacity() >= 100);
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 2);
    REQUIRE(v.size() == 2);
}

// ============================================================
// 6. MODIFIKATORIAI
// ============================================================

TEST_CASE("MyVector: push_back prideda elementa gale", "[myvector][modifikatoriai]") {
    MyVector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1);
    REQUIRE(v.back() == 3);
}

TEST_CASE("MyVector: pop_back pasalina paskutini elementa", "[myvector][modifikatoriai]") {
    MyVector<int> v{ 1, 2, 3 };
    v.pop_back();

    REQUIRE(v.size() == 2);
    REQUIRE(v.back() == 2);
}

TEST_CASE("MyVector: insert iterpia elementa viduryje", "[myvector][modifikatoriai]") {
    MyVector<int> v{ 1, 2, 4 };
    v.insert(v.begin() + 2, 3);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
}

TEST_CASE("MyVector: erase pasalina viena elementa", "[myvector][modifikatoriai]") {
    MyVector<int> v{ 1, 2, 3, 4, 5 };
    v.erase(v.begin() + 2);

    REQUIRE(v.size() == 4);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 4);     // 3 dingo, 4 atsidure jo vietoje
    REQUIRE(v[3] == 5);
}

TEST_CASE("MyVector: erase pasalina intervala", "[myvector][modifikatoriai]") {
    MyVector<int> v{ 1, 2, 3, 4, 5 };
    v.erase(v.begin() + 1, v.begin() + 4);

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 5);
}

TEST_CASE("MyVector: clear pasalina visus elementus", "[myvector][modifikatoriai]") {
    MyVector<int> v{ 1, 2, 3 };
    size_t saugomaCapacity = v.capacity();
    v.clear();

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == saugomaCapacity);  // talpa nesikeicia
}

TEST_CASE("MyVector: resize keicia dydzi", "[myvector][modifikatoriai]") {
    MyVector<int> v{ 1, 2, 3 };

    SECTION("Didinimas pridejus numatytasias reiksmes") {
        v.resize(5);
        REQUIRE(v.size() == 5);
        REQUIRE(v[3] == 0);
        REQUIRE(v[4] == 0);
    }

    SECTION("Didinimas pridejus konkrecia reiksme") {
        v.resize(5, 99);
        REQUIRE(v.size() == 5);
        REQUIRE(v[4] == 99);
    }

    SECTION("Mazinimas pasalina elementus is galo") {
        v.resize(2);
        REQUIRE(v.size() == 2);
        REQUIRE(v[1] == 2);
    }
}

TEST_CASE("MyVector: emplace_back sukonstruoja elementa vietoje", "[myvector][modifikatoriai]") {
    MyVector<std::string> v;
    v.emplace_back("labas");
    v.emplace_back(3, 'x');   // string(3, 'x') = "xxx"

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == "labas");
    REQUIRE(v[1] == "xxx");
}

// ============================================================
// 7. PALYGINIMO OPERATORIAI
// ============================================================

TEST_CASE("MyVector: operator== ir != lygina elementus", "[myvector][operatoriai]") {
    MyVector<int> a{ 1, 2, 3 };
    MyVector<int> b{ 1, 2, 3 };
    MyVector<int> c{ 1, 2, 4 };

    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("MyVector: operator< palygina leksikografiskai", "[myvector][operatoriai]") {
    MyVector<int> a{ 1, 2, 3 };
    MyVector<int> c{ 1, 2, 4 };

    REQUIRE(a < c);
    REQUIRE(c > a);
    REQUIRE(a <= a);
    REQUIRE(a >= a);
}

TEST_CASE("MyVector: swap sukeicia turini O(1) laiku", "[myvector][operatoriai]") {
    MyVector<int> a{ 1, 2, 3 };
    MyVector<int> b{ 9, 8 };
    swap(a, b);

    REQUIRE(a.size() == 2);
    REQUIRE(a[0] == 9);
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
}

// ============================================================
// 8. STL SUDERINAMUMAS
// ============================================================

TEST_CASE("MyVector: std::sort veikia su MyVector iteratoriais", "[myvector][stl]") {
    MyVector<int> v{ 5, 3, 1, 4, 2 };
    std::sort(v.begin(), v.end());

    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v[4] == 5);
}

TEST_CASE("MyVector: std::accumulate veikia su MyVector", "[myvector][stl]") {
    MyVector<int> v{ 1, 2, 3, 4, 5 };
    int suma = std::accumulate(v.begin(), v.end(), 0);
    REQUIRE(suma == 15);
}

TEST_CASE("MyVector: MyVector<string> veikia su netrivialiais tipais", "[myvector][stl]") {
    MyVector<std::string> v;
    for (int i = 0; i < 50; ++i)
        v.push_back("studentas " + std::to_string(i));

    REQUIRE(v.size() == 50);
    REQUIRE(v[0] == "studentas 0");
    REQUIRE(v[49] == "studentas 49");

    SECTION("std::sort dirba ir su stringais") {
        MyVector<std::string> mazas{ "obuolys", "agurkas", "morka" };
        std::sort(mazas.begin(), mazas.end());
        REQUIRE(mazas[0] == "agurkas");
        REQUIRE(mazas[1] == "morka");
        REQUIRE(mazas[2] == "obuolys");
    }
}

// ============================================================
// 9. KRASTINIAI ATVEJAI
// ============================================================

TEST_CASE("MyVector: Tuscias MyVector korektiskai elgiasi", "[myvector][krastiniai_atvejai]") {
    MyVector<int> v;

    REQUIRE(v.empty());
    REQUIRE(v.begin() == v.end());  // jokio elemento
    REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
}

TEST_CASE("MyVector: MyVector veikia ir su Studentas (musu klase)", "[myvector][krastiniai_atvejai]") {
    MyVector<Studentas> grupe;
    grupe.push_back(Studentas("Jonas", "Jonaitis", { 8, 9, 10 }, 9, 1));
    grupe.push_back(Studentas("Ona", "Onaite", { 5, 5, 5 }, 4, 1));

    REQUIRE(grupe.size() == 2);
    REQUIRE(grupe[0].vardas() == "Jonas");
    REQUIRE(grupe[1].vardas() == "Ona");

    SECTION("std::sort veikia ir su Studentas objektais") {
        std::sort(grupe.begin(), grupe.end(),
            [](const Studentas& a, const Studentas& b) {
                return a.galVid() > b.galVid();
            });
        REQUIRE(grupe[0].vardas() == "Jonas");  // didesnis pazymys -> pirmas
    }
}