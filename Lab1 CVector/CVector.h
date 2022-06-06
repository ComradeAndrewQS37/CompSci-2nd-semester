#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct Vector vector;
typedef struct TypeInfo type_info;

enum Error_Codes
{
	OK,
	FailedMemAlloc,
	IndexOutOfRange,
	TypeConflict,
	InvalidArguments,
	UnknownError
};

enum Error_Codes ERROR_CODE;

struct Vector
{
	size_t element_size;
	size_t vec_size;
	void* elements;
};


// create vector with zero elements
vector* CreateEmpty(size_t vec_size, size_t element_size);

// create vector from elements
vector* CreateFrom(size_t vec_size, size_t element_size, void* elements);

// delete vector and free memory
void FreeVector(vector* vec);

// copy vector to new memory and return ptr 
vector* Copy(vector* vec);

// copy elements from vec1 to vec2
int CopyElements(vector* vec_from, vector* vec_to);

// returns ptr to vector elem by index
void* Get_i(size_t index, vector* vec);

// returns ptr to copy of vector elem by index
void* Get_i_s(size_t index, vector* vec);

// change element in this vector (returns -1 if not successful, 0 if OK) 
int Set_i(size_t index, void* value, vector* vec);

// copy this vector, change element in new vector and return ptr to new vector
vector* Set_i_s(size_t index, void* value, vector* vec);

// change size of vector
vector* Resize(size_t new_size, vector* vec);

// inserts new_elem to old vector copy and returns ptr to copy
vector* Insert(void* new_elem, size_t index, vector* vec);

// add element to the end of vector (returns -1 if unsuccessful, else returns 0)
vector* Append(void* new_elem, vector* vec);

// take element by index and delete it from vector
void* Pop(size_t index, vector* vec);

// returns ptr to new vector without some element by index
vector* DeleteAt(size_t index, vector* vec);

// returns new vector which contains elements of old vector from a inclusively to b [a, b) 
vector* Take(size_t begin, size_t end, vector* vec);

// vector concatenation 
vector* Concat(vector* vec1, vector* vec2);

vector* Map(vector* vec, void* (*f)(void*));

vector* Where(vector* vec, bool (*f)(void*));

//classic bubble sort, compare_f is comparison function (must return true if a >= b, else false)
vector* VecSort(vector* vec, bool (*compare_f)(const void* a, const void* b));
