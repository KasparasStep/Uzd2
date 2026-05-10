// ============================================================
// tyrimas su std::list
//
// Pagrindiniai skirtumai nuo vector:
//   - Rūšiavimui naudojamas grupe.sort() (narys), ne std::sort
//   - S3 strategijoje tinginiai perkeliami per splice() — be kopijavimo
//   - Nėra reserve() (list to nepalaiko)
//
// Kompiliavimas:
//   g++ -O2 -std=c++17 List.cpp funkcijos.cpp -o stud_List
// ============================================================
#include "struktura.h"
#ifdef _WIN32
#include <windows.h>
#endif

static const string DATA_DIR = "Data/";

// ---- Duomenų skaitymas ----

static void skaityti(const string& failas, list<Studentas>& grupe, int metodas) {
    ifstream in(failas);
    if (!in) throw runtime_error("Failas nerastas: " + failas);

    string eilute;
    getline(in, eilute); // praleisti antraštę

    while (getline(in, eilute)) {
        if (eilute.empty()) continue;
        stringstream ss(eilute);
        Studentas st;
        string v, p;
        if (!(ss >> v >> p)) continue;
        st.setVardas(v);
        st.setPavarde(p);
        int paz;
        while (ss >> paz) st.addPazymys(paz);
        if (!st.paz().empty()) {
            st.nustatytiEgzIsGalo();
            st.apskaiciuoti(metodas);
            grupe.push_back(move(st));
        }
    }
}

// ---- Rūšiavimas ----
// std::sort() su list neveikia (nėra atsitiktinės prieigos iteratoriaus).
// list turi savo sort() narį, kuris rūšiuoja perskirstydamas mazgų rodykles.

static void rusiuoti(list<Studentas>& grupe, int metodas) {
    grupe.sort([&](const Studentas& a, const Studentas& b) {
        double ga = (metodas == 2) ? a.galMed() : a.galVid();
        double gb = (metodas == 2) ? b.galMed() : b.galVid();
        return ga > gb;
        });
}

// ---- 1 strategija: copy_if į du naujus konteinerius ----

static void split_S1(const list<Studentas>& grupe,
    list<Studentas>& kieti,
    list<Studentas>& tinginiai, int metodas) {
    auto galutinis = [&](const Studentas& st) {
        return (metodas == 2) ? st.galMed() : st.galVid();
        };
    copy_if(grupe.begin(), grupe.end(), back_inserter(kieti),
        [&](const Studentas& st) { return galutinis(st) >= 5.0; });
    copy_if(grupe.begin(), grupe.end(), back_inserter(tinginiai),
        [&](const Studentas& st) { return galutinis(st) < 5.0; });
}

// ---- 3 strategija: stable_partition + splice ----
// splice() perima mazgus tiesiai iš grupe — nekopijuoja duomenų, tik perkelia rodykles.

static void split_S3(list<Studentas>& grupe,
    list<Studentas>& tinginiai, int metodas) {
    auto yraKietas = [&](const Studentas& st) {
        double g = (metodas == 2) ? st.galMed() : st.galVid();
        return g >= 5.0;
        };
    auto riba = stable_partition(grupe.begin(), grupe.end(), yraKietas);
    // splice: O(k) mazgų perkėlimas be kopijavimo (tik rodyklių pakeitimas)
    tinginiai.splice(tinginiai.end(), grupe, riba, grupe.end());
}

// ---- Tyrimo lentelė ----

static void vykdytiTyryma(int metodas) {
    const vector<pair<string, int>> failai = {
        {DATA_DIR + "studentai1k.txt",    1'000},
        {DATA_DIR + "studentai10k.txt",   10'000},
        {DATA_DIR + "studentai100k.txt",  100'000},
        {DATA_DIR + "studentai1M.txt",    1'000'000},
        {DATA_DIR + "studentai10M.txt",   10'000'000}
    };

    cout << "\n=== std::list TYRIMAS ===\n";
    cout << fixed << setprecision(4);
    cout << left
        << setw(22) << "Failas"
        << setw(14) << "Nuskaitymas"
        << setw(14) << "Rūšiavimas"
        << setw(16) << "S1 (copy_if)"
        << setw(16) << "S3 (partition)"
        << "\n" << string(82, '-') << "\n";

    for (const auto& [failas, kiek] : failai) {
        ifstream tikrinimas(failas);
        if (!tikrinimas) {
            cout << setw(22) << failas << "NERASTAS — praleista\n";
            continue;
        }
        tikrinimas.close();

        // 1. Nuskaitymas
        list<Studentas> originalas;
        auto t1 = high_resolution_clock::now();
        skaityti(failas, originalas, metodas);
        auto t2 = high_resolution_clock::now();

        // 2. Rūšiavimas
        list<Studentas> rusiotas = originalas;
        auto t3 = high_resolution_clock::now();
        rusiuoti(rusiotas, metodas);
        auto t4 = high_resolution_clock::now();

        // 3. S1 strategija
        list<Studentas> kieti_s1, tinginiai_s1;
        auto t5 = high_resolution_clock::now();
        split_S1(rusiotas, kieti_s1, tinginiai_s1, metodas);
        auto t6 = high_resolution_clock::now();

        // 4. S3 strategija
        list<Studentas> s3 = rusiotas;
        list<Studentas> tinginiai_s3;
        auto t7 = high_resolution_clock::now();
        split_S3(s3, tinginiai_s3, metodas);
        auto t8 = high_resolution_clock::now();

        cout << setw(22) << failas
            << setw(14) << duration<double>(t2 - t1).count()
            << setw(14) << duration<double>(t4 - t3).count()
            << setw(16) << duration<double>(t6 - t5).count()
            << setw(16) << duration<double>(t8 - t7).count()
            << "  (kieti: " << s3.size()
            << ", tinginiai: " << tinginiai_s3.size() << ")\n";
    }
}

// ---- Failų generavimas ----

static void generuotiFailus() {
    fs::create_directories(DATA_DIR);
    const vector<pair<string, int>> failai = {
        {DATA_DIR + "studentai1k.txt",    1'000},
        {DATA_DIR + "studentai10k.txt",   10'000},
        {DATA_DIR + "studentai100k.txt",  100'000},
        {DATA_DIR + "studentai1M.txt",    1'000'000},
        {DATA_DIR + "studentai10M.txt",   10'000'000}
    };
    for (const auto& [vardas, kiek] : failai) {
        cout << "Generuojama " << vardas << " (" << kiek << " įrašų)...\n";
        auto t1 = high_resolution_clock::now();
        genFaila(vardas, kiek);
        auto t2 = high_resolution_clock::now();
        cout << "  Sugeneruota per " << fixed << setprecision(3)
            << duration<double>(t2 - t1).count() << " s\n";
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    try {
        cout << "Skaičiavimo metodas:\n1 - Vidurkis\n2 - Mediana\n";
        int metodas = gautiSkaiciu("Pasirinkimas: ", 1, 2);

        cout << "\nAr reikia generuoti testavimo failus?\n1 - Taip\n0 - Ne\n";
        if (gautiSkaiciu("Pasirinkimas: ", 0, 1) == 1)
            generuotiFailus();

        vykdytiTyryma(metodas);
    }
    catch (const exception& e) {
        cerr << "Klaida: " << e.what() << "\n";
        return 1;
    }
    return 0;
}