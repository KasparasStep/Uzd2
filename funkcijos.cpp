//funkcijos.cpp — v3.0
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _WIN32
#include <windows.h>
#endif
#include "struktura.h"

namespace fs = std::filesystem;
static std::mt19937 mt(std::chrono::steady_clock::now().time_since_epoch().count());

// ============================================================
// Generatoriai
// ============================================================

string genVarda() {
    string vardai[] = {
        "Jonas",    "Petras",   "Simas",    "Povilas",  "Mykolas",
        "Tomas",    "Dovydas",  "Matas",    "Lukas",    "Rokas",
        "Evelina",  "Gabija",   "Ieva",     "Greta",    "Sandra",
        "Egle",     "Viktorija","Kamile",   "Vilte",    "Vejune"
    };
    return vardai[mt() % 20];
}

string genPavarde(string vardas) {
    string Vpavardes[] = {
        "Kazlauskas",   "Jankauskas",  "Petrauskas",   "Stankevicius",
        "Vasiliauskas", "Butkus",      "Zukauskas",    "Paulauskas",
        "Urbonas",      "Kavaliauskas"
    };
    string Mpavardes[] = {
        "Kazlauskaite",  "Jankauskaite",  "Petrauskaite",   "Stankeviciute",
        "Vasiliauskaite","Butkute",       "Zukauskaite",    "Paulauskaite",
        "Urbonaite",     "Kavaliauskaite"
    };
    if (!vardas.empty() && vardas.back() == 's')
        return Vpavardes[mt() % 10];
    return Mpavardes[mt() % 10];
}

void genPazymius(vector<int>& paz, int& egz) {
    paz.clear();
    for (int i = 0; i < 20; i++) paz.push_back(mt() % 10 + 1);
    egz = mt() % 10 + 1;
}

// ============================================================
// Failo generavimas
// ============================================================

void genFaila(const string& failas, int kiek) {
    fs::create_directories("Data");
    ofstream out(failas);
    if (!out) throw runtime_error("Nepavyko atidaryti failo: " + failas);

    out << left << setw(15) << "Vardas" << setw(15) << "Pavarde";
    for (int i = 1; i <= 20; i++) out << setw(5) << ("ND" + to_string(i));
    out << setw(5) << "Egz." << "\n";

    for (int i = 1; i <= kiek; i++) {
        out << left << setw(15) << ("Vardas" + to_string(i))
            << setw(15) << ("Pavarde" + to_string(i));
        for (int j = 0; j < 20; j++)
            out << setw(5) << (mt() % 10 + 1);
        out << setw(5) << (mt() % 10 + 1) << "\n";
    }
}

// ============================================================
// Klaidų valymas skaičių įvedimui
// ============================================================

int gautiSkaiciu(string info, int min, int max) {
    int sk;
    while (true) {
        cout << info;
        try {
            if (!(cin >> sk)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Ivestas ne skaicius!");
            }
            if (cin.peek() != '\n' && cin.peek() != ' ' &&
                cin.peek() != '\t' && cin.peek() != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Skaicius negali tureti papildomu simboliu!");
            }
            if (sk < min || sk > max)
                throw out_of_range("Tokio pasirinkimo nera!");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return sk;
        }
        catch (const exception& e) {
            cout << "Klaida: " << e.what() << " Bandykite dar karta.\n";
        }
    }
}

// ============================================================
// Skaitymo funkcijos — Vector versija dabar MyVector<Studentas>
// (list/deque lieka su std konteineriais)
// ============================================================

void skaitytiVector(const string& failas, MyVector<Studentas>& grupe, int metodas) {
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
        st.nustatytiEgzIsGalo();
        st.apskaiciuoti(metodas);
        grupe.push_back(move(st));
    }
}

void skaitytiList(const string& failas, list<Studentas>& grupe, int metodas) {
    ifstream in(failas);
    if (!in) throw runtime_error("Failas nerastas: " + failas);
    string eilute;
    getline(in, eilute);
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

void skaitytiDeque(const string& failas, deque<Studentas>& grupe, int metodas) {
    ifstream in(failas);
    if (!in) throw runtime_error("Failas nerastas: " + failas);
    string eilute;
    getline(in, eilute);
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

// ============================================================
// Skirstymo funkcijos (naivi S1: du nauji konteineriai)
// ============================================================

void skirstytiVector(const MyVector<Studentas>& visi,
    MyVector<Studentas>& kieti, MyVector<Studentas>& tinginiai) {
    for (const auto& s : visi) {
        if (s.galVid() < 5.0) tinginiai.push_back(s);
        else                   kieti.push_back(s);
    }
}

void skirstytiList(const list<Studentas>& visi,
    list<Studentas>& kieti, list<Studentas>& tinginiai) {
    for (const auto& s : visi) {
        if (s.galVid() < 5.0) tinginiai.push_back(s);
        else                   kieti.push_back(s);
    }
}

void skirstytiDeque(const deque<Studentas>& visi,
    deque<Studentas>& kieti, deque<Studentas>& tinginiai) {
    for (const auto& s : visi) {
        if (s.galVid() < 5.0) tinginiai.push_back(s);
        else                   kieti.push_back(s);
    }
}