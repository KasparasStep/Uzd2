#include "struktura.h"

static const string DATA_DIR = "Data/";

// ============================================================
// Duomenų skaitymas iš failo į MyVector<Studentas>
// ============================================================

void skaitytiIsFailo(const string& failas, MyVector<Studentas>& grupe, int metodas) {
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
    cout << "Duomenys nuskaityti sekmingai.\n";
}

// ============================================================
// Rezultatų išvedimas
// ============================================================

void spausdintiRezultatus(const MyVector<Studentas>& grupe, int rodyti,
    const string& failas) {
    ostream* out = &cout;
    ofstream fout;
    if (!failas.empty()) {
        fout.open(failas);
        if (fout) out = &fout;
    }

    (*out) << left << setw(15) << "Vardas" << setw(15) << "Pavarde";
    if (rodyti == 1 || rodyti == 3) (*out) << setw(20) << "Galutinis (Vid.)";
    if (rodyti == 2 || rodyti == 3) (*out) << setw(20) << "Galutinis (Med.)";
    (*out) << "\n" << string(70, '-') << "\n";

    for (const auto& st : grupe) {
        (*out) << left << setw(15) << st.vardas() << setw(15) << st.pavarde();
        if (rodyti == 1 || rodyti == 3)
            (*out) << fixed << setprecision(2) << setw(20) << st.galVid();
        if (rodyti == 2 || rodyti == 3)
            (*out) << fixed << setprecision(2) << setw(20) << st.galMed();
        (*out) << "\n";
    }
}

// ============================================================
// splitStudents — S1 strategija (naivi, du nauji konteineriai)
// ============================================================

void splitStudents(const MyVector<Studentas>& visi,
    MyVector<Studentas>& kieti,
    MyVector<Studentas>& tinginiai, int metodas) {
    for (const auto& st : visi) {
        double g = (metodas == 2) ? st.galMed() : st.galVid();
        if (g >= 5.0) kieti.push_back(st);
        else          tinginiai.push_back(st);
    }
}

// ============================================================
// Pagrindinis meniu
// ============================================================

void vykdytiVector() {
    MyVector<Studentas> grupe;

    int metodas = gautiSkaiciu(
        "Skaiciavimo metodas:\n1 - Vidurkis\n2 - Mediana\n3 - Abu.\nPasirinkimas: ",
        1, 3);

    cout << "\nAr norite paleisti greicio tyrimus?\n"
        << "1 - Tyrimas 1 (failu kurimas)\n"
        << "2 - Tyrimas 2 (duomenu apdorojimas)\n"
        << "3 - Abu tyrimai\n"
        << "0 - Praleisti\n";
    int tPas = gautiSkaiciu("Pasirinkimas: ", 0, 3);

    if (tPas == 1 || tPas == 3) test1();
    if (tPas == 2 || tPas == 3) test2("", metodas);

    while (true) {
        cout << "\n--- MENIU ---\n"
            << "1 - Irasyti viska ranka\n"
            << "2 - Irasyti vardus ranka, generuoti tik pazymius\n"
            << "3 - Generuoti viska\n"
            << "4 - Nuskaityti is failo\n"
            << "5 - Generuoti studentu failus\n"
            << "0 - Baigti duomenu suvedima ir rikiuoti\n";
        int pas = gautiSkaiciu("Pasirinkimas: ", 0, 5);

        if (pas == 0) break;

        if (pas == 1 || pas == 2) {
            Studentas st;
            string v, p;
            cout << "Iveskite varda: ";   cin >> v;
            cout << "Iveskite pavarde: "; cin >> p;
            st.setVardas(v);
            st.setPavarde(p);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (pas == 1) {
                string input;
                cout << "Iveskite N.D. pazymius (1-10). 'stop' - baigti:\n";
                while (cin >> input && input != "stop") {
                    try {
                        int paz = stoi(input);
                        if (paz >= 1 && paz <= 10) st.addPazymys(paz);
                        else cout << "Tik 1-10!\n";
                    }
                    catch (...) { cout << "Neteisingas skaicius!\n"; }
                }
                st.setEgz(gautiSkaiciu("Iveskite egzamino bala (1-10): ", 1, 10));
            }
            else {
                vector<int> paz;          // laikinas std::vector (genPazymius helper'is)
                int egz;
                genPazymius(paz, egz);
                for (int p2 : paz) st.addPazymys(p2);
                st.setEgz(egz);
                cout << "Sugeneruoti " << st.paz().size() << " pazymiai ir egzaminas.\n";
            }

            st.apskaiciuoti(metodas);
            grupe.push_back(move(st));
        }
        else if (pas == 3) {
            int kiek = gautiSkaiciu("Kiek studentu generuoti? ", 1, 1000000);
            for (int i = 0; i < kiek; i++) {
                Studentas st;
                string vard = genVarda();
                st.setVardas(vard);
                st.setPavarde(genPavarde(vard));
                vector<int> paz;
                int egz;
                genPazymius(paz, egz);
                for (int p2 : paz) st.addPazymys(p2);
                st.setEgz(egz);
                st.apskaiciuoti(metodas);
                grupe.push_back(move(st));
            }
            cout << "Sugeneruota.\n";
        }
        else if (pas == 4) {
            string f;
            cout << "Iveskite failo pavadinima (is Data/ katalogo): ";
            cin >> f;
            try {
                auto s = high_resolution_clock::now();
                skaitytiIsFailo(DATA_DIR + f, grupe, metodas);
                auto e = high_resolution_clock::now();
                cout << "Nuskaityta per: " << fixed << setprecision(4)
                    << duration<double>(e - s).count() << " s\n";
            }
            catch (const exception& e) {
                cout << "Klaida: " << e.what() << " Bandykite dar karta.\n";
            }
        }
        else if (pas == 5) {
            cout << "\nKuri faila generuoti?\n"
                << "1 -      1 000 irasu\n2 -     10 000 irasu\n"
                << "3 -    100 000 irasu\n4 -  1 000 000 irasu\n"
                << "5 - 10 000 000 irasu\n6 - Visus is karto\n";
            int fPas = gautiSkaiciu("Pasirinkimas: ", 1, 6);

            vector<pair<string, int>> failai = {   // konfigūracija — std::vector
                {DATA_DIR + "studentai1k.txt",    1'000},
                {DATA_DIR + "studentai10k.txt",   10'000},
                {DATA_DIR + "studentai100k.txt",  100'000},
                {DATA_DIR + "studentai1M.txt",    1'000'000},
                {DATA_DIR + "studentai10M.txt",   10'000'000}
            };
            fs::create_directories(DATA_DIR);

            if (fPas >= 1 && fPas <= 5) {
                auto& [vardas, kiek] = failai[fPas - 1];
                cout << "Generuojama: " << vardas << "...\n";
                auto t1 = high_resolution_clock::now();
                genFaila(vardas, kiek);
                auto t2 = high_resolution_clock::now();
                cout << "Sugeneruota per: " << fixed << setprecision(4)
                    << duration<double>(t2 - t1).count() << " s\n";
            }
            else {
                for (auto& [vardas, kiek] : failai) {
                    cout << "Generuojama: " << vardas << " (" << kiek << " irasu)...\n";
                    auto t1 = high_resolution_clock::now();
                    genFaila(vardas, kiek);
                    auto t2 = high_resolution_clock::now();
                    cout << "Sugeneruota per: " << fixed << setprecision(4)
                        << duration<double>(t2 - t1).count() << " s\n";
                }
                cout << "Visi failai sugeneruoti.\n";
            }
        }
    }

    if (grupe.empty()) { cout << "Sarasas tuscias.\n"; return; }

    cout << "\nKaip rusiuoti?\n"
        << "1 - Pagal varda\n2 - Pagal pavarde\n3 - Pagal galutini pazymi\n";
    int rPas = gautiSkaiciu("Pasirinkimas: ", 1, 3);

    sort(grupe.begin(), grupe.end(),
        [rPas, metodas](const Studentas& a, const Studentas& b) {
            switch (rPas) {
            case 1:  return a.vardas() < b.vardas();
            case 2:  return a.pavarde() < b.pavarde();
            case 3: {
                double ga = (metodas == 2) ? a.galMed() : a.galVid();
                double gb = (metodas == 2) ? b.galMed() : b.galVid();
                return ga > gb;
            }
            default: return a.pavarde() < b.pavarde();
            }
        });

    cout << "Kur isvesti?\n1 - Ekranas\n2 - Failas\n";
    int kur = gautiSkaiciu("Pasirinkimas: ", 1, 2);
    string fVardas;
    if (kur == 2) { cout << "Failo pavadinimas: "; cin >> fVardas; }
    spausdintiRezultatus(grupe, metodas, fVardas);

    cout << "\nAr skirstyti i dvi grupes? 1 - Taip  2 - Ne\n";
    if (gautiSkaiciu("Pasirinkimas: ", 1, 2) == 1) {
        MyVector<Studentas> kieti, tinginiai;
        splitStudents(grupe, kieti, tinginiai, metodas);
        cout << "Kieti (>= 5.0): " << kieti.size() << "\n";
        cout << "Tinginiai (< 5.0): " << tinginiai.size() << "\n";

        string kF, tF;
        cout << "Kietu failo pavadinimas: ";    cin >> kF;
        cout << "Tinginiu failo pavadinimas: "; cin >> tF;
        spausdintiRezultatus(kieti, metodas, kF);
        spausdintiRezultatus(tinginiai, metodas, tF);
        cout << "Failai sukurti: " << kF << " ir " << tF << "\n";
    }
}