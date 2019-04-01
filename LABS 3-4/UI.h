#pragma once
#include "Controller.h"
#include "Utils.h"
typedef struct
{
	Controller* ctrl;
} UI;

UI* createUI(Controller* c);
void destroyUI(UI* ui);

void printMenu();
int validCommand(int command);
int addCountryUI(UI* ui);
int deleteCountryUI(UI* ui);
int updateCountryUI(UI* ui);
void listAllCountries(UI* ui);
void startUI(UI* ui);