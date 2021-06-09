#pragma once

#include "LinkedList.h"
#include "DynamicArray.h"
#include "ICollection.h"
#include "IEnumerable.h"
#include "UI.h"

#include <stdlib.h>
#include <time.h>
#include <cassert>


void test_LinkedList();
void test_DynamicArray();
void test_ListSequence();
void test_ArraySequence();

int* RandIntSeq(int size);
double* RandDoubleSeq(int size);

bool IsDivisibleBySix(const int num);
double DivideByTwo(const int num);
int SumBin(const int n, const int m);
