#include "Controller.h"
#include "Utils.h"
#include <stdlib.h>
#include <string.h>

Controller * createController(CountryRepo * r, OperationsStack * undoS, OperationsStack * redoS)
{
	Controller* c = (Controller*)malloc(sizeof(Controller));
	c->repo = r;
	c->undoStack = undoS;
	c->redoStack = redoS;

	return c;
}

void destroyController(Controller * c)
{
	// first destroy the repository inside
	destroyRepo(c->repo);

	// then the operation stacks
	destroyStack(c->undoStack);
	destroyStack(c->redoStack);
	// then free the memory
	free(c);
}

int addCountryCtrl(Controller * v, char * name, char * continent, int population)
{
	Country* c = createCountry(name, continent, population);

	int res = addCountry(v->repo, c);

	if (res == 1) // if the country was successfully added - register the operation
	{
		Operation* o = createOperation(NULL, "add", c);
		push(v->undoStack, o);
		// once added, the operation can be destroyed (a copy of the operation was added)
		destroyOperation(o);
	}

	// destroy the country that was just created, as the repository stored a copy
	destroyCountry(c);

	return res;
}

int addCountryCtrlWithoutCheck(Controller* v, char* name, char* continent, int population)
{
	Country* c = createCountry(name, continent, population);

	int res = addCountryWithoutCheck(v->repo, c);

	if (res == 1) // if the country was successfully added - register the operation
	{
		Operation* o = createOperation(NULL, "add", c);
		push(v->undoStack, o);
		// once added, the operation can be destroyed (a copy of the operation was added)
		destroyOperation(o);
	}

	// destroy the country that was just created, as the repository stored a copy
	destroyCountry(c);

	return res;
}

int deleteCountryCtrl(Controller * v, char* name)
{
	if (findPosOfCountry(v->repo, name) != -1) {
		Country* aux = copyCountry(getCountryOnPos(v->repo, findPosOfCountry(v->repo, name)));
		int res = deleteCountry(v->repo, name);

		if (res != -1) // if the country was successfully deleted - register the operation
		{
			Operation* o = createOperation(aux, "delete", NULL);
			push(v->undoStack, o);
			// once added, the operation can be destroyed (a copy of the operation was added)
			destroyOperation(o);
		}
		destroyCountry(aux);
		return res;
	}

}

int updateCountryCtrl2(Controller * v, char * text, char * name, int population)
{
	int pos = -1, aux = 0;
	if (findPosOfCountry(v->repo, text) != -1)
	{
		int aux = resizeDownCountry(v->repo, text, population);
		if (aux == -1)
			return -2;
		
		if (findPosOfCountry(v->repo, name) != -1)
			pos = resizeUpCountry(v->repo, name, population);	
		else
		{
			return -1;
		}
		Country* ca = getCountryOnPos(v->repo, findPosOfCountry(v->repo, name));
		Country* cb = getCountryOnPos(v->repo, findPosOfCountry(v->repo, text));
		char toReplace[200];
		sprintf(toReplace, "%d", population);
		strcat(toReplace, ",population");

		Operation* o = createOperation(cb, toReplace, ca);
		push(v->undoStack, o);
		// once added, the operation can be destroyed (a copy of the operation was added)
		destroyOperation(o);
	}


	return pos;
}

int updateCountryCtrl(Controller * v, char* text, char* name, char* continent, int population)
{
	if (findPosOfCountry(v->repo, text) != -1) {
		Country* cb = copyCountry(getCountryOnPos(v->repo, findPosOfCountry(v->repo, text)));
		int res = updateCountry(v->repo, text, name, continent, population);

		Country* ca = getCountryOnPos(v->repo, findPosOfCountry(v->repo, name));
		if (res == -1)
			return -1;
		// if the country was successfully updated - register the operation
		char str[50] = "update";
		//strcat(str, name);
		
		Operation* o = createOperation(cb, str, ca);
		push(v->undoStack, o);
		// once added, the operation can be destroyed (a copy of the operation was added)
		destroyOperation(o);

		destroyCountry(cb);
		
		return res;
	}
	else
		return -1;
	
}
	

CountryRepo * getRepo(Controller * c)
{
	return c->repo;
}

CountryRepo * sortCountriesByName(CountryRepo* c) {
	return sortCountriesName(c);
}

int undo(Controller * c)
{
	if(isEmpty(c->undoStack))
	{
		return 0;
	}

	Operation* operation = pop(c->undoStack);

	if (strcmp(getOperationType(operation), "add") == 0)
	{
		Country* country = getCountryAfter(operation);
		char name[50];
		strcpy(name, getName(country));
		deleteCountry(c->repo, name); 

		Operation* o = createOperation(NULL, "add", country);
		push(c->redoStack, o);
		destroyOperation(o);
		
	}
	else if (strcmp(getOperationType(operation), "delete") == 0)
	{
		Country* country = getCountryBefore(operation);
		addCountry(c->repo, country);

		Operation* o = createOperation(country, "delete", NULL);
		push(c->redoStack, o);
		destroyOperation(o);
	}
	else if (strstr(getOperationType(operation), "update") != NULL)
	{
		Country* countryBefore = getCountryBefore(operation);
		Country* countryAfter = getCountryAfter(operation);
		char str[50];
		strcpy(str, getOperationType(operation));

		/*char *token = tokenizeText(str, ",");
		token = tokenizeText(NULL, ",");
		printf("%s\t%s\t%s\t%d\n", token, country->name, country->continent,country->population);*/
		updateCountry(c->repo, countryAfter->name, countryBefore->name, countryBefore->continent, countryBefore->population);


		Operation* o = createOperation(countryBefore, "update", countryAfter);
		push(c->redoStack, o);
		destroyOperation(o);

	}
	else if (strstr(getOperationType(operation), "population") != NULL)
	{
		Country* countryBefore = getCountryBefore(operation);
		Country* countryAfter = getCountryAfter(operation);
		char str[50];
		strcpy(str, getOperationType(operation));
		char *number = tokenizeText(str, ",");
		char *p;
		long val = strtol(number, &p, 10); // Convert number from string to long
		int j = resizeUpCountry(c->repo, countryBefore->name, val);
		int i = resizeDownCountry(c->repo, countryAfter->name, val);


		Operation* o = createOperation(countryBefore, getOperationType(operation), countryAfter);
		push(c->redoStack, o);

		destroyOperation(o);
	}

	// the operation must be destroyed
	destroyOperation(operation);

	return 1;
}

int redo(Controller* c)
{
	if (isEmpty(c->redoStack))
		return 0;


	Operation* operation = pop(c->redoStack);
	push(c->undoStack, operation);

	if (strcmp(getOperationType(operation), "add") == 0)
	{
		Country* country = getCountryAfter(operation);
		addCountry(c->repo, country);
	}
	else if (strcmp(getOperationType(operation), "delete") == 0)
	{
		Country* country = getCountryBefore(operation);
		deleteCountry(c->repo, country->name);
	}
	else if (strstr(getOperationType(operation), "update") != NULL)
	{
		char str[50];
		strcpy(str, getOperationType(operation));
		char *token = tokenizeText(str, ",");
		token = tokenizeText(NULL, ",");
		Country* countryBefore = getCountryBefore(operation);
		Country* countryAfter = getCountryAfter(operation);
		updateCountry(c->repo, countryBefore->name, countryAfter->name, countryAfter->continent, countryAfter->population);

	}
	else if (strstr(getOperationType(operation), "population") != NULL)
	{
		Country* countryBefore = getCountryBefore(operation);
		Country* countryAfter = getCountryAfter(operation);
		printf("%s\t\n", getOperationType(operation));
		char str[50];
		strcpy(str, getOperationType(operation));
		char *number = tokenizeText(str, ",");
		char *p;
		long val = strtol(number, &p, 10); // Convert number from string to long
		int j = resizeUpCountry(c->repo, countryAfter->name, val);
		int i = resizeDownCountry(c->repo, countryBefore->name, val);
	}
	destroyOperation(operation);
	return 1;

}

