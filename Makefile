CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall

TARGETS := Uzd2 stud_Vector stud_List stud_Deque metodu_test

.PHONY: all clean docs unit_testai

all: $(TARGETS)

# ---- Pagrindinis projektas ----
Uzd2: Uzd2.cpp vektorius.cpp failu_greicio_test.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) Uzd2.cpp vektorius.cpp failu_greicio_test.cpp funkcijos.cpp -o Uzd2

# ---- Konteineriu tyrimas ----
stud_Vector: Vector/Vector.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) -I. Vector/Vector.cpp funkcijos.cpp -o stud_Vector

stud_List: List/List.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) -I. List/List.cpp funkcijos.cpp -o stud_List

stud_Deque: Deque/Deque.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) -I. Deque/Deque.cpp funkcijos.cpp -o stud_Deque

# ---- Metodu testas (v1.2/v1.5) ----
metodu_test: metodu_test.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) metodu_test.cpp funkcijos.cpp -o metodu_test

# ---- Catch2 unit testai (v2.0) ----
unit_testai: unit_test/unit_test.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) -I. unit_test/unit_test.cpp funkcijos.cpp -o unit_testai

# ---- Doxygen dokumentacija ----
docs: Doxyfile
	doxygen Doxyfile
	@echo "PDF generavimui: cd docs/latex && make"

clean:
	rm -f $(TARGETS) unit_testai
	rm -rf docs/ build/
