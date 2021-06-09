#pragma once
#include <CVector.h>
#include <IntCVector.h>
#include <StringCVector.h>
#include <assert.h>
#include <string.h>
#include <time.h>

void test_all();

void test_create();

void test_copy();

void test_get_set();

void change_size();

void test_add();

void test_delete();

void test_take();

void test_concat();

void test_map();

void test_where();

void test_sort();

// make array of integers
int* RandIntArray(size_t size);

// make array of strings
char** RandStringArray(size_t size);

void* MultBySix(void* num);

void* Capitalize(void* str);

bool IsEven(void* num);

bool StartsWithC(void* str);
