# Pirmoji užduotis — v1.0 pre-release

C++17 programa studentų galutinių pažymių skaičiavimui, rūšiavimui ir skaidymui į grupes. Versija v1.0 prideda konteinerių greičio tyrimą
(`std::vector`, `std::list`, `std::deque`).

---

## Failų struktūra

```
...
├── struktura.h       — bendras antraštės failas (struktūra, using, prototipai)
├── funkcijos.cpp     — pagalbinės funkcijos (generatoriai, matematika, I/O)
├── vektorius.cpp     — pagrindinio meniu logika (v0.4, std::vector)
├── testavimas.cpp    — test1 (failų kūrimas), test2 (vector lyginimas)
├── Uzd1.cpp          — pagrindinis įėjimas (projektas Uzd1)
├── vector.cpp        — v1.0 tyrimas: std::vector  → vykdomasis: stud_Vector
├── list.cpp          — v1.0 tyrimas: std::list    → vykdomasis: stud_List
├── deque.cpp         — v1.0 tyrimas: std::deque   → vykdomasis: stud_Deque
├── CMakeLists.txt    — build konfigūracija (Windows / Linux / macOS)
├── Makefile          — build konfigūracija (Linux / macOS, be CMake)
└── Data/             — sugeneruoti testavimo failai (sukuriamas automatiškai)
    ├── studentai1k.txt
    ├── studentai10k.txt
    ├── studentai100k.txt
    ├── studentai1M.txt
    └── studentai10M.txt
```

### Bendras `Data/` katalogas

Visos trys tyrimo programos (`vector`, `list`, `deque`) ir senoji (v0.4) programa skaito duomenų failus iš to paties `Data/` katalogo.
Tai reiškia, kad failus reikia sugeneruoti tik vieną kartą — bet kuri programa gali tai padaryti, o likusios naudos tuos pačius failus.

Katalogas sukuriamas automatiškai, kai pasirenkate failų generavimą. Jis atsiranda šalia vykdomojo failo.

```
Data/
├── studentai1k.txt      (~136 KB)
├── studentai10k.txt     (~1.4 MB)
├── studentai100k.txt    (~14 MB)
├── studentai1M.txt      (~136 MB)
└── studentai10M.txt     (~1.4 GB)
```

---

## Įdiegimas ir kompiliavimas

### Reikalavimai

- C++17 palaikantis kompiliatorius (`g++ 9+` arba `clang++ 9+` arba MSVC 2019+)
- CMake 3.14+ (neprivaloma — galima kompiliuoti rankiniu būdu)

### Būdas 1 — Makefile (Linux / macOS)

Nereikia jokių papildomų įrankių — tik `g++` ir `make`, kurie dažniausiai jau įdiegti.

**1. Patikrink ar įrankiai yra:**

```bash
g++ --version
make --version
```

Jei `g++` nėra — įdiek:

```bash
# Ubuntu / Debian
sudo apt install build-essential

# macOS
xcode-select --install
```

**2. Eik į projekto katalogą:**

```bash
cd /kelias/iki/projekto
```

Tai katalogas, kuriame yra `Makefile` ir visi `.cpp` failai.

**3. Kompiliuok:**

```bash
# Sukompiliuoja visus keturis projektus iš karto
make

# Arba tik vieną konkretų
make Uzd1
make stud_Vector
make stud_List
make stud_Deque
```

**4. Paleisk:**

```bash
./Uzd1
./stud_Vector
./stud_List
./stud_Deque
```

**5. Ištrinti sukompiliuotus failus:**

```bash
make clean
```

Vykdomieji failai atsiranda tiesiai projekto kataloge šalia `.cpp` failų — `Data/` katalogas randamas teisingai.

---

### Būdas 2 — CMake (Windows / Linux / macOS)

**Linux / macOS:**

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Windows su MinGW:**

```bash
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Windows su Visual Studio** — atsidaryk `CMakeLists.txt` tiesiai per Visual Studio (File → Open → CMake), pasirink konfigūraciją Release ir spausk Build All.

Vykdomieji failai rašomi tiesiai į projekto katalogą (ne į `build/`), kad `Data/` aplankas būtų randamas teisingai.

---

### Būdas 3 — rankiniu būdu (g++)

```bash
g++ -O2 -std=c++17 Uzd1.cpp vektorius.cpp testavimas.cpp funkcijos.cpp -o Uzd1
g++ -O2 -std=c++17 vector.cpp funkcijos.cpp -o stud_Vector
g++ -O2 -std=c++17 list.cpp   funkcijos.cpp -o stud_List
g++ -O2 -std=c++17 deque.cpp  funkcijos.cpp -o stud_Deque
```


## Naudojimas

### Pagrindinė programa

Interaktyvus meniu. Leidžia įvesti studentus ranka, generuoti juos arba nuskaityti iš failo, rūšiuoti ir skaidyti į grupes.

### Tyrimo programos

Paleiskite bet kurią iš trijų programų. Pirmą kartą pasirinkite failų generavimą — jie bus sukurti `Data/` kataloge ir visų programų bendrai naudojami.

```
./stud_Vector   # sugeneruoja Data/ ir atlieka tyrimą
./stud_List     # naudoja tuos pačius Data/ failus
./stud_Deque    # naudoja tuos pačius Data/ failus
```

Failų generuoti antrą kartą nereikia — tiesiog atsakykite `0` į klausimą apie generavimą.

---

## Releasai

### v0.1 — pradinė versija

Bazinė struktūra: `Studentas` su `std::vector<int>` pažymiams, rankinis įvedimas, galutinio pažymio skaičiavimas vidurkiu ir mediana.

### v0.2

Pridėtas skaitymas iš failo ir rezultatų išvedimas į failą.

### v0.3

Pridėtas automatinis duomenų generavimas (vardai, pavardės, pažymiai). Pridėta klaidų apdorojimas `gautiSkaiciu` funkcijai.

### v0.4

Pridėtas studentų skaidymas į dvi grupes: `kieti` (galutinis ≥ 5.0) ir `tinginiai` (galutinis < 5.0).
Pridėtas failų kūrimo ir duomenų apdorojimo greičio tyrimas (test1, test2). Naudojama `copy_if` į du naujus `std::vector` konteinerius.

### v1.0 -prerelease

Pridėtos trys atskiros tyrimo programos (`vector`, `list`, `deque`), matuojančios:

- duomenų nuskaitymą iš failo;
- studentų rūšiavimą mažėjančia galutinio pažymio tvarka;

### v1.0
- skaidymą į dvi grupes dviem strategijomis (S1 ir S3).


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