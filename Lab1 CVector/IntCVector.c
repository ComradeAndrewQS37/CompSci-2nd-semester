#include <IntCVector.h>


vector* CreateZeroInt(size_t vec_size)
{
	return CreateEmpty(vec_size, sizeof(int));
}

vector* CreateIntFrom(size_t vec_size, int* elements)
{
	return CreateFrom(vec_size, sizeof(int), (void*) elements);
}

bool CompareInt(const void* a, const void* b)
{
	int num_a = *(int*)a;
	int num_b = *(int*)b;
	return (num_a >= num_b);
}

vector* IntVecSort(vector* vec)
{
	return VecSort(vec, CompareInt);
}

