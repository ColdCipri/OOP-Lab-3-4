#include "DynamicArray.h"
#include <stdlib.h>
#include <assert.h>

DynamicArray * createDA(int capacity)
{
	DynamicArray* da = (DynamicArray*)malloc(sizeof(DynamicArray));
	if (da == NULL)
		return NULL;
	da->capacity = capacity;
	da->length = 0;

	da->elems = (TElement*)malloc(capacity * sizeof(TElement));
	if (da->elems == NULL)
		return NULL;

	return da;
}

void destroyDA(DynamicArray * arr)
{
	if (arr == NULL)
		return;

	for (int i = 0; i < arr->length; i++)
		destroyCountry(arr->elems[i]);

	free(arr->elems);
	arr->elems = NULL;

	free(arr);
	arr = NULL;
}
int resize(DynamicArray * arr)
{
	if (arr == NULL)
		return;
	arr->capacity *= 2;
	TElement* aux = (TElement*)malloc(arr->capacity * sizeof(TElement));
	if (aux == NULL)
		return -1;
	for (int i = 0; i < arr->length; i++)
		aux[i] = arr->elems[i];
	free(arr->elems);
	arr->elems = aux;

	return 0;
}

void add(DynamicArray * arr, TElement t)
{
	if (arr == NULL)
		return;
	if (arr->elems == NULL)
		return;
	if (arr->length == arr->capacity)
		resize(arr);
	arr->elems[arr->length++] = t;
}
void del(DynamicArray* arr, int pos)
{
	if (arr == NULL)
		return;
	if (arr->elems == NULL)
		return;

	if (pos < 0 || pos >= arr->length)
		return;


	for (int i = pos; i < arr->length - 1; i++)
		arr->elems[i] = arr->elems[i + 1];

	arr->length--;
}
int getLength(DynamicArray* arr)
{
	if (arr == NULL)
		return -1;

	return arr->length;
}
TElement get(DynamicArray* arr, int pos)
{
	return arr->elems[pos];
}
