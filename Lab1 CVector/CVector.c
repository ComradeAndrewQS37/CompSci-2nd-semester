#include <CVector.h>

#define UPDATE_ERROR_CODE(code)\
	if(!ERROR_CODE){ERROR_CODE=(code);}

#define CHECK_ALLOC(ptr) \
if (!(ptr)) {UPDATE_ERROR_CODE(FailedMemAlloc);return NULL;}


// create vector with zero elements
vector* CreateEmpty(size_t vec_size, size_t element_size)
{
	if (element_size == 0)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	
	vector* vec = malloc(sizeof(vector));
	CHECK_ALLOC(vec)

	vec->element_size = element_size;
	
	vec->vec_size = vec_size;

	if (vec_size == 0)
	{
		vec->elements = NULL;
		return vec;
	}
	
	
	vec->elements = malloc(vec_size * element_size);
	CHECK_ALLOC(vec->elements)
	
	return vec;
}


// create vector from elements
vector* CreateFrom(size_t vec_size, size_t element_size, void* elements)
{
	if (vec_size == 0 || element_size == 0|| !elements)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	
	vector* vec = malloc(sizeof(vector));
	CHECK_ALLOC(vec)

	vec->element_size = element_size;

	vec->vec_size = vec_size;

	vec->elements = malloc(vec_size * element_size);
	CHECK_ALLOC(vec->elements)

	for (size_t i = 0; i < vec_size; i++)
	{
		int8_t* position_from = (int8_t*)elements + i * element_size;
		int8_t* position_to = (int8_t*)vec->elements + i * element_size;
		memcpy(position_to, position_from, element_size);
	}

	return vec;
}


// delete vector and free memory
void FreeVector (vector* vec)
{
	if (vec) 
	{
		if (vec->elements) 
		{
			free(vec->elements);
		}
		free(vec);
	}
}


// copy vector to new memory and return ptr 
vector* Copy(vector* vec)
{
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	if (vec->vec_size == 0)
	{
		return CreateEmpty(0, vec->element_size);
	}
	return CreateFrom(vec->vec_size, vec->element_size, vec->elements);
}

// copy elements from vec1 to vec2
int CopyElements(vector* vec_from, vector* vec_to)
{
	if (!vec_from || !vec_to || vec_from->vec_size != vec_to->vec_size || vec_to->element_size != vec_from->element_size)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return -1;
	}
	const size_t el_size = vec_from->element_size;
	for (size_t i = 0; i < vec_from->vec_size; i++)
	{
		int8_t* pos_from = (int8_t*)vec_from->elements + i * el_size;
		int8_t* pos_to = (int8_t*)vec_to->elements + i * el_size;
		memcpy(pos_to, pos_from, el_size);

	}
	return 0;
}


// returns ptr to vector elem by index
void* Get_i(size_t index, vector* vec)
{
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	
	if (index >= vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return NULL;
	}
	
	int8_t* position = (int8_t*)vec->elements + index * vec->element_size;
	return (void*)position;
}


// returns ptr to copy of vector elem by index
void* Get_i_s(size_t index, vector* vec)
{
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}

	if (index >= vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return NULL;
	}

	const size_t el_size = vec->element_size;
	
	int8_t* position = (int8_t*)vec->elements + index * el_size;
	void* elem_copy = malloc(el_size);
	CHECK_ALLOC(elem_copy)
	memcpy(elem_copy, position, el_size);
	
	return elem_copy;
}


// change element in this vector (returns -1 if not successful, 0 if OK) 
int Set_i(size_t index, void* value, vector* vec)
{
	if (!vec || !value)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return -1;
	}

	if (index >= vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return -1;
	}
	
	const size_t el_size = vec->element_size;
	
	int8_t* position = (int8_t*)vec->elements + index * el_size;
	memcpy(position, value, el_size);

	return 0;
}


// copy this vector, change element in new vector and return ptr to new vector
vector* Set_i_s(size_t index, void* value, vector* vec)
{
	if (!vec || !value)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}

	if (index >= vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return NULL;
	}

	vector* new_vec = Copy(vec);
	CHECK_ALLOC(new_vec)

	const size_t el_size = new_vec->element_size;
	
	int8_t* position = (int8_t*)new_vec->elements + index * el_size;
	memcpy(position, value, el_size);

	return new_vec;
}


// change size of vector
vector* Resize(size_t new_size, vector* vec)
{
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	if (new_size == 0)
	{
		return CreateEmpty(0, vec->element_size);
	}
	
	vector* new_vec = Copy(vec);
	new_vec->vec_size = new_size;
	CHECK_ALLOC(new_vec)

	if (new_size == vec->vec_size)
	{
		return new_vec;
	}

	
	void* new_elements = realloc(new_vec->elements, new_size * new_vec->element_size);
	CHECK_ALLOC(new_elements)
		
	new_vec->elements = new_elements;
		
	return new_vec;
}


// inserts new_elem to old vector copy and returns ptr to copy
vector* Insert(void* new_elem, size_t index, vector* vec)
{
	if (!vec || !new_elem)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	if (index > vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return NULL;
	}

	vector* new_vec = Resize(vec->vec_size + 1, vec);
	CHECK_ALLOC(new_vec)

	const size_t el_size = new_vec->element_size;

	memcpy((int8_t*)new_vec->elements + index * el_size, new_elem, el_size);

	for (size_t i = index + 1; i < new_vec->vec_size; i++)
	{
		memcpy((int8_t*)new_vec->elements + i * el_size, (int8_t*)vec->elements + (i - 1) * el_size, el_size);
	}

	return new_vec;
}


// add element to the end of vector (returns -1 if unsuccessful, else returns 0)
vector* Append(void* new_elem, vector* vec)
{
	if (!vec || !new_elem)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}

	vector* new_vec;
	
	if (vec->vec_size == 0)
	{
		new_vec = CreateFrom(1, vec->element_size, new_elem);
	}
	else 
	{
		new_vec = Resize(vec->vec_size + 1, vec);
		void* last_elem = (int8_t*)new_vec->elements + (new_vec->vec_size - 1) * new_vec->element_size;
		memcpy(last_elem, new_elem, new_vec->element_size);
	}
	CHECK_ALLOC(new_vec)
	
	return new_vec;
}


// take element by index and delete it from vector
void* Pop(size_t index, vector* vec)
{
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	if (index >= vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return NULL;
	}
	
	void* old_elem = Get_i_s(index, vec);
	CHECK_ALLOC(old_elem)

	vector* new_vec = Resize(vec->vec_size - 1, vec);
	CHECK_ALLOC(new_vec)

	const size_t el_size = vec->element_size;
	for (size_t i = index + 1; i < new_vec->vec_size; i++)
	{
		memcpy((int8_t*)new_vec->elements + (i - 1) * el_size, (int8_t*)vec->elements + i * el_size, el_size);
	}
	
	vec->vec_size = new_vec->vec_size;
	vec->elements = new_vec->elements;
	
	return old_elem;
}

// returns ptr to new vector without some element by index
vector* DeleteAt(size_t index, vector* vec)
{
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	if (index >= vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return NULL;
	}
	
	vector* new_vec = Resize(vec->vec_size - 1, vec);
	CHECK_ALLOC(new_vec)

	const size_t el_size = vec->element_size;
	for (size_t i = index; i < new_vec->vec_size; i++)
	{
		memcpy((int8_t*)new_vec->elements + i * el_size, (int8_t*)vec->elements + (i + 1) * el_size, el_size);
	}

	return new_vec;
}


// returns new vector which contains elements of old vector from a inclusively to b not inclusively [a, b) 
vector* Take(size_t begin, size_t end, vector* vec)
{
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	if (begin > end || end >= vec->vec_size)
	{
		UPDATE_ERROR_CODE(IndexOutOfRange)
		return NULL;
	}

	vector* new_vec = CreateFrom(end - begin, vec->element_size, (int8_t*)vec->elements + begin * vec->element_size);
	CHECK_ALLOC(new_vec)
	
	return new_vec;
}


// vector concatenation 
vector* Concat(vector* vec1, vector* vec2)
{
	if (vec1->element_size != vec2->element_size)
	{
		UPDATE_ERROR_CODE(TypeConflict)
		return NULL;
	}

	if (!vec1 || !vec2)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}

	vector* new_vec = Resize(vec1->vec_size + vec2->vec_size, vec1);
	CHECK_ALLOC(new_vec)

	const size_t el_size = vec1->element_size;

	for (size_t i = vec1->vec_size; i < new_vec->vec_size; i++)
	{
		memcpy((int8_t*)new_vec->elements + i * el_size,
			(int8_t*)vec2->elements + (i - vec1->vec_size) * el_size, el_size);
		
	}
	
	return new_vec;
}

vector* Map(vector* vec, void* (*f)(void*))
{
	// we hope that f is valid function
	if (!vec || !f)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	
	vector* new_vec = Copy(vec);
	CHECK_ALLOC(new_vec)

	if (vec->vec_size == 0)
	{
		return new_vec;
	}
	
	const size_t el_size = vec->element_size;

	void* element_copy = malloc(el_size);
	CHECK_ALLOC(element_copy)
	void* f_result = malloc(el_size);
	CHECK_ALLOC(f_result)

	for (size_t i = 0; i < vec->vec_size; i++)
	{
		int8_t* el_position = (int8_t*)new_vec->elements + i * el_size;
		memcpy(element_copy, el_position, el_size);

		f_result = f((void*)element_copy);
		if (!f_result)
		{
			return NULL;
		}
		memcpy(el_position, f_result, el_size);
	}

	return new_vec;
}

vector* Where(vector* vec, bool (*f)(void*))
{
	// we hope that f is valid function
	if (!vec || !f)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	size_t size_of_new = 0;

	vector* new_vec = CreateEmpty(vec->vec_size, vec->element_size);
	CHECK_ALLOC(new_vec)

	if (vec->vec_size == 0)
	{
		return new_vec;

	}
	const size_t el_size = vec->element_size;

	void* element_copy = malloc(el_size);
	CHECK_ALLOC(element_copy)

	for (size_t i = 0; i < vec->vec_size; i++)
	{
		int8_t* el_position = (int8_t*)vec->elements + i * el_size;
		memcpy(element_copy, el_position, el_size);
		if (f(element_copy))
		{
			memcpy((int8_t*)new_vec->elements + size_of_new * el_size, el_position, el_size);
			size_of_new++;
		}
	}

	return Resize(size_of_new, new_vec);
}

//classic bubble sort, compare_f is comparison function (must return true if a >= b, else false)
vector* VecSort(vector* vec, bool (*compare_f)(const void* a, const void* b))
{
	// we hope that compare_f is valid function
	if (!vec)
	{
		UPDATE_ERROR_CODE(InvalidArguments)
		return NULL;
	}
	
	vector* new_vec = Copy(vec);
	CHECK_ALLOC(new_vec)

	if (vec->vec_size == 0)
	{
		return new_vec;
	}

	size_t number_of_iterations = new_vec->vec_size - 1;
	const size_t el_size = new_vec->element_size;
	void* buff = malloc(el_size);
	CHECK_ALLOC(buff)

	while(number_of_iterations--)
	{
		for (size_t i = 0; i < new_vec->vec_size - 1; i++)
		{
			int8_t* element_1 = (int8_t*)new_vec->elements + i * el_size;
			int8_t* element_2 = (int8_t*)new_vec->elements + (i + 1) * el_size;
			if (compare_f((void*)element_1, (void*)element_2))
			{
				// swap element_1 and element_2
				memcpy(buff, element_1, el_size);
				memcpy(element_1, element_2, el_size);
				memcpy(element_2, buff, el_size);
			}
		}
	}

	return new_vec;
}
