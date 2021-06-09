#include "StringCVector.h"


vector* CreateZeroString(size_t vec_size)
{
	return CreateEmpty(vec_size, sizeof(char*));
}


vector* CreateStringFrom(size_t vec_size, char** elements)
{
	return CreateFrom(vec_size, sizeof(char*), elements);
}


bool CompareString(const void* a, const void* b)
{
	char** str_a = (char**)a;
	char** str_b = (char**)b;
	
	int i = 0;
	while(true)
	{
		if (*(*str_a + i) > *(*str_b + i))
		{
			// a[i]>b[i]
			return true;
		}
		if(*(*str_a + i ) < *(*str_b + i))
		{
			// a[i]<b[i]
			return false;
		}
		if (*(*str_a + i ) == 0 && *(*str_b + i ) == 0)
		{
			// end of a and b
			return true;
		}
		// if 0!=a[i]=b[i]!=0 continue comparing
		i++;
	}
}


vector* StringVecSort(vector* vec)
{
	return VecSort(vec, CompareString);
}
