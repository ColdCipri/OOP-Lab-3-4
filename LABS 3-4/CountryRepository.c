#include "CountryRepository.h"
#include "Utils.h"
#include <stdlib.h>
#include <string.h>

CountryRepo * createRepo()
{
	CountryRepo * v = (CountryRepo*)malloc(sizeof(CountryRepo));
	v->countries = createDA(MAX);

	return v;
}
void destroyRepo(CountryRepo * v)
{
	if (v == NULL)
		return;
	destroyDA(v->countries);
	free(v);
}

Country* find(CountryRepo * v, char * name)
{
	if (v == NULL)
		return NULL;
	int pos = findPosOfCountry(v, name);
	if (pos == -1)
		return NULL;
	return get(v->countries, pos);
}

int findPosOfCountry(CountryRepo * v, char * name)
{
	if (v == NULL)
		return -1;
	char textlower[50], namelower[50];
	for (int i = 0; i < getLength(v->countries); i++)
	{
		Country* c = get(v->countries, i);
		strcpy(textlower, name);
		toLowerCase(textlower);
		strcpy(namelower, c->name);
		toLowerCase(namelower);
		if (strcmp(namelower, textlower) == 0)
			return i;
	}
	return -1;
}

int addCountry(CountryRepo* v, Country* c)
{
	if (v == NULL)
		return 0;
	//search for a country with the same name as the one that is to be added and do not add it if it already exists
	if (find(v, c->name) != NULL)
		return 0;
	Country* copy = copyCountry(c);
	add(v->countries, copy);

	return 1;
}

int addCountryWithoutCheck(CountryRepo* v, Country* c)
{
	if (v == NULL)
		return 0;
	Country* copy = copyCountry(c);
	add(v->countries, copy);

	return 1;
}

int deleteCountry(CountryRepo* v, char* text)
{
	if (v == NULL)
		return;

	// find the position of the country in the repository
	int pos = findPosOfCountry(v, text);
	if (pos == -1)
		return -1;

	//destroy the country
	Country* c = get(v->countries, pos);
	destroyCountry(c);
	
	//delete the pointer from the DA
	del(v->countries, pos);
	return 1;
}

int updateCountry(CountryRepo* v, char* text, char* name, char* continent, int population)
{
	if (v == NULL)
		return 0;
	
	// find the position of the country in the repository
	int pos = findPosOfCountry(v, text);
	if (pos == -1)
		return -1;

	Country* c = get(v->countries, pos);

	free(c->name);
	free(c->continent);

	c->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(c->name, name);

	c->continent = (char*)malloc(sizeof(char) * (strlen(continent) + 1));
	strcpy(c->continent, continent);

	c->population = population;

	return 1;
}

int resizeDownCountry(CountryRepo* v, char* text, int population)
{
	if (v == NULL)
		return NULL;

	// find the position of the country in the repository
	int pos = findPosOfCountry(v, text);

	Country* c = get(v->countries, pos);
	int aux = c->population;
	aux -= population;
	if (aux < 0)
		return -1;
	c->population -= population;
	return 0;
}

int resizeUpCountry(CountryRepo* v, char* text, int population)
{
	if (v == NULL)
		return 0;

	// find the position of the country in the repository
	int pos = findPosOfCountry(v, text);
	if (pos == -1)
		return -1;

	Country* c = get(v->countries, pos);
	c->population += population;
	return 1;
}

int getRepoLength(CountryRepo* v)
{
	if (v == NULL)
		return -1;

	return getLength(v->countries);
}

Country* getCountryOnPos(CountryRepo* v, int pos)
{
	if (v == NULL)
		return NULL;
	if (pos < 0 || pos >= getLength(v->countries))
		return NULL;

	return get(v->countries, pos);
}

CountryRepo* sortCountriesName(CountryRepo* v)
{
	int length = getLength(v->countries);
	int i, j;
	for (i = 0; i < length - 1; i++) {
		Country* c = getCountryOnPos(v, i);
		for (j = i + 1; j < length; j++) {
			Country* d = getCountryOnPos(v, j);
			if (strcmp(c->name, d->name) > 0)
				swapCountries(&v->countries->elems[i], &v->countries->elems[j]);
		}
	}
	return v;
}

