#include "OperationStack.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Operation* createOperation(Country* cb, char* operationType, Country* ca)
{
	Operation* o = (Operation*)malloc(sizeof(Operation));
	o->countryBefore = copyCountry(cb);
	o->countryAfter = copyCountry(ca);

	if (operationType != NULL)
	{
		o->operationType = (char*)malloc(sizeof(char) * (strlen(operationType) + 1));
		strcpy(o->operationType, operationType);
	}
	else
		o->operationType = NULL;

	return o;
}

void destroyOperation(Operation* o)
{
	if (o == NULL)
		return;

	// first destroy the countries
	destroyCountry(o->countryAfter);
	destroyCountry(o->countryBefore);
	// then the operationType
	free(o->operationType);
	// then free the operation
	free(o);
}

Operation* copyOperation(Operation * o)
{
	if (o == NULL)
		return NULL;

	Operation* newOp = createOperation(o->countryBefore, o->operationType, o->countryAfter);
	return newOp;
}

char* getOperationType(Operation* o)
{
	return o->operationType;
}

Country* getCountryBefore(Operation* o)
{
	return o->countryBefore;
}

Country* getCountryAfter(Operation* o)
{
	return o->countryAfter;
}
// ---------------------------------------------------------------

OperationsStack* createStack()
{
	OperationsStack* s = (OperationsStack*)malloc(sizeof(OperationsStack));
	s->length = 0;

	return s;
}

void destroyStack(OperationsStack* s)
{
	if (s == NULL)
		return;

	// first deallocate memory of operations (this is allocated when a new operation is pushed onto the stack)
	for (int i = 0; i < s->length; i++)
		destroyOperation(s->operations[i]);

	// then free the stack
	free(s);
}

void push(OperationsStack* s, Operation* o)
{
	if (isFull(s))
		return;

	s->operations[s->length++] = copyOperation(o);	// copies of operations are added, such that the stask manages its own operations
}

Operation* pop(OperationsStack* s)
{
	if (isEmpty(s))
		return NULL;
	s->length--;
	return s->operations[s->length];
}

int isEmpty(OperationsStack* s)
{
	return (s->length == 0);
}

int isFull(OperationsStack* s)
{
	return s->length == 100;
}


