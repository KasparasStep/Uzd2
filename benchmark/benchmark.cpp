// ============================================================
// benchmark.cpp — std::vector vs MyVector spartos analizė
//   1) push_back užpildymo laikas (10k .. 100M)
//   2) atminties perskirstymų skaičius (capacity augimas)
// ============================================================
#include "MyVector.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <Windows.h>

using std::cout;
using namespace std::chrono;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    cout << std::fixed << std::setprecision(5);
    cout << "\n=== UŽPILDYMO LAIKAS (push_back), sekundėmis ===\n";
    cout << std::left << std::setw(15) << "Elementų"
        << std::setw(15) << "std::vector"
        << std::setw(15) << "MyVector"
        << "santykis\n";
    cout << std::string(55, '-') << "\n";

    // SANDBOX: iki 10M, kad netaupytume RAM. Tikrame teste pridėk 100000000.
    const std::vector<unsigned int> dydziai = { 10000u, 100000u, 1000000u, 10000000u, 100000000u };

    for (unsigned int sz : dydziai) {
        // std::vector
        auto t1 = high_resolution_clock::now();
        {
            std::vector<int> v1;
            for (unsigned int i = 1; i <= sz; ++i) v1.push_back(i);
        }
        auto t2 = high_resolution_clock::now();
        double t_std = duration<double>(t2 - t1).count();

        // MyVector
        auto t3 = high_resolution_clock::now();
        {
            MyVector<int> v2;
            for (unsigned int i = 1; i <= sz; ++i) v2.push_back(i);
        }
        auto t4 = high_resolution_clock::now();
        double t_my = duration<double>(t4 - t3).count();

        cout << std::setw(15) << sz
            << std::setw(15) << t_std
            << std::setw(15) << t_my
            << std::setprecision(2) << (t_std > 0 ? t_my / t_std : 0) << "x\n"
            << std::setprecision(5);
    }

    // ====== Perskirstymų skaičius ======
    cout << "\n=== ATMINTIES PERSKIRSTYMŲ SKAIČIUS ===\n";
    // Skaičiuojame, kiek kartų capacity() pasikeičia užpildant N elementų.
    const unsigned int N = 100000000u;  // SANDBOX: 10M (tikrame teste 100000000)

    // std::vector
    {
        std::vector<int> v;
        size_t reallocs = 0;
        size_t last_cap = v.capacity();
        for (unsigned int i = 1; i <= N; ++i) {
            v.push_back(i);
            if (v.capacity() != last_cap) { ++reallocs; last_cap = v.capacity(); }
        }
        cout << "std::vector (" << N << " el.): " << reallocs
            << " perskirstymai, galutinė talpa = " << v.capacity() << "\n";
    }
    // MyVector
    {
        MyVector<int> v;
        size_t reallocs = 0;
        size_t last_cap = v.capacity();
        for (unsigned int i = 1; i <= N; ++i) {
            v.push_back(i);
            if (v.capacity() != last_cap) { ++reallocs; last_cap = v.capacity(); }
        }
        cout << "MyVector     (" << N << " el.): " << reallocs
            << " perskirstymai, galutinė talpa = " << v.capacity() << "\n";
    }

    return 0;
}