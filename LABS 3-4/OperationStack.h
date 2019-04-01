#pragma once
#include "Country.h"
#include "DynamicArray.h"
#include <string.h>

typedef struct
{
	Country* countryBefore;
	char* operationType;
	Country* countryAfter;
} Operation;

Operation* createOperation(Country* cb, char* operationType, Country* ca);
void destroyOperation(Operation* o);
Operation* copyOperation(Operation* o);
char* getOperationType(Operation* o);
Country* getCountryBefore(Operation* o);
Country* getCountryAfter(Operation* o);

// ---------------------------------------------------------------
typedef struct
{
	Operation* operations[100];
	int length;
} OperationsStack;

OperationsStack* createStack();
void destroyStack(OperationsStack* s);
void push(OperationsStack* s, Operation* o);
Operation* pop(OperationsStack* s);
int isEmpty(OperationsStack* s);
int isFull(OperationsStack* s);
