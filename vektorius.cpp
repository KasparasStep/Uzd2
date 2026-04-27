#include "struktura.h"
#include "funkcijos.h"

static const string DATA_DIR = "Data/";
// ============================================================
// Duomenų skaitymas iš failo į vector
// ============================================================
void skaitytiIsFailo(const string& failas, vector<Studentas>& grupe, int metodas) {
    ifstream in(failas);
    if (!in) throw runtime_error("Failas nerastas: " + failas);

    string eilute;
    getline(in, eilute); // praleisti antraštę

    while (getline(in, eilute)) {
        if (eilute.empty()) continue;

        stringstream ss(eilute);
        Studentas st;
        string v, p;

        // 1. Skaitome vardą ir pavardę į laikinus string kintamuosius
        if (!(ss >> v >> p)) continue;
        st.setVardas(v);
        st.setPavarde(p);

        int balas;
        vector<int> tempPaz;

        // 2. Skaitome visus skaičius į laikiną vektorių
        while (ss >> balas) {
            tempPaz.push_back(balas);
        }

        // 3. Jei radome skaičių, atskiriame egzaminą nuo pažymių
        if (!tempPaz.empty()) {
            st.setEgz(tempPaz.back()); // Paskutinis yra egzaminas
            tempPaz.pop_back();        // Pašaliname egzaminą iš pažymių sąrašo

            // Visus likusius skaičius sudedame kaip pažymius
            for (int n : tempPaz) {
                st.addPaz(n);
            }

            // 4. Skaičiuojame balus naudodami KLASĖS metodą
            st.apskaiciuoti(metodas);

            // 5. Išsaugome objektą grupėje (move paspartina procesą)
            grupe.push_back(move(st));
        }
    }
    cout << "Duomenys nuskaityti sėkmingai.\n";
}
// Rezultatų išvedimas (į ekraną arba failą)
 

// ============================================================
// Rezultatų išvedimas (į ekraną arba failą)
// ============================================================
void spausdintiRezultatus(const vector<Studentas>& grupe, int rodyti, const string& failas) {
    ostream* out = &cout;
    ofstream fout;

    if (!failas.empty()) {
        fout.open(failas);
        if (fout) out = &fout;
    }

    (*out) << left << setw(15) << "Vardas" << setw(15) << "Pavardė";
    if (rodyti == 1 || rodyti == 3) (*out) << setw(20) << "Galutinis (Vid.)";
    if (rodyti == 2 || rodyti == 3) (*out) << setw(20) << "Galutinis (Med.)";
    (*out) << endl << string(70, '-') << endl;

    for (const auto& st : grupe) {
        // Naudojame GETERIUS vardas() ir pavarde()
        (*out) << left << setw(15) << st.vardas() << setw(15) << st.pavarde();

        if (rodyti == 1 || rodyti == 3)
            (*out) << fixed << setprecision(2) << setw(20) << st.galVid(); // Naudojame GETERĮ
        if (rodyti == 2 || rodyti == 3)
            (*out) << fixed << setprecision(2) << setw(20) << st.galMed(); // Naudojame GETERĮ
        (*out) << endl;
    }
}

// ============================================================
// Skaidymas į dvi grupes (1 strategija: du nauji konteineriai)
// ============================================================
void splitStudents(const vector<Studentas>& visi,
    vector<Studentas>& kieti,
    vector<Studentas>& tinginiai, int metodas) {

    // Lambda funkcija, nustatanti, kurį galutinį balą imti (naudojant geterius)
    auto galutinis = [&](const Studentas& st) {
        return (metodas == 2) ? st.galMed() : st.galVid();
        };

    copy_if(visi.begin(), visi.end(), back_inserter(kieti),
        [&](const Studentas& st) { return galutinis(st) >= 5.0; });

    copy_if(visi.begin(), visi.end(), back_inserter(tinginiai),
        [&](const Studentas& st) { return galutinis(st) < 5.0; });
}

// ============================================================
// Pagrindinis meniu (senoji v0.4 programa)
// ============================================================
void vykdytiVector() {
    vector<Studentas> grupe;

    int metodas = gautiSkaiciu(
        "Skaičiavimo metodas:\n1 - Vidurkis\n2 - Mediana\n3 - Abu.\nPasirinkimas: ", 1, 3);

    cout << "\nAr norite paleisti greičio tyrimus?\n";
    cout << "1 - Tyrimas 1 (failų kūrimas)\n";
    cout << "2 - Tyrimas 2 (duomenų apdorojimas)\n";
    cout << "3 - Abu tyrimai\n";
    cout << "0 - Praleisti\n";
    int tPas = gautiSkaiciu("Pasirinkimas: ", 0, 3);

    if (tPas == 1 || tPas == 3) test1();
    if (tPas == 2 || tPas == 3) test2("", metodas);

    while (true) {
        cout << "\n--- MENIU ---\n";
        cout << "1 - Įrašyti viską ranka\n";
        cout << "2 - Įrašyti vardus ranka, generuoti tik pažymius\n";
        cout << "3 - Generuoti viską\n";
        cout << "4 - Nuskaityti iš failo\n";
        cout << "5 - Generuoti studentų failus\n";
        cout << "0 - Baigti duomenų suvedimą ir rikiuoti\n";
        int pas = gautiSkaiciu("Pasirinkimas: ", 0, 5);

        if (pas == 0) break;

        if (pas == 1 || pas == 2) {
            Studentas st;
            string v, p;

            cout << "Įveskite vardą: "; cin >> v;
            st.setVardas(v); // Naudojame seterį

            cout << "Įveskite pavardę: "; cin >> p;
            st.setPavarde(p); // Naudojame seterį

            if (pas == 1) {
                string input;
                cout << "Įveskite N.D. pažymius (1-10). 'stop' - baigti:\n";
                while (cin >> input && input != "stop") {
                    try {
                        int paz = stoi(input);
                        if (paz >= 1 && paz <= 10) st.addPaz(paz); // Naudojame metodą priedui
                        else cout << "Tik 1-10!\n";
                    }
                    catch (...) { cout << "Neteisingas skaičius!\n"; }
                }
                int e = gautiSkaiciu("Įveskite egzamino balą (1-10): ", 1, 10);
                st.setEgz(e); // Naudojame seterį
            }
            else {
                // Kadangi tavo genPazymius nori vectoriaus ir int adresų, geriau naudoti laikinus
                vector<int> tempPaz;
                int tempEgz;
                genPazymius(tempPaz, tempEgz);

                for (int paz : tempPaz) st.addPaz(paz);
                st.setEgz(tempEgz);

                cout << "Sugeneruota pažymių ir egzaminas.\n";
            }
            st.apskaiciuoti(metodas); // Pakeistas iš išorinės funkcijos į metodą
            grupe.push_back(move(st));
        }
        else if (pas == 3) {
            int kiek = gautiSkaiciu("Kiek studentų generuoti? ", 1, 1000000);
            for (int i = 0; i < kiek; i++) {
                Studentas st;
                st.setVardas(genVarda());
                st.setPavarde(genPavarde(st.vardas())); // Naudojame vardas() gauti, ką tik įdėjome

                vector<int> tempPaz;
                int tempEgz;
                genPazymius(tempPaz, tempEgz);

                for (int paz : tempPaz) st.addPaz(paz);
                st.setEgz(tempEgz);

                st.apskaiciuoti(metodas);
                grupe.push_back(move(st));
            }
            cout << "Sugeneruota.\n";
        }
        else if (pas == 4) {
            string f;
            cout << "Įveskite failo pavadinimą (iš Data/ katalogo): ";
            cin >> f;
            auto s = high_resolution_clock::now();
            skaitytiIsFailo(DATA_DIR + f, grupe, metodas);
            auto e = high_resolution_clock::now();
            cout << "Nuskaityta per: " << duration<double>(e - s).count() << " s\n";
        }
        else if (pas == 5) {
            // (Nepakeista, failų generavimas nepriklauso nuo Studentas klasės)
            cout << "\nKurį failą generuoti?\n";
            cout << "1 -      1 000 įrašų\n2 -     10 000 įrašų\n";
            cout << "3 -    100 000 įrašų\n4 -  1 000 000 įrašų\n";
            cout << "5 - 10 000 000 įrašų\n6 - Visus iš karto\n";
            int fPas = gautiSkaiciu("Pasirinkimas: ", 1, 6);

            vector<pair<string, int>> failai = {
                {DATA_DIR + "studentai1k.txt",    1000},
                {DATA_DIR + "studentai10k.txt",   10000},
                {DATA_DIR + "studentai100k.txt",  100000},
                {DATA_DIR + "studentai1M.txt",    1000000},
                {DATA_DIR + "studentai10M.txt",   10000000}
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
                    cout << "Generuojama: " << vardas << " (" << kiek << " įrašų)...\n";
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

    if (grupe.empty()) { cout << "Sąrašas tuščias.\n"; return; }

    cout << "\nKaip rūšiuoti?\n1 - Pagal vardą\n2 - Pagal pavardę\n3 - Pagal galutinį pažymį\n";
    int rPas = gautiSkaiciu("Pasirinkimas: ", 1, 3);

    // Rūšiavimas taip pat naudoja GETERIUS
    sort(grupe.begin(), grupe.end(), [rPas, metodas](const Studentas& a, const Studentas& b) {
        switch (rPas) {
        case 1: return a.vardas() < b.vardas();
        case 2: return a.pavarde() < b.pavarde();
        case 3: {
            double ga = (metodas == 2) ? a.galMed() : a.galVid();
            double gb = (metodas == 2) ? b.galMed() : b.galVid();
            return ga > gb; // Didesnis balas eina pirmas
        }
        default: return a.pavarde() < b.pavarde();
        }
        });


    cout << "Kur išvesti?\n1 - Ekranas\n2 - Failas\n";
    int kur = gautiSkaiciu("Pasirinkimas: ", 1, 2);
    string fVardas;
    if (kur == 2) { cout << "Failo pavadinimas: "; cin >> fVardas; }
    spausdintiRezultatus(grupe, metodas, fVardas);

    cout << "\nAr skirstyti į dvi grupes? 1 - Taip  2 - Ne\n";
    if (gautiSkaiciu("Pasirinkimas: ", 1, 2) == 1) {
        vector<Studentas> kieti, tinginiai;
        splitStudents(grupe, kieti, tinginiai, metodas);
        cout << "Kieti (>= 5.0): " << kieti.size() << "\n";
        cout << "Tinginiai (< 5.0): " << tinginiai.size() << "\n";

        string kF, tF;
        cout << "Kietų failo pavadinimas: ";   cin >> kF;
        cout << "Tinginių failo pavadinimas: "; cin >> tF;
        spausdintiRezultatus(kieti, metodas, kF);
        spausdintiRezultatus(tinginiai, metodas, tF);
        cout << "Failai sukurti: " << kF << " ir " << tF << "\n";
    }
}