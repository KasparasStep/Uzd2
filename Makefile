CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall

TARGETS := Uzd1 stud_Vector stud_List stud_Deque

.PHONY: all clean

all: $(TARGETS)

# ---- Uzd1 (pagrindinis projektas) ----
Uzd1: Uzd1.cpp vektorius.cpp testavimas.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) Uzd1.cpp vektorius.cpp testavimas.cpp funkcijos.cpp -o Uzd1

# ---- Tyrimo projektai ----
stud_Vector: vector.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) vector.cpp funkcijos.cpp -o stud_Vector

stud_List: list.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) list.cpp funkcijos.cpp -o stud_List

stud_Deque: deque.cpp funkcijos.cpp struktura.h
	$(CXX) $(CXXFLAGS) deque.cpp funkcijos.cpp -o stud_Deque

clean:
	rm -f $(TARGETS)