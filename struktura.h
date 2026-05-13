// struktura.h
#pragma once
// struktura.h — v1.5
//
// Klasių hierarchija:
//   Zmogus (abstrakti bazinė klasė)
//      └── Studentas (paveldėjusi)
//
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
using std::istream;
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
using std::stable_partition;
using std::copy_if;
using std::back_inserter;
using std::make_move_iterator;
namespace fs = std::filesystem;

// ============================================================
// Studentas klasė
// Visi laukai yra private — prieinama tik per metodus.
// ============================================================

class Zmogus {
protected:
    // protected — paveldėjusi klasė (Studentas) gali pasiekti tiesiogiai;
    // iš išorės — tik per getter'ius/setter'ius.
    string vardas_;
    string pavarde_;

public:
    // ---- Konstruktoriai ----
    Zmogus() = default;

    Zmogus(string v, string p)
        : vardas_(move(v)), pavarde_(move(p)) {}

   // ---- Rule of Five baziniai metodai ----
   // = default — naudojami kompiliatoriaus generuoti variantai,
   // nes laukai (string) patys tvarkosi su kopijavimu/perkėlimu.
    Zmogus(const Zmogus&) = default;
    Zmogus(Zmogus&&) noexcept = default;
    Zmogus& operator=(const Zmogus&) = default;
    Zmogus& operator=(Zmogus&&) noexcept = default;

    // Virtualus destruktorius — BŪTINA paveldėjimo bazei
    virtual ~Zmogus() = default;

    // ---- Getteriai ----
    const string& vardas()  const { return vardas_; }
    const string& pavarde() const { return pavarde_; }

    // ---- Setteriai ----
    void setVardas(const string& v) { vardas_ = v; }
    void setPavarde(const string& p) { pavarde_ = p; }

    // ---- Gryna virtuali funkcija ----
    // = 0 padaro Zmogų abstrakčia klase.
    // Kiekviena paveldėjusi klasė PRIVALO ją įgyvendinti.
    virtual void spausdinti(ostream& os) const = 0;
};

class Studentas : public Zmogus {
private:
    vector<int> paz_;
    int         egz_ = 0;
    double      gal_vid_ = 0.0;
    double      gal_med_ = 0.0;

public:
    // ==========================================================
    // Konstruktoriai
    // ==========================================================

    Studentas() = default;

    // Pilnas konstruktorius — perduodame v ir p į Zmogaus konstruktorių
    Studentas(string v, string p, vector<int> paz, int egz, int metodas)
        : Zmogus(move(v), move(p)),   // <-- iškviečiamas bazinis konstruktorius
        paz_(move(paz)), egz_(egz)
    {
        apskaiciuoti(metodas);
    }

    // ==========================================================
    // Rule of Five
    // Kiekviename privalu IŠKVIESTI bazinį (Zmogus) variantą,
    // kad vardas ir pavardė būtų teisingai nukopijuoti/perkelti.
    // ==========================================================

    // Destruktorius — override žymi, kad perdengiame bazinį virtualų
    ~Studentas() override = default;

    // Kopijavimo konstruktorius
    Studentas(const Studentas& kitas)
        : Zmogus(kitas),                  // <-- bazinis copy ctor
        paz_(kitas.paz_),
        egz_(kitas.egz_),
        gal_vid_(kitas.gal_vid_),
        gal_med_(kitas.gal_med_)
    {}

    // Kopijavimo priskyrimas
    Studentas& operator=(const Studentas& kitas) {
        if (this != &kitas) {
            Zmogus::operator=(kitas);     // <-- bazinis copy assignment
            paz_ = kitas.paz_;
            egz_ = kitas.egz_;
            gal_vid_ = kitas.gal_vid_;
            gal_med_ = kitas.gal_med_;
        }
        return *this;
    }

    // Perkėlimo konstruktorius
    Studentas(Studentas&& kitas) noexcept
        : Zmogus(move(kitas)),            // <-- bazinis move ctor
        paz_(move(kitas.paz_)),
        egz_(kitas.egz_),
        gal_vid_(kitas.gal_vid_),
        gal_med_(kitas.gal_med_)
    {
        kitas.egz_ = 0;
        kitas.gal_vid_ = 0.0;
        kitas.gal_med_ = 0.0;
    }

    // Perkėlimo priskyrimas
    Studentas& operator=(Studentas&& kitas) noexcept {
        if (this != &kitas) {
            Zmogus::operator=(move(kitas)); // <-- bazinis move assignment
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
    // Virtualios funkcijos perdengimas
    // ==========================================================

    // override — kompiliatorius patikrina, kad funkcija TIKRAI
    // perdengia bazinę (jei užklysta klaida, neleis kompiliuoti).
    void spausdinti(ostream& os) const override {
        os << left << setw(15) << vardas_
            << setw(15) << pavarde_;
        os << "ND:";
        for (int p : paz_) os << " " << p;
        os << "  Egz: " << egz_;
        if (gal_vid_ > 0.0)
            os << fixed << setprecision(2) << "  Vid: " << gal_vid_;
        if (gal_med_ > 0.0)
            os << fixed << setprecision(2) << "  Med: " << gal_med_;
    }

   
    // ==========================================================
    // Getteriai (papildomi prie paveldėtų iš Zmogus)
    // ==========================================================

    const vector<int>& paz()    const { return paz_; }
    int                egz()    const { return egz_; }
    double             galVid() const { return gal_vid_; }
    double             galMed() const { return gal_med_; }

    // ==========================================================
    // Setteriai (papildomi)
    // ==========================================================

    void setEgz(int e) { egz_ = e; }
    void addPazymys(int p) { paz_.push_back(p); }

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