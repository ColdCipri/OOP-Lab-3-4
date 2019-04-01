#pragma once
#include "Country.h"
#include "DynamicArray.h"

typedef struct
{
	DynamicArray* countries;
} CountryRepo;

CountryRepo* createRepo();
void destroyRepo(CountryRepo* v);
Country* find(CountryRepo* v, char* name);
int findPosOfCountry(CountryRepo* v, char* name);
int addCountry(CountryRepo* v, Country* c);
int addCountryWithoutCheck(CountryRepo* v, Country* c);
int deleteCountry(CountryRepo* v, char* name);
int updateCountry(CountryRepo* v, char* text, char* name, char* continent, int population);
int resizeDownCountry(CountryRepo* v, char* text, int population);
int resizeUpCountry(CountryRepo* v, char* text, int population);
int getRepoLength(CountryRepo* v);
Country* getCountryOnPos(CountryRepo* v, int pos);
CountryRepo* sortCountriesName(CountryRepo* v);