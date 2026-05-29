// ============================================================
// palyginimas.cpp — std::vector vs MyVector spartos palyginimas
// su studentų duomenimis (visos pipeline operacijos)
// ============================================================
#include "struktura.h"

// Šabloninė funkcija: tas pats kodas paleidžiamas su skirtingais konteineriais.
// Container = std::vector<Studentas> arba MyVector<Studentas>.
// Matuojami: nuskaitymas, rūšiavimas, S1 (naivi), S3 (stable_partition).
template <typename Container>
void pilnasCiklas(const string& failas, int metodas, const string& tipas) {
    Container grupe;

    // --- 1. Nuskaitymas iš failo ---
    auto t1 = high_resolution_clock::now();
    {
        ifstream in(failas);
        if (!in) { cout << "  NERASTAS: " << failas << "\n"; return; }
        string eilute;
        getline(in, eilute);  // praleisti antraštę
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
            st.nustatytiEgzIsGalo();
            st.apskaiciuoti(metodas);
            grupe.push_back(move(st));
        }
    }
    auto t2 = high_resolution_clock::now();

    // --- 2. Rūšiavimas ---
    sort(grupe.begin(), grupe.end(), [&](const Studentas& a, const Studentas& b) {
        double ga = (metodas == 2) ? a.galMed() : a.galVid();
        double gb = (metodas == 2) ? b.galMed() : b.galVid();
        return ga > gb;
        });
    auto t3 = high_resolution_clock::now();

    // --- 3. S1 (naivi: du nauji konteineriai) ---
    Container kieti_s1, tinginiai_s1;
    for (const auto& s : grupe) {
        double g = (metodas == 2) ? s.galMed() : s.galVid();
        if (g >= 5.0) kieti_s1.push_back(s);
        else          tinginiai_s1.push_back(s);
    }
    auto t4 = high_resolution_clock::now();

    // --- 4. S3 (stable_partition + move) ---
    Container grupe3 = grupe;            // kopija — S3 dirba vietoje
    Container tinginiai_s3;
    {
        auto pred = [&](const Studentas& s) {
            return ((metodas == 2) ? s.galMed() : s.galVid()) >= 5.0;
            };
        auto riba = stable_partition(grupe3.begin(), grupe3.end(), pred);
        tinginiai_s3.assign(make_move_iterator(riba),
            make_move_iterator(grupe3.end()));
        grupe3.erase(riba, grupe3.end());
    }
    auto t5 = high_resolution_clock::now();

    // --- Spausdinam laikus ---
    cout << left << setw(15) << tipas
        << setw(13) << duration<double>(t2 - t1).count()
        << setw(13) << duration<double>(t3 - t2).count()
        << setw(13) << duration<double>(t4 - t3).count()
        << setw(13) << duration<double>(t5 - t4).count()
        << "  (kieti=" << kieti_s1.size()
        << ", tinginiai=" << tinginiai_s1.size() << ")\n";
}

int main() {
    cout << fixed << setprecision(4);

    // Patikrinam, kad failai egzistuoja; jei ne — sugeneruojam
    fs::create_directories("Data");
    const vector<pair<string, int>> failai = {
        {"Data/studentai100k.txt", 100'000},
        {"Data/studentai1M.txt",  1'000'000},
        {"Data/studentai10M.txt", 10000000}
    };
    for (const auto& [f, n] : failai) {
        ifstream tikrint(f);
        if (!tikrint) {
            cout << "Generuojam " << f << " (" << n << ")... " << std::flush;
            genFaila(f, n);
            cout << "OK\n";
        }
    }

    // Paleidžiam palyginimą kiekvienam failui
    const int metodas = 1;  // vidurkis
    for (const auto& [failas, kiek] : failai) {
        cout << "\n=== " << failas << " (" << kiek << " irasu) ===\n";
        cout << left << setw(15) << "Konteineris"
            << setw(13) << "Nuskait."
            << setw(13) << "Rusiavim."
            << setw(13) << "S1 naivi"
            << setw(13) << "S3 partit."
            << "\n" << string(70, '-') << "\n";

        pilnasCiklas<std::vector<Studentas>>(failas, metodas, "std::vector");
        pilnasCiklas<MyVector<Studentas>>(failas, metodas, "MyVector");
    }
    return 0;
}