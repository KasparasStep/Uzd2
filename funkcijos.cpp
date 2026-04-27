#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include "struktura.h"

namespace fs = std::filesystem;
static std::mt19937 mt(std::chrono::steady_clock::now().time_since_epoch().count());

string genVarda() {
    string vardai[] = { "Jonas", "Petras", "Simas", "Povilas", "Mykolas", "Tomas", "Dovydas", "Matas", "Lukas", "Rokas",
                        "Evelina", "Gabija", "Ieva", "Greta", "Sandra", "Eglė", "Viktorija", "Kamilė", "Viltė", "Vėjūnė" };
    return vardai[mt() % 20];
}

string genPavarde(string vardas) {
    string Vpavardes[] = { "Kazlauskas", "Jankauskas", "Petrauskas", "Stankevičius", "Vasiliauskas", "Butkus", "Žukauskas", "Paulauskas", "Urbonas", "Kavaliauskas" };
    string Mpavardes[] = { "Kazlauskaitė", "Jankauskaitė", "Petrauskaitė", "Stankevičiūtė", "Vasiliauskaitė", "Butkutė", "Žukauskaitė", "Paulauskaitė", "Urbonaitė", "Kavaliauskaite" };
    if (!vardas.empty() && vardas.back() == 's') {
        return Vpavardes[mt() % 10];
    }
    return Mpavardes[mt() % 10];
}


/*
genFaila veikia greiciau, nei rezultatu spausdinimo funkcija,
nes nereikia formatuoti duomenų ir skaičiuoti galutinių pažymių.
Taip pat, generuojant failą, tiesiog rašome skaičius be papildomų formatavimo operacijų, o
spausdinant rezultatus, kiekvienam studentui reikia apskaičiuoti galutinį pažymį ir
formatuoti išvestį, kas užtrunka daugiau laiko.
Be to, spausdinant didelį kiekį duomenų į konsolę, gali būti lėtesnis procesas nei
rašant į failą.
*/
void genFaila(const string& failas, int kiek) {
    if (!fs::exists("Data")) {
        fs::create_directory("Data");
    }
    string failoVardas = "Data/studentai" + to_string(kiek) + ".txt";
    ofstream out(failas);
    //prideti throw error
    
    out << left << setw(15) << "Vardas" << setw(15) << "Pavardė";
    for (int i = 1; i <= 20; i++) out << setw(5) << ("ND" + to_string(i));
    out << setw(5) << "Egz." << "\n";

    for (int i = 1; i <= kiek; i++) {
        // Šabloniniai vardai
        out << left << setw(15) << ("Vardas" + to_string(i))
            << setw(15) << ("Pavarde" + to_string(i));
        for (int j = 0; j < 20; j++)
            out << setw(5) << (mt() % 10 + 1);
        out << setw(5) << (mt() % 10 + 1) << "\n";
    }
}

void genPazymius(vector<int>& paz, int& egz) {
    paz.clear();
    for (int i = 0; i < 20; i++) paz.push_back(mt() % 10 + 1);
    egz = mt() % 10 + 1;
}

double skaiciuotiVidurki(const vector<int>& paz) {
    if (paz.empty()) return 0.0;
    double suma = 0.0;
    for (int p : paz) suma += p;
    return suma / paz.size();
}

double skaiciuotiMediana(vector<int> paz) {
    if (paz.empty()) return 0.0;
    sort(paz.begin(), paz.end());
    size_t n = paz.size();
    if (n % 2 == 0) return (paz[n / 2 - 1] + paz[n / 2]) / 2.0;
    else return paz[n / 2];
}

void Studentas::apskaiciuoti(int metodas) {
    // Naudojame tiesioginius kintamųjų vardus (su pabraukimais gale)
    if (metodas == 1 || metodas == 3) {
        gal_vid_ = skaiciuotiVidurki() * 0.4 + egz_ * 0.6;
    }
    if (metodas == 2 || metodas == 3) {
        // Kadangi skaiciuotiMediana() grąžina double, tiesiog dauginame
        gal_med_ = skaiciuotiMediana() * 0.4 + egz_ * 0.6;
    }
}

// klaidu valymas skaiciu irasyme
int gautiSkaiciu(string info, int min, int max) {
    int sk;
    while (true) {
        cout << info;
        try {
            if (!(cin >> sk)) {
                cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Įvestas ne skaičius!");
            }
            if (cin.peek() != '\n' && cin.peek() != ' ' &&
                cin.peek() != '\t' && cin.peek() != EOF) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Skaičius negali turėti papildomų simbolių!");
            }
            if (sk < min || sk > max)
                throw out_of_range("Tokio pasirinkimo nėra!");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return sk;
        }
        catch (const exception& e) {
            cout << "Klaida: " << e.what() << " Bandykite dar kartą.\n";
        }
    }
}
 

void skaitytiVector(string failas, vector<Studentas>& grupe, int metodas) {
    ifstream in(failas);
    if (!in) return; // Saugiklis, jei failo nėra

    string line;
    getline(in, line); // Praleisti antrastę
    while (getline(in, line)) {
        stringstream ss(line);
        Studentas st;
        string v, p;
        
        ss >> v >> p;
        st.setVardas(v);
        st.setPavarde(p);

        int balas;
        vector<int> temp_paz;
        while (ss >> balas) {
            temp_paz.push_back(balas);
        }

        if (!temp_paz.empty()) {
            st.setEgz(temp_paz.back()); // paskutinis yra egz
            temp_paz.pop_back();
        }
        for (int n : temp_paz) st.addPaz(n);

		st.apskaiciuoti(metodas);
        grupe.push_back(st);
    }
}
void skaitytiList(string failas, list<Studentas>& grupe, int metodas) {
    ifstream in(failas);
    if (!in) return;

    string line;
    getline(in, line);
    while (getline(in, line)) {
        stringstream ss(line);
        Studentas st;
        string v, p;
        ss >> v >> p;
        st.setVardas(v);
        st.setPavarde(p);
        int val;
        vector<int> temp;
        while (ss >> val) temp.push_back(val);
        if (!temp.empty()) {
            st.setEgz(temp.back());
            temp.pop_back();
        }
        for (int x : temp) st.addPaz(x);
        st.apskaiciuoti(metodas);
        grupe.push_back(st);
    }
}
void skaitytiDeque(string failas, deque<Studentas>& grupe, int metodas) {
    ifstream in(failas);
    if (!in) return;

    string line;
    getline(in, line);
    while (getline(in, line)) {
        stringstream ss(line);
        Studentas st;
        string v, p;
        ss >> v >> p;
        st.setVardas(v);
        st.setPavarde(p);
        int val;
        vector<int> temp;
        while (ss >> val) temp.push_back(val);
        if (!temp.empty()) {
            st.setEgz(temp.back());
            temp.pop_back();
        }
        for (int x : temp) st.addPaz(x);
        st.apskaiciuoti(metodas);
        grupe.push_back(st);
    }
}

// 1 Strategijos skirstymas
void skirstytiVector(const vector<Studentas>& visi, vector<Studentas>& kieti, vector<Studentas>& tinginiai) {
    for (const auto& s : visi) {
        if (s.galVid() < 5.0) tinginiai.push_back(s);
        else kieti.push_back(s);
    }
}
void skirstytiList(const list<Studentas>& visi, list<Studentas>& kieti, list<Studentas>& tinginiai) {
    for (const auto& s : visi) {
        if (s.galVid() < 5.0) tinginiai.push_back(s);
        else kieti.push_back(s);
    }
}
void skirstytiDeque(const deque<Studentas>& visi, deque<Studentas>& kieti, deque<Studentas>& tinginiai) {
    for (const auto& s : visi) {
        if (s.galVid() < 5.0) tinginiai.push_back(s);
        else kieti.push_back(s);
    }
}