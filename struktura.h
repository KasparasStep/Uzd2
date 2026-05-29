// struktura.h — v3.0
#pragma once
//
// Klasių hierarchija:
//   Zmogus (abstrakti bazinė) → Studentas (išvestinė)
//
// v3.0: studentų konteineris dabar MyVector<Studentas> (nuosavas) vietoj std::vector.
// Pažymiai (paz_) lieka std::vector<int> — tai fiksuoto dydžio vidinė detalė,
// o tiriamasis konteineris yra studentų kolekcija.

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
#ifdef _WIN32
#include <windows.h>
#endif

#include "MyVector.h"   // nuosavas konteineris

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
// Zmogus — abstrakti bazinė klasė
// ============================================================

class Zmogus {
protected:
    string vardas_;
    string pavarde_;

public:
    Zmogus() = default;
    Zmogus(string v, string p) : vardas_(move(v)), pavarde_(move(p)) {}

    Zmogus(const Zmogus&) = default;
    Zmogus(Zmogus&&) noexcept = default;
    Zmogus& operator=(const Zmogus&) = default;
    Zmogus& operator=(Zmogus&&) noexcept = default;
    virtual ~Zmogus() = default;

    const string& vardas()  const { return vardas_; }
    const string& pavarde() const { return pavarde_; }
    void setVardas(const string& v) { vardas_ = v; }
    void setPavarde(const string& p) { pavarde_ = p; }

    // Gryna virtuali — daro Zmogų abstrakčia klase
    virtual void spausdinti(ostream& os) const = 0;
};

// ============================================================
// Studentas — išvestinė klasė
// ============================================================

class Studentas : public Zmogus {
private:
    vector<int> paz_;          // namų darbų pažymiai (vidinė detalė)
    int         egz_ = 0;
    double      gal_vid_ = 0.0;
    double      gal_med_ = 0.0;

public:
    Studentas() = default;

    Studentas(string v, string p, vector<int> paz, int egz, int metodas)
        : Zmogus(move(v), move(p)), paz_(move(paz)), egz_(egz) {
        apskaiciuoti(metodas);
    }

    // ---- Rule of Five (su baziniais iškvietimais) ----
    ~Studentas() override = default;

    Studentas(const Studentas& kitas)
        : Zmogus(kitas), paz_(kitas.paz_), egz_(kitas.egz_),
        gal_vid_(kitas.gal_vid_), gal_med_(kitas.gal_med_) {}

    Studentas& operator=(const Studentas& kitas) {
        if (this != &kitas) {
            Zmogus::operator=(kitas);
            paz_ = kitas.paz_;
            egz_ = kitas.egz_;
            gal_vid_ = kitas.gal_vid_;
            gal_med_ = kitas.gal_med_;
        }
        return *this;
    }

    Studentas(Studentas&& kitas) noexcept
        : Zmogus(move(kitas)), paz_(move(kitas.paz_)), egz_(kitas.egz_),
        gal_vid_(kitas.gal_vid_), gal_med_(kitas.gal_med_) {
        kitas.egz_ = 0;
        kitas.gal_vid_ = 0.0;
        kitas.gal_med_ = 0.0;
    }

    Studentas& operator=(Studentas&& kitas) noexcept {
        if (this != &kitas) {
            Zmogus::operator=(move(kitas));
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

    // ---- Virtualios funkcijos perdengimas ----
    void spausdinti(ostream& os) const override {
        os << left << setw(15) << vardas_ << setw(15) << pavarde_;
        os << "ND:";
        for (int p : paz_) os << " " << p;
        os << "  Egz: " << egz_;
        if (gal_vid_ > 0.0) os << fixed << setprecision(2) << "  Vid: " << gal_vid_;
        if (gal_med_ > 0.0) os << fixed << setprecision(2) << "  Med: " << gal_med_;
    }

    // ---- Getteriai ----
    const vector<int>& paz()    const { return paz_; }
    int                egz()    const { return egz_; }
    double             galVid() const { return gal_vid_; }
    double             galMed() const { return gal_med_; }

    // ---- Setteriai ----
    void setEgz(int e) { egz_ = e; }
    void addPazymys(int p) { paz_.push_back(p); }

    void nustatytiEgzIsGalo() {
        if (!paz_.empty()) {
            egz_ = paz_.back();
            paz_.pop_back();
        }
    }

    // ---- Skaičiavimai ----
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
        if (n % 2 == 0) return (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
        return tmp[n / 2];
    }

    void apskaiciuoti(int metodas) {
        if (metodas == 1 || metodas == 3)
            gal_vid_ = skaiciuotiVidurki() * 0.4 + egz_ * 0.6;
        if (metodas == 2 || metodas == 3)
            gal_med_ = skaiciuotiMediana() * 0.4 + egz_ * 0.6;
    }

    friend istream& operator>>(istream& is, Studentas& st);
};

// ============================================================
// I/O operatoriai
// ============================================================

inline ostream& operator<<(ostream& os, const Zmogus& z) {
    z.spausdinti(os);
    return os;
}

inline istream& operator>>(istream& is, Studentas& st) {
    string eilute;
    if (!getline(is, eilute)) return is;
    if (eilute.empty()) return is;

    stringstream ss(eilute);
    string v, p;
    if (!(ss >> v >> p)) return is;

    st.vardas_ = v;
    st.pavarde_ = p;
    st.paz_.clear();
    st.egz_ = 0;
    st.gal_vid_ = 0.0;
    st.gal_med_ = 0.0;

    int n;
    while (ss >> n) st.paz_.push_back(n);
    if (!st.paz_.empty()) {
        st.egz_ = st.paz_.back();
        st.paz_.pop_back();
    }
    return is;
}

// ============================================================
// Laisvų funkcijų prototipai
//
// v3.0: studentų konteineris — MyVector<Studentas>.
// list/deque variantai (stud_List, stud_Deque) lieka su std konteineriais.
// ============================================================

int    gautiSkaiciu(string info, int min, int max);
string genVarda();
string genPavarde(string vardas);
void   genPazymius(vector<int>& paz, int& egz);

void genFaila(const string& failas, int kiek);

void skaitytiVector(const string& failas, MyVector<Studentas>& grupe, int metodas);
void skaitytiList(const string& failas, list  <Studentas>& grupe, int metodas);
void skaitytiDeque(const string& failas, deque <Studentas>& grupe, int metodas);

void skirstytiVector(const MyVector<Studentas>& visi, MyVector<Studentas>& kieti, MyVector<Studentas>& tinginiai);
void skirstytiList(const list  <Studentas>& visi, list  <Studentas>& kieti, list  <Studentas>& tinginiai);
void skirstytiDeque(const deque <Studentas>& visi, deque <Studentas>& kieti, deque <Studentas>& tinginiai);

void skaitytiIsFailo(const string& failas, MyVector<Studentas>& grupe, int metodas);
void spausdintiRezultatus(const MyVector<Studentas>& grupe, int rodyti, const string& failas);
void splitStudents(const MyVector<Studentas>& visi, MyVector<Studentas>& kieti,
    MyVector<Studentas>& tinginiai, int metodas);
void vykdytiVector();

void test1();
void test2(const string& failas, int metodas);