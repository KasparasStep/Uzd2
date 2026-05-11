// struktura.h
#pragma once

// NOMINMAX būtina prieš windows.h —
// be jos windows.h apibrėžia min/max kaip makrosus ir sugadina
// std::numeric_limits<T>::max() bei std::min/std::max kvietimus.
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <fstream>
#include <sstream>
#include <list>
#include <deque>
#include <filesystem>
#include <windows.h>

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::left;
using std::right;
using std::setw;
using std::endl;
using std::sort;
using std::fixed;
using std::setprecision;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::cerr;
using std::stringstream;
using std::move;
using std::ostream;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::numeric_limits;
using std::mt19937;
using std::chrono::steady_clock;
using std::invalid_argument;
using std::out_of_range;
using std::exception;
using std::runtime_error;
using std::streamsize;
using std::to_string;
using std::pair;
using std::list;
using std::deque;
namespace fs = std::filesystem;

// ============================================================
// Studentas klasė
// Visi laukai yra private — prieinama tik per metodus.
// ============================================================

class Studentas {
private:
    string   vardas_;
    string   pavarde_;
    vector<int> paz_;   // namų darbų pažymiai
    int      egz_ = 0;
    double   gal_vid_ = 0.0;
    double   gal_med_ = 0.0;

public:
    // ---- Konstruktoriai ----

    // 1. Numatytasis konstruktorius
    // Sukuria tuščią studentą su nulinėmis reikšmėmis.
    Studentas() = default;

    // 2. Pilnas konstruktorius
    // Iš karto apskaičiuoja galutinį pažymį pagal metodą.
    Studentas(string v, string p, vector<int> paz, int egz, int metodas)
        : vardas_(move(v)), pavarde_(move(p)),
        paz_(move(paz)), egz_(egz)
    {
        apskaiciuoti(metodas);
    }

   // ==========================================================
   // Rule of Five
   // ==========================================================

    // 3. Destruktorius
    // vector ir string patys išvalo atmintį, todėl explicit logikos nereikia.
    // Parašomas aiškiai — kad būtų matoma, jog jis apsvarstytas.
    ~Studentas() = default;

    // 4. Kopijavimo konstruktorius
    // Sukuria naują objektą kaip tikslią kito kopiją.
    // Kiekvienas laukas nukopijuojamas atskirai — gaunamos dvi nepriklausomos
    // paz_ vektorių kopijos (deep copy).
    Studentas(const Studentas& kitas)
        : vardas_(kitas.vardas_),
        pavarde_(kitas.pavarde_),
        paz_(kitas.paz_),
        egz_(kitas.egz_),
        gal_vid_(kitas.gal_vid_),
        gal_med_(kitas.gal_med_)
    {}

    // 5. Kopijavimo priskyrimo operatorius
    // Pakeičia esamo objekto turinį kito objekto kopija.
    // Patikrina savipriskyrimą (a = a) — be šio patikrinimo
    // galėtume išvalyti savo duomenis prieš nukopijuodami juos.
    Studentas& operator=(const Studentas& kitas) {
        if (this != &kitas) {
            vardas_ = kitas.vardas_;
            pavarde_ = kitas.pavarde_;
            paz_ = kitas.paz_;
            egz_ = kitas.egz_;
            gal_vid_ = kitas.gal_vid_;
            gal_med_ = kitas.gal_med_;
        }
        return *this;
    }

    // 6. Perkėlimo konstruktorius (move)
    // "Pagrobia" kito objekto resursus — vektoriaus atminties blokas
    // perduodamas be kopijavimo. Originalas paliekamas galiojančioje,
    // bet neapibrėžtoje būsenoje (tuščios reikšmės).
    // noexcept — garantuoja, kad konteineriai (pvz. vector) naudos
    // šį konstruktorių vietoj kopijavimo, kai reikalingas realokavimas.
    Studentas(Studentas&& kitas) noexcept
        : vardas_(move(kitas.vardas_)),
        pavarde_(move(kitas.pavarde_)),
        paz_(move(kitas.paz_)),
        egz_(kitas.egz_),
        gal_vid_(kitas.gal_vid_),
        gal_med_(kitas.gal_med_)
    {
        // Paliekame originalą nulinėje būsenoje
        kitas.egz_ = 0;
        kitas.gal_vid_ = 0.0;
        kitas.gal_med_ = 0.0;
    }

    // 7. Perkėlimo priskyrimo operatorius (move assignment)
    // Perduoda resursus iš kito objekto į šį, be kopijavimo.
    // Patikrinamas saviperkėlimas (a = move(a)) — be jo
    // galėtume sunaikinti savo duomenis prieš juos perkeldami.
    Studentas& operator=(Studentas&& kitas) noexcept {
        if (this != &kitas) {
            vardas_ = move(kitas.vardas_);
            pavarde_ = move(kitas.pavarde_);
            paz_ = move(kitas.paz_);
            egz_ = kitas.egz_;
            gal_vid_ = kitas.gal_vid_;
            gal_med_ = kitas.gal_med_;
            kitas.egz_ = 0;
            kitas.gal_vid_ = 0.0;
            kitas.gal_med_ = 0.0;
        }
        return *this;
    }

    // ==========================================================
    // Įvesties / išvesties operatoriai
    // ==========================================================

    // operator<< — išvestis į srautą (ekranas arba failas)
    //
    // Formatuoja studento duomenis į vieną eilutę:
    //   Vardas          Pavardė         ND: 7 8 9 ...  Egz: 8
    //   Vid: 7.60  Med: 8.00
    //
    // Naudojimas:
    //   cout << studentas;
    //   failas << studentas;
    // ---- Getteriai ----

    const string& vardas()  const { return vardas_; }
    const string& pavarde() const { return pavarde_; }
    const vector<int>& paz()     const { return paz_; }
    int                egz()     const { return egz_; }
    double             galVid()  const { return gal_vid_; }
    double             galMed()  const { return gal_med_; }

    // ---- Setteriai ----

    void setVardas(const string& v) { vardas_ = v; }
    void setPavarde(const string& p) { pavarde_ = p; }
    void setEgz(int e) { egz_ = e; }
    void addPazymys(int p) { paz_.push_back(p); }

    // Paskutinį pažymį iš sąrašo perkelia į egzamino lauką.
    // Naudojama skaitant iš failo, kur egzaminas rašomas paskutinis.
    void nustatytiEgzIsGalo() {
        if (!paz_.empty()) {
            egz_ = paz_.back();
            paz_.pop_back();
        }
    }

    // ---- Skaičiavimo metodai ----

    double skaiciuotiVidurki() const {
        if (paz_.empty()) return 0.0;
        double suma = 0.0;
        for (int p : paz_) suma += p;
        return suma / static_cast<double>(paz_.size());
    }

    double skaiciuotiMediana() const {
        if (paz_.empty()) return 0.0;
        vector<int> tmp = paz_;
        sort(tmp.begin(), tmp.end());
        size_t n = tmp.size();
        if (n % 2 == 0)
            return (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
        return tmp[n / 2];
    }

    // Apskaičiuoja galutinį pažymį pagal pasirinktą metodą:
    //   1 — tik vidurkis, 2 — tik mediana, 3 — abu
    void apskaiciuoti(int metodas) {
        if (metodas == 1 || metodas == 3)
            gal_vid_ = skaiciuotiVidurki() * 0.4 + egz_ * 0.6;
        if (metodas == 2 || metodas == 3)
            gal_med_ = skaiciuotiMediana() * 0.4 + egz_ * 0.6;
    }
};

// ============================================================
// Laisvų funkcijų prototipai
// ============================================================

int    gautiSkaiciu(string info, int min, int max);
string genVarda();
string genPavarde(string vardas);
void   genPazymius(vector<int>& paz, int& egz);

void genFaila(const string& failas, int kiek);

void skaitytiVector(const string& failas, vector<Studentas>& grupe, int metodas);
void skaitytiList(const string& failas, list  <Studentas>& grupe, int metodas);
void skaitytiDeque(const string& failas, deque <Studentas>& grupe, int metodas);

void skirstytiVector(const vector<Studentas>& visi, vector<Studentas>& kieti, vector<Studentas>& tinginiai);
void skirstytiList(const list  <Studentas>& visi, list  <Studentas>& kieti, list  <Studentas>& tinginiai);
void skirstytiDeque(const deque <Studentas>& visi, deque <Studentas>& kieti, deque <Studentas>& tinginiai);

void skaitytiIsFailo(const string& failas, vector<Studentas>& grupe, int metodas);
void spausdintiRezultatus(const vector<Studentas>& grupe, int rodyti, const string& failas);
void splitStudents(const vector<Studentas>& visi, vector<Studentas>& kieti,
    vector<Studentas>& tinginiai, int metodas);
void vykdytiVector();

void test1();
void test2(const string& failas, int metodas);