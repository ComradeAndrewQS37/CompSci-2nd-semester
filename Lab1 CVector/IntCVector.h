#pragma once

#include <CVector.h>
#include <stdint.h>
#include <stdio.h>


vector* CreateZeroInt(size_t vec_size);

vector* CreateIntFrom(size_t vec_size, int* elements);

bool CompareInt(const void* a, const void* b);

vector* IntVecSort(vector* vec);
