#pragma once

#include <CVector.h>

#include <stdint.h>
#include <stdio.h>


vector* CreateZeroString(size_t vec_size);

vector* CreateStringFrom(size_t vec_size, char** elements);

bool CompareString(const void* a, const void* b);

vector* StringVecSort(vector* vec);
