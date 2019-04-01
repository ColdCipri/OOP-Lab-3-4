#include "Utils.h"
#include "Country.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Reads an integer number from the keyboard. Asks for number while read errors encoutered.
Input: the message to be displayed when asking the user for input.
Returns the number.
*/
int readIntegerNumber(const char* message)
{
	char s[16];
	int res = 0;
	int flag = 0;
	int r = 0;

	while (flag == 0)
	{
		printf(message);
		scanf("%s", s);

		r = sscanf(s, "%d", &res);	// reads data from s and stores them as integer, if possible; returns 1 if successful
		flag = (r == 1);
		if (flag == 0)
			printf("Error reading number!\n");
	}
	return res;
}

/*
Reads a string with spaces from the standard input.
Input:	message - string, message to be shown to the user.
maxStrSize - the maximum size for the string to be read
Output: the string that was read.
*/
void readStringWithSpaces(const char* message, int maxStrSize, char str[])
{
	printf(message);
	fgets(str, maxStrSize, stdin);
	// the newline is also read so we must eliminate it from the string
	int size = strlen(str) - 1;
	if (str[size] == '\n')
		str[size] = '\0';
}


void toLowerCase(char s[]) {
	int c = 0;

	while (s[c] != '\0') {
		if (s[c] >= 'A' && s[c] <= 'Z') {
			s[c] = s[c] + 32;
		}
		c++;
	}
}

char* tokenizeText(char text[50],char delim[2]) {
	char *token = strtok(text, delim);
	return token;
}


void swapCountries(Country* *c, Country* *d)
{
	Country* aux = *c;
	*c = *d;
	*d = aux;
}