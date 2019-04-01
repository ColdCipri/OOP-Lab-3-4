#include "Country.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Country * createCountry(char * name, char * continent, int population)
{
	Country* c = (Country*)malloc(sizeof(Country));

	c->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(c->name, name);

	c->continent = (char*)malloc(sizeof(char) * (strlen(continent) + 1));
	strcpy(c->continent, continent);

	c->population = population;

	return c;
}
void destroyCountry(Country * c)
{
	if (c == NULL)
		return;

	free(c->name);
	free(c->continent);

	free(c);
}
Country * copyCountry(Country * c)
{
	if (c == NULL)
		return NULL;
	Country* newCountry = createCountry(getName(c), getContinent(c), getPopulation(c));
	return newCountry;
}

char * getName(Country * c)
{
	return c->name;
}
char * getContinent(Country * c)
{
	return c->continent;
}
int getPopulation(Country * c)
{
	return c->population;
}

void toString(Country* c, char str[])
{
	sprintf(str, "%s is on %s and has %d millions of population", c->name, c->continent, c->population);
}