#pragma once
#include "Country.h"
#define MAX 10

typedef Country* TElement;

typedef struct
{
	TElement* elems;
	int length;
	int capacity;
}DynamicArray;

DynamicArray* createDA(int capacity);
void destroyDA(DynamicArray* arr);
int resize(DynamicArray* arr);

void add(DynamicArray* arr, TElement t);
void del(DynamicArray* arr, int pos);
int getLength(DynamicArray* arr);
TElement get(DynamicArray* arr, int pos);
