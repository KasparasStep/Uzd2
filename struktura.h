#pragma once
// struktura.h
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
using std::mt19937;
using std::pair;
using std::list;
using std::deque;
namespace fs = std::filesystem;


namespace fs = std::filesystem;

class Studentas {
private:
	string vardas_, pavarde_;
	vector<int> paz_;
	int egz_ = 0;
	double gal_vid_ = 0.0;
	double gal_med_ = 0.0;

	double skaiciuotiVidurki() const;
	double skaiciuotiMediana() const;

public:
	//konstruktoriai
	Studentas() = default;

	Studentas(std::istream& is);
	//Studentas(string v, string p, vector<int> paz, int egz)
	//	: vardas_(move(v)), pavarde_(move(p)), paz_(move(paz)), egz_(egz) {}


//getters
	inline const string& vardas() const { return vardas_; }
	inline const string& pavarde() const { return pavarde_; }
	inline const vector<int>& paz() const { return paz_; }
	inline int egz() const { return egz_; }
	inline double galVid() const { return gal_vid_; }
	inline double galMed() const { return gal_med_; }

	//setters
	inline void setVardas(const string& v) { vardas_ = v; }
	inline void setPavarde(const string& p) { pavarde_ = p; }
	inline void setEgz(int egz) { egz_ = egz; }
	inline void addPaz(int p) { paz_.push_back(p); }
	

	std::istream& readStudent(std::istream& is);
	void apskaiciuoti(int metodas);

	bool compareByVardas(const Studentas& a, const Studentas& b);
	bool compareByPavarde(const Studentas& a, const Studentas& b);
};
