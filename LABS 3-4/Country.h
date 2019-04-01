#pragma once

typedef struct
{
	char* name;
	char* continent;
	int population;
}Country;

Country* createCountry(char* name, char* continent, int population);
void destroyCountry(Country* c);
Country* copyCountry(Country* c);

char* getName(Country* c);
char* getContinent(Country* c);
int getPopulation(Country* c);

void toString(Country* c, char str[]);