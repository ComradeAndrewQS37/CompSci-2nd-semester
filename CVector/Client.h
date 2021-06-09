#pragma once
#include <CVector.h>
#include <IntCVector.h>
#include <StringCVector.h>
#include <Tests.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// start point of client work
void ClientStart();

void MainMenu();

// to scan option from list
int ChoiceScan(int min, int max);

// work with vector manually
void ClientVector();

// if we need to scan vector
vector* ScanVector();

void PrintVector(vector* vec, int vec_type);

int WorkWithOneVector(vector* vec1);

void ErrorExit();

vector* ClientSort(vector* vec, int type_vec);
