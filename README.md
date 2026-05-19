# Studentų pažymių programa - v2.0

C++17 programa studentų pažymiams skaičiuoti, rūšiuoti ir grupuoti.
Naudojamos OOP konstrukcijos: paveldėjimas, polimorfizmas, Rule of Five.

---

## Įdiegimas

### Reikalavimai

- C++17 kompiliatorius (`g++` 9+, `clang++` 9+, MSVC 2019+)
- CMake 3.14+ *(rekomenduojama)* arba `make`
- *(neprivaloma)* Doxygen + LaTeX dokumentacijai generuoti
- *(neprivaloma)* `catch.hpp` unit testams

### Kompiliavimas su CMake

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Kompiliavimas su Makefile (Unix)

```bash
make             # sukompiliuoja viską
make Uzd2        # tik pagrindinę programą
make testas      # tik paprastus testus
make unit_testai # Catch2 testus (reikia catch.hpp)
make docs        # Doxygen dokumentaciją
make clean       # ištrina sukompiliuotus failus
```

### Kompiliavimas rankiniu būdu

```bash
g++ -O2 -std=c++17 Uzd2.cpp vektorius.cpp testavimas.cpp funkcijos.cpp -o Uzd2
```

---

## Naudojimas

### Pagrindinė programa

```bash
./Uzd2
```

Interaktyvus meniu - leidžia įvesti studentus ranka, generuoti, skaityti iš failo, rūšiuoti ir skaidyti į grupes.

### Tyrimo programos

```bash
./stud_Vector   # sugeneruoja Data/ ir matuoja vector greitį
./stud_List     # naudoja tuos pačius Data/ failus
./stud_Deque    # naudoja tuos pačius Data/ failus
```

### Testai

```bash
./testas         # paprastas rankinis testas (v1.2/v1.5)
./unit_testai    # pilnas Catch2 unit test rinkinys (v2.0)
```

### Dokumentacijos generavimas

```bash
doxygen Doxyfile

# HTML - atidaromas naršyklėje
open docs/html/index.html        # macOS
xdg-open docs/html/index.html    # Linux
start docs/html/index.html       # Windows

# PDF - kompiliuojamas iš LaTeX
cd docs/latex
make             # arba: pdflatex refman.tex
```

Jei nėra `pdflatex` lokaliai - įkelkite `docs/latex/*` į [Overleaf](https://www.overleaf.com/) ir sukompiliuokite ten.

---

## Failų struktūra

```
.
+-- struktura.h         - klasės (Zmogus, Studentas), Doxygen komentarai
+-- funkcijos.cpp       - pagalbinės funkcijos
+-- vektorius.cpp       - meniu logika
+-- testavimas.cpp      - greičio tyrimas
+-- Uzd2.cpp            - pagrindinis įėjimas
+-- stud_Vector.cpp     - konteinerio tyrimas (vector)
+-- stud_List.cpp       - konteinerio tyrimas (list)
+-- stud_Deque.cpp      - konteinerio tyrimas (deque)
+-- testas.cpp          - paprastas rankinis testas
+-- unit_testai.cpp     - Catch2 unit testai
+-- CMakeLists.txt      - build konfigūracija
+-- Makefile            - alternatyvus build (Unix)
+-- Doxyfile            - Doxygen konfigūracija
+-- .gitignore
+-- README.md
```

---

## Releasai

### v0.1 - pradinė versija
Struct `Studentas`, rankinis įvedimas, vidurkio/medianos skaičiavimas.

### v0.2
Skaitymas iš failo, išvedimas į failą.

### v0.3
Automatinis duomenų generavimas, klaidų apdorojimas.

### v0.4
Skaidymas į kietus/tinginius, failo kūrimo ir apdorojimo tyrimai.

### v1.0
Trys atskiros tyrimo programos (`vector`, `list`, `deque`) su dviem skaidymo strategijomis (S1, S3).

### v1.1
Perėjimas nuo `struct` prie `class`. Privatūs laukai, getter'iai, setter'iai.

### v1.2 - Rule of Five + I/O operatoriai
- Realizuota pilna penkių metodų taisyklė
- Perdengti `operator<<` ir `operator>>`
- Pridėtas testavimo failas `testas.cpp`

### v1.5 - Paveldėjimas
- Sukurta abstrakti bazinė klasė `Zmogus`
- `Studentas` paveldi iš `Zmogus`
- Demonstruojamas virtualus dispatch ir polimorfizmas

### v2.0 - Dokumentacija + Unit testai
- Doxygen dokumentacija (HTML + PDF)
- Catch2 unit testai
- Švari repozitorija (`.gitignore`)

---

## v1.2 - Rule of Five paaiškinimas

Kai klasė valdo resursus (vector, string), reikia aprašyti penkis metodus:

| # | Metodas | Funkcija |
|---|---|---|
| 1 | `~Studentas()` | Atlaisvina atmintį |
| 2 | `Studentas(const Studentas&)` | Gili kopija |
| 3 | `operator=(const Studentas&)` | Kopijavimo priskyrimas |
| 4 | `Studentas(Studentas&&) noexcept` | Move - be kopijavimo |
| 5 | `operator=(Studentas&&) noexcept` | Move priskyrimas |

### I/O operatoriai

| Operatorius | Paskirtis | Pavyzdys |
|---|---|---|
| `operator<<` | Išvedimas į ekraną/failą | `cout << st;` `failas << st;` |
| `operator>>` | Skaitymas iš klaviatūros/failo | `cin >> st;` `failas >> st;` |

Formatas `operator>>`: `Vardas Pavardė ND1 ND2 ... NDn Egzaminas` *(paskutinis skaičius = egzaminas)*

### Duomenų įvedimo būdai

| Būdas | Kaip | Vidinis mechanizmas |
|---|---|---|
| Rankinis | Meniu -> 1 | `cin >>`, `gautiSkaiciu()` |
| Automatinis | Meniu -> 3 | `genVarda()`, `genPazymius()` |
| Iš failo | Meniu -> 4 | `operator>>` |

### Duomenų išvedimo būdai

| Būdas | Kaip | Vidinis mechanizmas |
|---|---|---|
| Į ekraną | Po rūšiavimo | `cout << st` per `spausdintiRezultatus()` |
| Į failą | Po rūšiavimo | `ofstream << st` per `spausdintiRezultatus()` |

---

## v1.5 - Klasių hierarchija

```
Zmogus  (abstrakti)
   |
   +-- Studentas
```

### Kodėl Zmogus abstrakti

Klasėje yra gryna virtuali funkcija:
```cpp
virtual void spausdinti(ostream& os) const = 0;
```
`= 0` neleidžia sukurti `Zmogus z;` - kompiliatorius išmes klaidą.

### Kritinis virtualus destruktorius

```cpp
virtual ~Zmogus() = default;  // BŪTINA paveldėjime
```

Be `virtual`, ištrinant per bazinę rodyklę (`delete zmogus_ptr`), iškviestumėme tik bazinį destruktorių - Studento `vector` ir kt. liktų neišvalyti.

### Polimorfizmas

```cpp
vector<unique_ptr<Zmogus>> zmones;
zmones.push_back(make_unique<Studentas>(...));

for (const auto& z : zmones)
    cout << *z;   // virtual dispatch -> Studentas::spausdinti
```

---

## v2.0 - Unit testai

Naudojama [Catch2](https://github.com/catchorg/Catch2) - populiarus C++ testavimo framework'as (single-header).

### Setup

```bash
# Atsisiųskite catch.hpp į projekto katalogą:
wget https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp
```

### Sintaksė

```cpp
TEST_CASE("aprasymas", "[tag1][tag2]") {
    Studentas st;
    REQUIRE(st.egz() == 0);   // testas FAIL jei nepavyksta
    CHECK(st.vardas() == ""); // tęsia net jei nepavyksta
    
    SECTION("atskira testo dalis") {
        // ...
    }
}
```

### Padengiamos sritys

| TEST_CASE | Tag'as | Aprašas |
|---|---|---|
| Numatytasis konstruktorius | `[konstruktoriai]` | Tikrina tuščio objekto reikšmes |
| Pilnas konstruktorius | `[konstruktoriai]` | Skaičiavimas iš pradinių duomenų |
| Copy constructor | `[rule_of_five][copy]` | Gili kopija |
| Copy assignment | `[rule_of_five][copy]` | Savipriskyrimas |
| Move constructor | `[rule_of_five][move]` | Originalo ištuštinimas |
| Move assignment | `[rule_of_five][move]` | Saviperkėlimas |
| Destruktorius | `[rule_of_five]` | Automatinis atminties valymas |
| Zmogus abstraktumas | `[abstrakti]` | `static_assert` patvirtina |
| Virtualus dispatch | `[abstrakti][polimorfizmas]` | Per Zmogus& nuorodą |
| operator<< | `[io]` | Visi laukai išvedami |
| operator>> | `[io]` | Formato analizė |
| Skaičiavimai | `[skaiciavimas]` | Vidurkis, mediana |
| Kraštiniai atvejai | `[krastiniai_atvejai]` | Tuščias studentas |

### Paleidimas

```bash
./unit_testai             # visi testai
./unit_testai -s          # rodo ir praėjusius
./unit_testai "[copy]"    # tik tam tikras tag'ų grupes
```

---

## Konteinerių tyrimo rezultatai

Testavimo sistema: *(ASUS VIVOBOOK S 14)*

| Parametras 	 | Reikšmė 		  |
|----------------|----------------|
| CPU        	 | AMD Ryzen AI 9 |
| RAM        	 | 24 GB 		  |
| Saugykla   	 | 1 TB SSD (NVMe)|
| OS         	 | Windows 11	  |
| Kompiliatorius | g++ 		  	  |

Žemiau pateikti rezultatai, gauti testavimo metu (su release):

### Nuskaitymas (s)

| Failas          	  | vector  | list    | deque  |
|---------------------|---------|---------|--------|
| 1 000 įrašų     	  | 0.0050  | 0.0042  | 0.0050 |
| 10 000 įrašų    	  | 0.0411  | 0.0407  | 0.0406 |
| 100 000 įrašų   	  | 0.3905  | 0.3891  | 0.3811 |
| 1 000 000 įrašų     | 3.7790  | 3.7911  | 3.7534 |
| 10 000 000 įrašų    | 38.2739 | 50.6590 | 49.0184 |

Nuskaitymas visur panašus — skirtumai mažesni nei failo I/O triukšmas.

### Rūšiavimas (s)

| Failas          	  | vector | list    | deque  |
|---------------------|--------|---------|--------|
| 1 000 įrašų     	  | 0.0001 | 0.0000  | 0.0001 |
| 10 000 įrašų    	  | 0.0009 | 0.0007  | 0.0011 |
| 100 000 įrašų   	  | 0.0073 | 0.0193  | 0.0128 |
| 1 000 000 įrašų     | 0.0855 | 0.5396  | 0.2673 |
| 10 000 000 įrašų    | 0.7860 | 12.2730 | 4.2492 |

`std::vector` rūšiuojamas greičiausiai dėl gretimos atminties (CPU talpykla). `list` yra ~2.5× lėtesnis 100k atveju. `deque` artimas vektoriui.


## Skaidymo strategijos

### S1 — du nauji konteineriai (`copy_if`)

### Skaidymas (s)

| Failas          	  | vector | list   | deque  |
|---------------------|--------|--------|--------|
| 1 000 įrašų     	  | 0.0002 | 0.0001 | 0.0001 |
| 10 000 įrašų    	  | 0.0016 | 0.0021 | 0.0014 |
| 100 000 įrašų   	  | 0.0215 | 0.0330 | 0.0236 |
| 1 000 000 įrašų     | 0.2446 | 0.3862 | 0.2574 |
| 10 000 000 įrašų    | 3.1586 | 5.2494 | 3.2320 |

Originalas nekeičiamas. Kiekvienas studentas saugomas **dviejose** vietose atmintyje. Du pilni praėjimai per duomenis.

### S3 — `stable_partition` + `move`/`splice`

### Skaidymas (s)

| Failas          	  | vector | list   | deque  |
|---------------------|--------|--------|--------|
| 1 000 įrašų     	  | 0.0000 | 0.0000 | 0.0000 |
| 10 000 įrašų    	  | 0.0001 | 0.0001 | 0.0003 |
| 100 000 įrašų   	  | 0.0011 | 0.0056 | 0.0045 |
| 1 000 000 įrašų     | 0.0130 | 0.0736 | 0.0636 |
| 10 000 000 įrašų    | 0.1820 | 0.8060 | 0.7349 |

Duomenys tvarkomi **vietoje** — nereikia papildomos atminties iteracijoms. Vargšiukai perkeliami (`move`) be kopijavimo.
 `list` versijoje vietoj `move` naudojamas `splice()` — tik rodyklių pakeitimas.


### v1.1

Struct pakeista į Class
Su visual studio

Skaidymas naudojant vector (be optimizacijos):

| Failas          	  | vector |
|---------------------|--------|
| 1 000 įrašų     	  | 0.0000 |
| 10 000 įrašų    	  | 0.0001 |
| 100 000 įrašų   	  | 0.0011 |
| 1 000 000 įrašų     | 0.0125 |
| 10 000 000 įrašų    | 0.2181 |

(su /O1)
| Failas          	  | vector |
|---------------------|--------|
| 1 000 įrašų     	  | 0.0000 |
| 10 000 įrašų    	  | 0.0001 |
| 100 000 įrašų   	  | 0.0010 |
| 1 000 000 įrašų     | 0.0124 |
| 10 000 000 įrašų    | 0.3042 |

(su /O2)
| Failas          	  | vector |
|---------------------|--------|
| 1 000 įrašų     	  | 0.0000 |
| 10 000 įrašų    	  | 0.0001 |
| 100 000 įrašų   	  | 0.0009 |
| 1 000 000 įrašų     | 0.0133 |
| 10 000 000 įrašų    | 0.1656 |


Su cmake:

(su /O1)
| Failas          	  | vector |
|---------------------|--------|
| 1 000 įrašų     	  | 0.0000 |
| 10 000 įrašų    	  | 0.0001 |
| 100 000 įrašų   	  | 0.0021 |
| 1 000 000 įrašų     | 0.0273 |
| 10 000 000 įrašų    | 0.3471 |

(su /O2)
| Failas          	  | vector |
|---------------------|--------|
| 1 000 įrašų     	  | 0.0000 |
| 10 000 įrašų    	  | 0.0003 |
| 100 000 įrašų   	  | 0.0025 |
| 1 000 000 įrašų     | 0.0282 |
| 10 000 000 įrašų    | 0.3588 |

(su /O3)
| Failas          	  | vector |
|---------------------|--------|
| 1 000 įrašų     	  | 0.0000 |
| 10 000 įrašų    	  | 0.0003 |
| 100 000 įrašų   	  | 0.0023 |
| 1 000 000 įrašų     | 0.0272 |
| 10 000 000 įrašų    | 0.4811 |


Nors naudojant cmake dalinimas užtrunka ilgiau (apie 2x), tačiau
failų nuskaitymas vyskta kelis kart greičiau. Tokio dydžio duomenimis programa bendrai veikia greičiau.
O naudojant Visual Studio ar CMake vis tiek bus greičiau nei naudojant struct be optimizacijos.