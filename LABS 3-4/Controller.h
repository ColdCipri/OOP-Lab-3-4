#pragma once
#include "CountryRepository.h"
#include "OperationStack.h"
typedef struct
{
	CountryRepo* repo;
	OperationsStack* undoStack;
	OperationsStack* redoStack;
} Controller;

Controller* createController(CountryRepo* r, OperationsStack* undoS, OperationsStack * redoS);
void destroyController(Controller* c);

int addCountryCtrl(Controller* c, char* name, char* continent, int population);
int addCountryCtrlWithoutCheck(Controller * v, char * name, char * continent, int population);
int deleteCountryCtrl(Controller * v, char* name);
int updateCountryCtrl2(Controller * v, char* text, char* name, int population);
int updateCountryCtrl(Controller * v, char* text, char* name, char* continent, int population);
CountryRepo * sortCountriesByName(CountryRepo* c);
CountryRepo* getRepo(Controller* c);

int undo(Controller* c);
int redo(Controller* c);