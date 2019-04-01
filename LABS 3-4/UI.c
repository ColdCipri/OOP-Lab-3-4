#include "UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

UI* createUI(Controller* c)
{
	UI* ui = (UI*)malloc(sizeof(UI));
	ui->ctrl = c;

	return ui;
}

void destroyUI(UI * ui)
{
	// first destroy the controller
	destroyController(ui->ctrl);
	// free the UI memory
	free(ui);
}

/*
Prints the available menu for the problem
Input: -
Output: the menu is printed at the console
*/
void printMenu()
{
	printf("\n**********************************************************\n");
	printf("0 \t To exit.\n");
	printf("1 \t Add a country.\ta\n");
	printf("2 \t Remove a country.\ta\n");
	printf("3 \t Update a country.\ta\n");
	printf("4 \t List all countries.\ta\n");
	printf("5 \t List all countries whose names contain a given string.\tb\n");
	printf("6 \t List all countries on a given continent, sorted ascending by country name.\tb activity\n");
	printf("7 \t List all countries on a given continent whose population is bigger than a given number.\tc+\n");
	printf("8 \t List all countries whose names contain a given string, sorted by population.\tc?\n");
	printf("9 \t Undo.\td\n");
	printf("10 \t Redo.\td\n");
	printf("11 \t Clear screen.\n"); 
	printf("\**********************************************************\n");
}

/*
Verifies if the given command is valid (is either 1, 2, 3or 0)
Input: command - integer
Output: 1 - if the command is valid
0 - otherwise
*/
int validCommand(int command)
{
	if (command >= 0 && command <= 11)
		return 1;
	return 0;
}


int addCountryUI(UI* ui)
{
	char name[50], continent[50];
	int population;

	fgetc(stdin); // read the newline, to prevent it from going further to fgets
	readStringWithSpaces("Please input the name: ", 50, name);
	readStringWithSpaces("Please input the continent: ", 50, continent);
	printf("Please input the population (in millions): ");
	scanf("%d", &population);

	return addCountryCtrl(ui->ctrl, name, continent, population);
}

int deleteCountryUI(UI* ui)
{
	char name[50];

	fgetc(stdin); // read the newline, to prevent it from going further to fgets
	readStringWithSpaces("Please input the name: ", 50, name);

	return deleteCountryCtrl(ui->ctrl, name);
}

int updateCountryUI(UI* ui)
{
	char text[50], name[50], continent[50];
	int population = 0, going = 1;
	while (going)
	{
		printf("\n");
		printf("0  Back\n");
		printf("1  Update all\n");
		printf("2  Migration\n");
		int command = readIntegerNumber("Input command: ");
		switch (command)
		{
		case 0:
		{
			going = 0;
			return 0;
			break;
		}
		case 1:
		{
			fgetc(stdin); // read the newline, to prevent it from going further to fgets
			readStringWithSpaces("Please input the name to be updated: ", 50, text);
			readStringWithSpaces("Please input the new name: ", 50, name);
			readStringWithSpaces("Please input the new continent: ", 50, continent);
			printf("Please input the population (in millions): ");
			scanf("%d", &population);
			return updateCountryCtrl(ui->ctrl, text, name, continent, population);
			break;
		}
		case 2:
		{
			fgetc(stdin); // read the newline, to prevent it from going further to fgets
			readStringWithSpaces("Please input the country from where the people migrate: ", 50, text);
			readStringWithSpaces("Where do they go? ", 50, name);
			printf("Please input the population (in millions) which migrate: ");
			scanf("%d", &population);
			int aux = updateCountryCtrl2(ui->ctrl, text, name, population);
			if (aux == -1)
				printf("The country where you want to migrate does not exist!");
			else if (aux == -2)
				printf("The country does not have so much population...\nsearch for another country please..\n");
			else
				printf("Migrated succesfully!\n");
			break;
		}
		default:
		{
			printf("You have to chose between 1 and 2!\n");
			break;
		}
		}
	}
}

void listAllCountries(UI* ui)
{
	CountryRepo* repo = getRepo(ui->ctrl);
	int length = getRepoLength(repo);
	if (length == 0)
	{
		char* str = "There are no stored countries.";
		printf("%s \n", str);
	}

	for (int i = 0; i < getRepoLength(repo); i++)
	{
		char str[200];
		toString(getCountryOnPos(repo, i), str);
		printf("%s\n", str);
	}
}

void listAllCountriesString(UI* ui)
{
	char text[50];
	fgetc(stdin); // read the newline, to prevent it from going further to fgets
	readStringWithSpaces("Please input the string: ", 50, text);
	int i = 0;
	CountryRepo* repo = getRepo(ui->ctrl);
	int length = getRepoLength(repo);
	int ok = 0;
	if (length == 0)
	{
		char* str = "There are no stored countries.";
		printf("%s \n", str);
	}
	if (strlen(text) == 0)
		listAllCountries(ui);
	else
	{ 
		for (int i = 0; i < getRepoLength(repo); i++)
		{
			char str[200];
			Country* c = getCountryOnPos(repo, i);
			char textlower[50], namelower[50];
			strcpy(textlower, text);
			toLowerCase(textlower);
			strcpy(namelower, c->name);
			toLowerCase(namelower);
			if (strstr(namelower, textlower) != NULL)
			{
				toString(getCountryOnPos(repo, i), str);
				printf("%s\n", str);
				ok = 1;
			}
		}
		if (ok == 0)
			printf("There are no countries that containt that string!");
	}
}

void listAllCountriesPopulation(UI* ui, int value)
{
	char text[50];
	int limit = 0;

	fgetc(stdin); // read the newline, to prevent it from going further to fgets
	readStringWithSpaces("Please input the continent: ", 50, text); 
	printf("With population (in millions) bigger than:");
	scanf("%d", &limit);
	switch (value) {
	case 1:
	{
		
		CountryRepo* repo = getRepo(ui->ctrl);
		int length = getRepoLength(repo);
		int ok = 0;
		if (length == 0)
		{
			char* str = "There are no stored countries.";
			printf("%s \n", str);
		}
		CountryRepo* new = createRepo();
		OperationsStack* undos = createStack();
		OperationsStack* redos = createStack();
		Controller* ctrl = createController(new, undos, redos);
		if (strlen(text) == 0)
		{
			char str[200];
			for (int i = 0; i <= 32767; i++)
				for (int j = 0; j < getRepoLength(repo); j++)
				{
					Country* c = getCountryOnPos(repo, j);
					if (c->population == i)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
		}
		else
		{
			char str[200];
			for (int i = 0; i < getRepoLength(repo); i++)
			{
				Country* c = getCountryOnPos(repo, i);
				char textlower[50], continentlower[50];
				strcpy(textlower, text);
				toLowerCase(textlower);
				strcpy(continentlower, c->continent);
				toLowerCase(continentlower);
				if ((strcmp(continentlower, textlower) == 0) && (c->population >= limit))
				{
					int res = addCountryCtrl(ctrl, c->name, c->continent, c->population);
					ok = 1;
				}
			}
			//SOUNDS GOOD, DOESN'T WORK -- ACTUALLY WORKS BUT 0 EFFICIENT
			for (int i = 0; i <= 32767; i++)
				for (int j = 0; j < getRepoLength(new); j++)
				{
					Country* c = getCountryOnPos(new, j);
					if (c->population == i)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
			destroyController(ctrl);
			if (ok == 0)
				printf("There are no continents with that name!");
		}
		break;
	}
	case 2:
	{
		//Ramas aici, nu intra pe cazul cand text = 0
		CountryRepo* repo = getRepo(ui->ctrl);
		int length = getRepoLength(repo);
		int ok = 0;
		if (length == 0)
		{
			char* str = "There are no stored countries.";
			printf("%s \n", str);
		}
		CountryRepo* new = createRepo();
		OperationsStack* undos = createStack();
		OperationsStack* redos = createStack();
		Controller* ctrl = createController(new, undos, redos);
		if (strlen(text) == 0)
		{
			char str[200];
			for (int i = 32767; i >= 0; i--)
				for (int j = 0; j < getRepoLength(repo); j++)
				{
					Country* c = getCountryOnPos(repo, j);
					if (c->population == i && c->population >= limit)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
		}
		else
		{
			char str[200];
			for (int i = 0; i < getRepoLength(repo); i++)
			{
				Country* c = getCountryOnPos(repo, i);
				char textlower[50], continentlower[50];
				strcpy(textlower, text);
				toLowerCase(textlower);
				strcpy(continentlower, c->continent);
				toLowerCase(continentlower);
				if ((strcmp(continentlower, textlower) == 0) && (c->population >= limit))
				{
					int res = addCountryCtrl(ctrl, c->name, c->continent, c->population);
					ok = 1;
				}
			}
			//SOUNDS GOOD, DOESN'T WORK -- ACTUALLY WORKS BUT 0 EFFICIENT
			for (int i = 32767; i >= 0; i--)
				for (int j = 0; j < getRepoLength(new); j++)
				{
					Country* c = getCountryOnPos(new, j);
					if (c->population == i)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
			destroyController(ctrl);
			if (ok == 0)
				printf("There are no continents with that name");
		}
		break;
	}
	default:
		break;
	}
	
}

void sortByPopulationGraterThan(UI* ui) {
	int going = 1;
	while (going)
	{
		printf("\n");
		printf("0  Back\n");
		printf("1  Sort ascending by population and greater than number\n");
		printf("2  Sort descending by population and greater than number\n");
		int command = readIntegerNumber("Input command: ");
		switch (command)
		{
		case 0:
		{
			going = 0;
			break;
		}
		case 1:
		{
			listAllCountriesPopulation(ui, 1);
			break;
		}
		case 2:
		{
			listAllCountriesPopulation(ui, 2);
			break;
		}
		default:
		{
			printf("You have to chose between 0, 1 or 2!\n");
			break;
		}
		}
	}
}


void listAllCountriesStringName(UI* ui)
{
	char text[50];
	fgetc(stdin); // read the newline, to prevent it from going further to fgets
	readStringWithSpaces("Please input the string: ", 50, text);

	CountryRepo* repo = getRepo(ui->ctrl);
	int length = getRepoLength(repo);
	int ok = 0;
	if (length == 0)
	{
		char* str = "There are no stored countries.";
		printf("%s \n", str);
	}
	if (strlen(text) == 0)
	{
		/*for (int i = 0; i < getRepoLength(repo) - 1; i++)
			for (int j = i+1; j < getRepoLength(repo); j++)
				if (strcmp(getCountryOnPos(repo, i)->name, getCountryOnPos(repo, j)->name) < 0)
					swapCountries( getCountryOnPos(repo, i), getCountryOnPos(repo, j) );
		repo = sortCountriesByName(repo);
		char str[200];
		for (int j = 0; j < getRepoLength(repo); j++)
		{
			Country* c = getCountryOnPos(repo, j);
			toString(c, str);
			printf("%s\n", str);
			destroyCountry(c);
			//memory leaks daca nu dau destroy
		}
		*/

		printf("Error! You have to write a continent!\n");
	}
	else
	{
		char str[200];
		CountryRepo* new = createRepo();
		OperationsStack* undos = createStack();
		OperationsStack* redos = createStack();
		Controller* ctrl = createController(new, undos, redos);
		char textlower[50], namelower[50];
		for (int i = 0; i < getRepoLength(repo); i++)
		{
			Country* c = getCountryOnPos(repo, i);
			strcpy(textlower, text);
			toLowerCase(textlower);
			strcpy(namelower, c->continent);
			toLowerCase(namelower);
			if (strstr(namelower, textlower) != NULL)
			{
				int res = addCountryCtrl(ctrl, c->name, c->continent, c->population);
				ok = 1;
			}
		}
		/*for (int i = 0; i < (getRepoLength(new) - 1); i++)
			for (int j = i+1; j < getRepoLength(new); j++)
				swapCountries(getCountryOnPos(new, i), getCountryOnPos(new, j));*/
		new = sortCountriesByName(ctrl->repo);
		for (int j = 0; j < getRepoLength(new); j++)
		{
			Country* c = getCountryOnPos(new, j);
			toString(c, str);
			printf("%s\n", str);
		}
		destroyController(ctrl);
	}
}

void listAllCountriesStringPopulation(UI* ui, int value)
{
	char text[50];
	fgetc(stdin); // read the newline, to prevent it from going further to fgets
	readStringWithSpaces("Please input the string: ", 50, text);
	switch (value) {
	case 1:
	{
		CountryRepo* repo = getRepo(ui->ctrl);
		int length = getRepoLength(repo);
		int ok = 0;
		if (length == 0)
		{
			char* str = "There are no stored countries.";
			printf("%s \n", str);
		}
		if (strlen(text) == 0)
		{
			/*char str[200];
			CountryRepo* new = createRepo();
			OperationsStack* operationsStack = createStack();
			Controller* ctrl = createController(new, operationsStack);
			for (int i = 0; i < getRepoLength(repo); i++)
			{
				Country* c = getCountryOnPos(repo, i);
				if (strstr(c->name, text) != NULL)
				{
					int res = addCountryCtrl(ctrl, c->name, c->continent, c->population);
					ok = 1;
				}
			}
			//SOUNDS GOOD, DOESN'T WORK -- ACTUALLY WORKS BUT 0 EFFICIENT
			for (int i = 0; i < (getRepoLength(repo) - 1); i++)
				for (int j = i + 1; j < getRepoLength(repo); j++)
					sortCountryCtrl(repo, getCountryOnPos(repo, i), getCountryOnPos(repo, j));
			destroyController(ctrl);*/
			char str[200];
			for (int i = 0; i <= 32767; i++)
				for (int j = 0; j < getRepoLength(repo); j++)
				{
					Country* c = getCountryOnPos(repo, j);
					if (c->population == i)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
		}
		else
		{
			char str[200];
			CountryRepo* new = createRepo();
			OperationsStack* undos = createStack();
			OperationsStack* redos = createStack();
			Controller* ctrl = createController(new, undos, redos);
			char textlower[50], namelower[50];
			for (int i = 0; i < getRepoLength(repo); i++)
			{
				Country* c = getCountryOnPos(repo, i);
				strcpy(textlower, text);
				toLowerCase(textlower);
				strcpy(namelower, c->name);
				toLowerCase(namelower);
				if (strstr(namelower, textlower) != NULL)
				{
					int res = addCountryCtrl(ctrl, c->name, c->continent, c->population);
					ok = 1;
				}
			}
			//SOUNDS GOOD, DOESN'T WORK -- ACTUALLY WORKS BUT 0 EFFICIENT
			for (int i = 0; i < 32767; i++)
				for (int j = 0; j < getRepoLength(new); j++)
				{
					Country* c = getCountryOnPos(new, j);
					if (c->population == i)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
			destroyController(ctrl);
			if (ok == 0)
				printf("There are no countries that containt that string!");
		}
		break;
	}
	case 2:
	{
		CountryRepo* repo = getRepo(ui->ctrl);
		int length = getRepoLength(repo);
		int ok = 0;
		if (length == 0)
		{
			char* str = "There are no stored countries.";
			printf("%s \n", str);
		}
		if (strlen(text) == 0)
		{
			char str[200];
			CountryRepo* new = createRepo();
			OperationsStack* undos = createStack();
			OperationsStack* redos = createStack();
			Controller* ctrl = createController(new, undos, redos);
			for (int i = 0; i < getRepoLength(repo); i++)
			{
				Country* c = getCountryOnPos(repo, i);
				if (strstr(c->name, text) != NULL)
				{
					int res = addCountryCtrl(ctrl, c->name, c->continent, c->population);
					ok = 1;
				}
			}
			//SOUNDS GOOD, DOESN'T WORK -- ACTUALLY WORKS BUT 0 EFFICIENT
			for (int i = 32767; i >= 0; i--)
				for (int j = 0; j < getRepoLength(new); j++)
				{
					Country* c = getCountryOnPos(new, j);
					if (c->population == i)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
			destroyController(ctrl);
		}
		else
		{
			char str[200];
			CountryRepo* new = createRepo();
			OperationsStack* undos = createStack();
			OperationsStack* redos = createStack();
			Controller* ctrl = createController(new, undos, redos);
			char textlower[50], namelower[50];
			for (int i = 0; i < getRepoLength(repo); i++)
			{
				Country* c = getCountryOnPos(repo, i);
				strcpy(textlower, text);
				toLowerCase(textlower);
				strcpy(namelower, c->name);
				toLowerCase(namelower);
				if (strstr(namelower, textlower) != NULL)
				{
					int res = addCountryCtrl(ctrl, c->name, c->continent, c->population);
					ok = 1;
				}
			}
			//SOUNDS GOOD, DOESN'T WORK -- ACTUALLY WORKS BUT 0 EFFICIENT
			for (int i = 32767; i >= 0; i--)
				for (int j = 0; j < getRepoLength(new); j++)
				{
					Country* c = getCountryOnPos(new, j);
					if (c->population == i)
					{
						toString(c, str);
						printf("%s\n", str);
					}
				}
			destroyController(ctrl);
			if (ok == 0)
				printf("There are no countries that containt that string!");
		}
		break;
	}
	default:
		break;
	}
}

void sortByPopulation(ui) {
	int going = 1;
	while (going)
	{
		printf("\n");
		printf("0  Back\n");
		printf("1  Sort ascending by population\n");
		printf("2  Sort descending by population\n");
		int command = readIntegerNumber("Input command: ");
		switch (command)
		{
		case 0:
		{
			going = 0;
			return 0;
			break;
		}
		case 1:
		{
			listAllCountriesStringPopulation(ui, 1);
			break;
		}
		case 2:
		{
			listAllCountriesStringPopulation(ui, 2);
			break;
		}
		default:
		{
			printf("You have to chose between 0, 1 or 2!\n");
			break;
		}
		}
	}
}

void startUI(UI* ui)
{
	int going = 1;
	while (going)
	{
		printMenu();
		int command = readIntegerNumber("Input command: ");
		while (validCommand(command) == 0)
		{
			printf("Please input a valid command!\n");
			command = readIntegerNumber("Input command: ");
		}
		switch (command)
		{
		case 0:
			going = 0;
			break;
		case 1:
		{
			int res = addCountryUI(ui);
			if (res == 1)
				printf("Country successfully added.\n");
			else
				printf("Error! Country could not be added, as there is another country with the same name!\n");
			break;
		}
		case 2:
		{
			int res = deleteCountryUI(ui);
			if (res == -1)
				printf("Error! Country could not be deleted, as there is no country with that name!\n");
			else
				printf("Country successfully deleted.\n");
			break;
		}
		case 3:
		{
			int res = updateCountryUI(ui);
			if (res == -1)
				printf("Error! Country could not be update, as there is no country with that name!\n");
			else if (res == 0)
				printf("\n");
			else
				printf("Country successfully updated.\n");
			break;
		}
		case 4:
		{
			listAllCountries(ui);
			break;
		}
		case 5:
		{
			listAllCountriesString(ui);
			break;
		}
		case 6:
		{
			listAllCountriesStringName(ui);
			break;
		}
		case 7:
		{
			sortByPopulationGraterThan(ui);
			break;
		}
		case 8:
		{
			sortByPopulation(ui);
			break;
		}
		case 9:
		{
			int res = undo(ui->ctrl);
			if (res == 1)
				printf("Undo was successful.\n");
			else
				printf("No more undos to be made.\n");
			break;
		}
		case 10:
		{
			int res = redo(ui->ctrl);
			if (res == 1)
				printf("Redo was successful.\n");
			else
				printf("No more redos to be made.\n");
			break;
		}
		case 11:
			system("cls");
		}
	}
}