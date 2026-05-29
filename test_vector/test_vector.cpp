// Greitas testas: ar Vector elgiasi taip pat kaip std::vector

#include "Vector.h"
#include <vector>
#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>

static int praejo = 0, nepraejo = 0;
static void check(const std::string& v, bool ok) {
    std::cout << (ok ? "  [PASS] " : "  [FAIL] ") << v << "\n";
    ok ? ++praejo : ++nepraejo;
}

int main() {
    // 1. push_back + size + operator[]
    {
        Vector<int> v;
        std::vector<int> s;
        for (int i = 0; i < 100; ++i) { v.push_back(i); s.push_back(i); }
        bool ok = (v.size() == s.size());
        for (size_t i = 0; i < v.size(); ++i) if (v[i] != s[i]) ok = false;
        check("push_back / size / operator[]", ok);
    }
    // 2. at() su ribu tikrinimu
    {
        Vector<int> v{ 1,2,3 };
        bool threw = false;
        try { v.at(10); }
        catch (const std::out_of_range&) { threw = true; }
        check("at() meta out_of_range", threw && v.at(1) == 2);
    }
    // 3. initializer_list + front/back
    {
        Vector<int> v{ 10, 20, 30 };
        check("initializer_list + front/back",
            v.front() == 10 && v.back() == 30 && v.size() == 3);
    }
    // 4. copy ctor — gili kopija
    {
        Vector<int> a{ 1,2,3 };
        Vector<int> b(a);
        b[0] = 99;
        check("kopijavimo ktorius (gili kopija)", a[0] == 1 && b[0] == 99);
    }
    // 5. move ctor
    {
        Vector<int> a{ 1,2,3 };
        Vector<int> b(std::move(a));
        check("perkelimo ktorius", b.size() == 3 && a.size() == 0);
    }
    // 6. copy assignment
    {
        Vector<int> a{ 1,2,3 }, b;
        b = a;
        check("kopijavimo priskyrimas", b.size() == 3 && b[2] == 3);
    }
    // 7. move assignment
    {
        Vector<int> a{ 1,2,3 }, b;
        b = std::move(a);
        check("perkelimo priskyrimas", b.size() == 3 && a.size() == 0);
    }
    // 8. insert
    {
        Vector<int> v{ 1,2,4 };
        std::vector<int> s{ 1,2,4 };
        v.insert(v.begin() + 2, 3);
        s.insert(s.begin() + 2, 3);
        bool ok = v.size() == s.size();
        for (size_t i = 0; i < v.size(); ++i) if (v[i] != s[i]) ok = false;
        check("insert viduryje", ok);  // turi buti {1,2,3,4}
    }
    // 9. erase
    {
        Vector<int> v{ 1,2,3,4,5 };
        std::vector<int> s{ 1,2,3,4,5 };
        v.erase(v.begin() + 1);
        s.erase(s.begin() + 1);
        bool ok = v.size() == s.size();
        for (size_t i = 0; i < v.size(); ++i) if (v[i] != s[i]) ok = false;
        check("erase vieno elemento", ok);  // {1,3,4,5}
    }
    // 10. erase intervalo
    {
        Vector<int> v{ 1,2,3,4,5 };
        v.erase(v.begin() + 1, v.begin() + 4);
        check("erase intervalo", v.size() == 2 && v[0] == 1 && v[1] == 5);
    }
    // 11. resize
    {
        Vector<int> v{ 1,2,3 };
        v.resize(5, 9);
        check("resize didinimas", v.size() == 5 && v[4] == 9);
        v.resize(2);
        check("resize mazinimas", v.size() == 2 && v[1] == 2);
    }
    // 12. reserve + capacity
    {
        Vector<int> v;
        v.reserve(100);
        check("reserve + capacity", v.capacity() >= 100 && v.size() == 0);
    }
    // 13. pop_back + clear
    {
        Vector<int> v{ 1,2,3 };
        v.pop_back();
        check("pop_back", v.size() == 2 && v.back() == 2);
        v.clear();
        check("clear", v.empty() && v.size() == 0);
    }
    // 14. iteratoriai + STL algoritmas
    {
        Vector<int> v{ 5,3,1,4,2 };
        std::sort(v.begin(), v.end());
        check("std::sort su Vector iteratoriais",
            v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4 && v[4] == 5);
        int sum = std::accumulate(v.begin(), v.end(), 0);
        check("std::accumulate su Vector", sum == 15);
    }
    // 15. reverse iteratoriai
    {
        Vector<int> v{ 1,2,3 };
        std::string r;
        for (auto it = v.rbegin(); it != v.rend(); ++it) r += std::to_string(*it);
        check("reverse iteratoriai", r == "321");
    }
    // 16. emplace_back
    {
        Vector<std::string> v;
        v.emplace_back("labas");
        v.emplace_back(3, 'x');  // "xxx"
        check("emplace_back", v.size() == 2 && v[0] == "labas" && v[1] == "xxx");
    }
    // 17. palyginimo operatoriai
    {
        Vector<int> a{ 1,2,3 }, b{ 1,2,3 }, c{ 1,2,4 };
        check("operator== / !=", a == b && a != c);
        check("operator< / >", a < c && c > a);
    }
    // 18. swap
    {
        Vector<int> a{ 1,2,3 }, b{ 9,8 };
        swap(a, b);
        check("swap", a.size() == 2 && a[0] == 9 && b.size() == 3 && b[0] == 1);
    }
    // 19. veikia su netrivialiu tipu (string)
    {
        Vector<std::string> v;
        for (int i = 0; i < 50; ++i) v.push_back("studentas" + std::to_string(i));
        check("Vector<string> push_back 50x", v.size() == 50 && v[49] == "studentas49");
    }

    std::cout << "\n=== " << praejo << " PASS, " << nepraejo << " FAIL ===\n";
    return nepraejo == 0 ? 0 : 1;
}