/**
 * @file struktura.h
 * @brief Klasių apibrėžimai: Zmogus (abstrakti bazė) ir Studentas (paveldėjusi).
 * @version 2.0
 *
 * Šiame faile aprašyta visa klasių hierarchija ir laisvų funkcijų prototipai.
 */

#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

/**
 * @class Zmogus
 * @brief Abstrakti bazinė klasė, aprašanti bendrus žmogaus atributus.
 *
 * Klasė yra abstrakti dėl grynosios virtualios funkcijos spausdinti().
 * Negalima sukurti Zmogus tipo objektų tiesiogiai — tik per paveldėjusias klases.
 *
 * @note Destruktorius yra virtualus — būtina paveldėjimo hierarchijoje,
 *       kad ištrindami per bazinę rodyklę išvalytume ir išvestinio objekto laukus.
 */
class Zmogus {
protected:
    string vardas_;   ///< Žmogaus vardas
    string pavarde_;  ///< Žmogaus pavardė

public:
    /// @brief Numatytasis konstruktorius — sukuria tuščią objektą.
    Zmogus() = default;

    /**
     * @brief Pilnas konstruktorius.
     * @param v Vardas
     * @param p Pavardė
     */
    Zmogus(string v, string p)
        : vardas_(move(v)), pavarde_(move(p)) {}

    /// @name Rule of Five
    /// @{
    Zmogus(const Zmogus&) = default;
    Zmogus(Zmogus&&) noexcept = default;
    Zmogus& operator=(const Zmogus&) = default;
    Zmogus& operator=(Zmogus&&) noexcept = default;
    /**
     * @brief Virtualus destruktorius.
     *
     * Būtinas paveldėjimo hierarchijoje — be jo, ištrynus išvestinį
     * objektą per Zmogus* rodyklę, jo papildomi laukai liktų neišvalyti.
     */
    virtual ~Zmogus() = default;
    /// @}

    /// @brief Grąžina vardą.
    const string& vardas()  const { return vardas_; }
    /// @brief Grąžina pavardę.
    const string& pavarde() const { return pavarde_; }

    /// @brief Nustato vardą.
    void setVardas(const string& v) { vardas_ = v; }
    /// @brief Nustato pavardę.
    void setPavarde(const string& p) { pavarde_ = p; }

    /**
     * @brief Gryna virtuali funkcija — daro klasę abstrakčia.
     * @param os Išvesties srautas, į kurį spausdinama.
     *
     * Kiekviena paveldėjusi klasė privalo įgyvendinti šią funkciją.
     */
    virtual void spausdinti(ostream& os) const = 0;
};

/**
 * @class Studentas
 * @brief Paveldėjusi klasė iš Zmogus — atstovauja studentui su pažymiais ir egzaminu.
 *
 * Paveldi vardą ir pavardę iš Zmogus, prideda namų darbų pažymius (paz_),
 * egzamino balą (egz_) ir du galutinio pažymio variantus (vidurkis ir mediana).
 */
class Studentas : public Zmogus {
private:
    vector<int> paz_;     ///< Namų darbų pažymiai
    int         egz_ = 0;     ///< Egzamino balas
    double      gal_vid_ = 0.0;   ///< Galutinis pažymys (vidurkio metodu)
    double      gal_med_ = 0.0;   ///< Galutinis pažymys (medianos metodu)

public:
    /// @brief Numatytasis konstruktorius — sukuria tuščią studentą.
    Studentas() = default;

    /**
     * @brief Pilnas konstruktorius su iš karto atliekamu skaičiavimu.
     * @param v Vardas
     * @param p Pavardė
     * @param paz Namų darbų pažymių vektorius
     * @param egz Egzamino balas
     * @param metodas Skaičiavimo metodas: 1 — vidurkis, 2 — mediana, 3 — abu
     */
    Studentas(string v, string p, vector<int> paz, int egz, int metodas)
        : Zmogus(move(v), move(p)),
        paz_(move(paz)), egz_(egz)
    {
        apskaiciuoti(metodas);
    }

    /// @name Rule of Five
    /// @{

    /// @brief Destruktorius.
    ~Studentas() override = default;

    /**
     * @brief Kopijavimo konstruktorius.
     * @param kitas Originalas, iš kurio kopijuojama
     *
     * Sukuria gilią kopiją — vector ir string laukai nukopijuojami atskirai.
     */
    Studentas(const Studentas& kitas)
        : Zmogus(kitas),
        paz_(kitas.paz_), egz_(kitas.egz_),
        gal_vid_(kitas.gal_vid_), gal_med_(kitas.gal_med_)
    {}

    /**
     * @brief Kopijavimo priskyrimo operatorius.
     * @param kitas Originalas
     * @return Nuoroda į šį objektą
     *
     * Apsaugotas nuo savipriskyrimo (a = a).
     */
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

    /**
     * @brief Perkėlimo konstruktorius (move).
     * @param kitas Objektas, iš kurio perduodami resursai
     *
     * noexcept — leidžia std::vector naudoti šį konstruktorių
     * vietoj kopijavimo realokavimo metu.
     */
    Studentas(Studentas&& kitas) noexcept
        : Zmogus(move(kitas)),
        paz_(move(kitas.paz_)),
        egz_(kitas.egz_), gal_vid_(kitas.gal_vid_), gal_med_(kitas.gal_med_)
    {
        kitas.egz_ = 0;
        kitas.gal_vid_ = 0.0;
        kitas.gal_med_ = 0.0;
    }

    /// @brief Perkėlimo priskyrimo operatorius.
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
    /// @}

    /**
     * @brief Perdengia bazinę spausdinti() funkciją.
     * @param os Išvesties srautas
     *
     * Formatuoja studento duomenis į vieną eilutę su ND, egzaminu ir
     * galutiniais pažymiais (jei jie apskaičiuoti).
     */
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

    /// @brief Grąžina namų darbų pažymių vektorių (const nuoroda).
    const vector<int>& paz()    const { return paz_; }
    /// @brief Grąžina egzamino balą.
    int                egz()    const { return egz_; }
    /// @brief Grąžina galutinį pažymį (vidurkio metodu).
    double             galVid() const { return gal_vid_; }
    /// @brief Grąžina galutinį pažymį (medianos metodu).
    double             galMed() const { return gal_med_; }

    /// @brief Nustato egzamino balą.
    void setEgz(int e) { egz_ = e; }
    /// @brief Prideda vieną pažymį prie ND sąrašo.
    void addPazymys(int p) { paz_.push_back(p); }

    /**
     * @brief Paskutinį pažymį iš sąrašo perkelia į egzamino lauką.
     *
     * Naudojama skaitant iš failo, kur paskutinis skaičius eilutėje
     * laikomas egzamino balu.
     */
    void nustatytiEgzIsGalo() {
        if (!paz_.empty()) {
            egz_ = paz_.back();
            paz_.pop_back();
        }
    }

    /**
     * @brief Apskaičiuoja namų darbų vidurkį.
     * @return Aritmetinis vidurkis arba 0.0 jei pažymių nėra.
     */
    double skaiciuotiVidurki() const {
        if (paz_.empty()) return 0.0;
        double suma = 0.0;
        for (int p : paz_) suma += p;
        return suma / static_cast<double>(paz_.size());
    }

    /**
     * @brief Apskaičiuoja namų darbų medianą.
     * @return Mediana arba 0.0 jei pažymių nėra.
     */
    double skaiciuotiMediana() const {
        if (paz_.empty()) return 0.0;
        vector<int> tmp = paz_;
        sort(tmp.begin(), tmp.end());
        size_t n = tmp.size();
        if (n % 2 == 0) return (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
        return tmp[n / 2];
    }

    /**
     * @brief Apskaičiuoja galutinį pažymį pagal pasirinktą metodą.
     * @param metodas 1 — vidurkis, 2 — mediana, 3 — abu
     *
     * Formulė: galutinis = 0.4 × NDvidurkis + 0.6 × egzaminas
     */
    void apskaiciuoti(int metodas) {
        if (metodas == 1 || metodas == 3)
            gal_vid_ = skaiciuotiVidurki() * 0.4 + egz_ * 0.6;
        if (metodas == 2 || metodas == 3)
            gal_med_ = skaiciuotiMediana() * 0.4 + egz_ * 0.6;
    }

    friend istream& operator>>(istream& is, Studentas& st);
};

/**
 * @brief Išvesties operatorius bet kuriam Zmogus tipui.
 * @param os Išvesties srautas
 * @param z Žmogaus nuoroda (gali būti Studentas)
 * @return Nuoroda į srautą
 *
 * Per virtualų dispatch automatiškai parinks teisingą spausdinti() versiją.
 */
inline ostream& operator<<(ostream& os, const Zmogus& z) {
    z.spausdinti(os);
    return os;
}

/**
 * @brief Įvesties operatorius Studento objektui.
 * @param is Įvesties srautas
 * @param st Studentas, į kurį skaitoma
 * @return Nuoroda į srautą
 *
 * Tikisi vienos eilutės: Vardas Pavardė ND1 ND2 ... NDn Egzaminas
 * Paskutinis skaičius laikomas egzamino balu.
 */
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
// Laisvos funkcijos
// ============================================================

/// @brief Saugiai skaito skaičių iš įvesties; bando iš naujo kol bus įvesta tinkama reikšmė.
int    gautiSkaiciu(string info, int min, int max);

/// @brief Atsitiktinai parenka vardą iš fiksuoto sąrašo.
string genVarda();

/// @brief Pagal vardo galūnę parenka tinkamą pavardę (vyrišką ar moterišką).
string genPavarde(string vardas);

/// @brief Sugeneruoja 20 atsitiktinių pažymių ir egzamino balą.
void   genPazymius(vector<int>& paz, int& egz);

/// @brief Sukuria duomenų failą su nurodytu studentų kiekiu.
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