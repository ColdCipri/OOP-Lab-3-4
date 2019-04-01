#pragma once
#include "Country.h"

int readIntegerNumber(const char* message);
void readStringWithSpaces(const char* message, int maxStrSize, char str[]);
void toLowerCase(char s[]); 
char* tokenizeText(char text[50], char delim[2]);
void swapCountries(Country* *c, Country* *d);