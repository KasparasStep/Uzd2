//funkcijos.h
#pragma once
#include "struktura.h"
// Funkcijų prototipai
int gautiSkaiciu(string info, int min, int max);
string genVarda();
string genPavarde(string vardas);
void genPazymius(vector<int>& paz, int& egz);

void skaitytiIsFailo(const string& failas, vector<Studentas>& grupe, int metodas);
void spausdintiRezultatus(const vector<Studentas>& grupe, int rodyti, const string& failas);
void vykdytiVector();

void genFaila(const string& failas, int kiek);
void splitStudents(const vector<Studentas>& visi, vector<Studentas>& kieti,
	vector <Studentas>& tiniginiai, int metodas);

void test1();
void test2(const string& failas, int metodas);
