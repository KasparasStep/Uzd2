//testavimas.cpp
#include "struktura.h"

static const string DATA_DIR = "Data/";

// ============================================================
// test1 — failų kūrimo greičio tyrimas
// ============================================================

void test1() {
    cout << "\n=== TYRIMAS 1: Failų kūrimas ===\n";
    fs::create_directories(DATA_DIR);
    const vector<pair<string, int>> failai = {
        {DATA_DIR + "studentai1k.txt",    1'000},
        {DATA_DIR + "studentai10k.txt",   10'000},
        {DATA_DIR + "studentai100k.txt",  100'000},
        {DATA_DIR + "studentai1M.txt",    1'000'000},
        {DATA_DIR + "studentai10M.txt",   10'000'000}
    };
    for (const auto& [vardas, kiek] : failai) {
        cout << "Generuojama " << vardas << " (" << kiek << " įrašų)... ";
        auto t1 = high_resolution_clock::now();
        genFaila(vardas, kiek);
        auto t2 = high_resolution_clock::now();
        cout << fixed << setprecision(3)
            << duration<double>(t2 - t1).count() << " s\n";
    }
}

// ============================================================
// Pagalbinė: splitStudents_S3 — 3 strategija su stable_partition
// (naudojama tik test2 lyginimui; vector versija)
// ============================================================

static void splitStudents_S3(vector<Studentas>& grupe,
    vector<Studentas>& vargsiukai, int metodas) {
    auto yraKietas = [&](const Studentas& st) {
        double g = (metodas == 2) ? st.gal_med : st.gal_vid;
        return g >= 5.0;
        };
    auto riba = stable_partition(grupe.begin(), grupe.end(), yraKietas);
    vargsiukai.assign(make_move_iterator(riba),
        make_move_iterator(grupe.end()));
    grupe.erase(riba, grupe.end());
}

// ============================================================
// test2 — duomenų apdorojimo greičio tyrimas (std::vector)
// Matuojami žingsniai: nuskaitymas, rūšiavimas, skaidymas (S1 ir S3)
// ============================================================

void test2(const string& /* nenaudojamas */, int metodas) {
    cout << "\n=== TYRIMAS 2: Duomenų apdorojimas (std::vector) ===\n";
    cout << fixed << setprecision(4);
    cout << left
        << setw(22) << "Failas"
        << setw(14) << "Nuskaitymas"
        << setw(14) << "Rūšiavimas"
        << setw(16) << "S1 (copy_if)"
        << setw(16) << "S3 (partition)"
        << "\n" << string(82, '-') << "\n";

    const vector<string> failai = {
        DATA_DIR + "studentai1k.txt",
        DATA_DIR + "studentai10k.txt",
        DATA_DIR + "studentai100k.txt",
        DATA_DIR + "studentai1M.txt",
        DATA_DIR + "studentai10M.txt"
    };

    for (const auto& failas : failai) {
        ifstream tikrinimas(failas);
        if (!tikrinimas) {
            cout << setw(22) << failas
                << "NERASTAS — paleiskite Tyrimą 1.\n";
            continue;
        }
        tikrinimas.close();

        // Nuskaitymas
        vector<Studentas> originalas;
        auto t1 = high_resolution_clock::now();
        skaitytiIsFailo(failas, originalas, metodas);
        auto t2 = high_resolution_clock::now();

        // Rūšiavimas
        vector<Studentas> rusiotas = originalas;
        sort(rusiotas.begin(), rusiotas.end(), [&](const Studentas& a, const Studentas& b) {
            double ga = (metodas == 2) ? a.gal_med : a.gal_vid;
            double gb = (metodas == 2) ? b.gal_med : b.gal_vid;
            return ga > gb;
            });
        auto t3 = high_resolution_clock::now();

        // S1 — du nauji konteineriai
        vector<Studentas> kieti_s1, vargsiukai_s1;
        auto t4 = high_resolution_clock::now();
        splitStudents(rusiotas, kieti_s1, vargsiukai_s1, metodas);
        auto t5 = high_resolution_clock::now();

        // S3 — stable_partition + move (dirbame su kopija)
        vector<Studentas> s3 = rusiotas;
        vector<Studentas> vargsiukai_s3;
        auto t6 = high_resolution_clock::now();
        splitStudents_S3(s3, vargsiukai_s3, metodas);
        auto t7 = high_resolution_clock::now();

        cout << setw(22) << failas
            << setw(14) << duration<double>(t2 - t1).count()
            << setw(14) << duration<double>(t3 - t2).count()
            << setw(16) << duration<double>(t5 - t4).count()
            << setw(16) << duration<double>(t7 - t6).count()
            << "  (" << kieti_s1.size() << " kieti, "
            << vargsiukai_s1.size() << " vargsiukai)\n";
    }
}