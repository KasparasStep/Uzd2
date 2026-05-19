CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall

TARGETS := Uzd2 stud_Vector stud_List stud_Deque testas

.PHONY: all clean docs unit_testai

all: $(TARGETS)

# ---- Pagrindinis projektas ----
Uzd2: Uzd2.cpp vektorius.cpp testavimas.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) Uzd2.cpp vektorius.cpp testavimas.cpp funkcijos.cpp -o Uzd2

# ---- Tyrimo projektai ----
stud_Vector: stud_Vector.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) stud_Vector.cpp funkcijos.cpp -o stud_Vector

stud_List: stud_List.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) stud_List.cpp funkcijos.cpp -o stud_List

stud_Deque: stud_Deque.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) stud_Deque.cpp funkcijos.cpp -o stud_Deque

# ---- Paprasti testai ----
testas: testas.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) testas.cpp funkcijos.cpp -o testas

# ---- Catch2 unit testai ----
# Reikalauja catch.hpp faile projekto kataloge
unit_testai: unit_testai.cpp funkcijos.cpp struktura.h catch.hpp
	$(CXX) $(CXXFLAGS) unit_testai.cpp funkcijos.cpp -o unit_testai

# ---- Doxygen dokumentacija ----
docs: Doxyfile
	doxygen Doxyfile
	@echo "Norint sukompiliuoti PDF, vykdykite: cd docs/latex && make"

clean:
	rm -f $(TARGETS) unit_testai
	rm -rf docs/ build/
