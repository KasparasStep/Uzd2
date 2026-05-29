#include "struktura.h"

static const string DATA_DIR = "Data/";

void test1() {
    cout << "\n=== TYRIMAS 1: Failu kurimas ===\n";
    fs::create_directories(DATA_DIR);
    const vector<pair<string, int>> failai = {
        {DATA_DIR + "studentai1k.txt",    1'000},
        {DATA_DIR + "studentai10k.txt",   10'000},
        {DATA_DIR + "studentai100k.txt",  100'000},
        {DATA_DIR + "studentai1M.txt",    1'000'000},
        {DATA_DIR + "studentai10M.txt",   10'000'000}
    };
    for (const auto& [vardas, kiek] : failai) {
        cout << "Generuojama " << vardas << " (" << kiek << " irasu)... ";
        auto t1 = high_resolution_clock::now();
        genFaila(vardas, kiek);
        auto t2 = high_resolution_clock::now();
        cout << fixed << setprecision(3)
            << duration<double>(t2 - t1).count() << " s\n";
    }
}

// S2 — naivi su erase (MyVector::erase viduryje O(n), visa strategija O(n^2))
static void splitStudents_S2(MyVector<Studentas>& grupe,
    MyVector<Studentas>& tinginiai, int metodas) {
    auto it = grupe.begin();
    while (it != grupe.end()) {
        double g = (metodas == 2) ? it->galMed() : it->galVid();
        if (g < 5.0) {
            tinginiai.push_back(move(*it));
            it = grupe.erase(it);
        }
        else {
            ++it;
        }
    }
}

// S3 — efektyvi su stable_partition + move
static void splitStudents_S3(MyVector<Studentas>& grupe,
    MyVector<Studentas>& tinginiai, int metodas) {
    auto yraKietas = [&](const Studentas& st) {
        double g = (metodas == 2) ? st.galMed() : st.galVid();
        return g >= 5.0;
        };
    auto riba = stable_partition(grupe.begin(), grupe.end(), yraKietas);
    tinginiai.assign(make_move_iterator(riba), make_move_iterator(grupe.end()));
    grupe.erase(riba, grupe.end());
}

void test2(const string& /* nenaudojamas */, int metodas) {
    cout << "\n=== TYRIMAS 2: Duomenu apdorojimas (MyVector) ===\n";
    cout << fixed << setprecision(4);
    cout << left
        << setw(22) << "Failas"
        << setw(13) << "Nuskaitym."
        << setw(13) << "Rusiavim."
        << setw(14) << "S1 (naivi)"
        << setw(14) << "S2 (erase)"
        << setw(14) << "S3 (partit.)"
        << "\n" << string(90, '-') << "\n";

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
            cout << setw(22) << failas << "NERASTAS — paleiskite Tyrima 1.\n";
            continue;
        }
        tikrinimas.close();

        MyVector<Studentas> originalas;
        auto t1 = high_resolution_clock::now();
        skaitytiIsFailo(failas, originalas, metodas);
        auto t2 = high_resolution_clock::now();

        MyVector<Studentas> rusiotas = originalas;
        sort(rusiotas.begin(), rusiotas.end(), [&](const Studentas& a, const Studentas& b) {
            double ga = (metodas == 2) ? a.galMed() : a.galVid();
            double gb = (metodas == 2) ? b.galMed() : b.galVid();
            return ga > gb;
            });
        auto t3 = high_resolution_clock::now();

        MyVector<Studentas> kieti_s1, tinginiai_s1;
        auto t4 = high_resolution_clock::now();
        splitStudents(rusiotas, kieti_s1, tinginiai_s1, metodas);
        auto t5 = high_resolution_clock::now();

        MyVector<Studentas> s2 = rusiotas;
        MyVector<Studentas> tinginiai_s2;
        auto t6 = high_resolution_clock::now();
        splitStudents_S2(s2, tinginiai_s2, metodas);
        auto t7 = high_resolution_clock::now();

        MyVector<Studentas> s3 = rusiotas;
        MyVector<Studentas> tinginiai_s3;
        auto t8 = high_resolution_clock::now();
        splitStudents_S3(s3, tinginiai_s3, metodas);
        auto t9 = high_resolution_clock::now();

        cout << setw(22) << failas
            << setw(13) << duration<double>(t2 - t1).count()
            << setw(13) << duration<double>(t3 - t2).count()
            << setw(14) << duration<double>(t5 - t4).count()
            << setw(14) << duration<double>(t7 - t6).count()
            << setw(14) << duration<double>(t9 - t8).count()
            << "  (kieti: " << kieti_s1.size()
            << ", tinginiai: " << tinginiai_s1.size() << ")\n";
    }
}