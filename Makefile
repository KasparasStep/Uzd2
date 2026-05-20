CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall

TARGETS := Uzd2 stud_Vector stud_List stud_Deque metodu_test

.PHONY: all clean docs unit_testai

all: $(TARGETS)

# ---- Pagrindinis projektas ----
Uzd2: Uzd2.cpp vektorius.cpp testavimas.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) Uzd2.cpp vektorius.cpp testavimas.cpp funkcijos.cpp -o Uzd2

# ---- Konteineriu tyrimas ----
stud_Vector: stud_Vector.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) stud_Vector.cpp funkcijos.cpp -o stud_Vector

stud_List: stud_List.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) stud_List.cpp funkcijos.cpp -o stud_List

stud_Deque: stud_Deque.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) stud_Deque.cpp funkcijos.cpp -o stud_Deque

# ---- Paprastas metodu testas (v1.2/v1.5) ----
metodu_test: metodu_test.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) metodu_test.cpp funkcijos.cpp -o metodu_test

# ---- Catch2 unit testai (v2.0) ----
unit_testai: unit_testai.cpp funkcijos.cpp struktura.h catch.hpp
	$(CXX) $(CXXFLAGS) unit_testai.cpp funkcijos.cpp -o unit_testai

# ---- Doxygen dokumentacija ----
docs: Doxyfile
	doxygen Doxyfile
	@echo "PDF generavimui: cd docs/latex && make"

clean:
	rm -f $(TARGETS) unit_testai
	rm -rf docs/ build/
