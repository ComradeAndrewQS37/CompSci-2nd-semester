#include <Tests.h>

void test_all()
{	
	test_create();
	test_copy();
	test_get_set();
	change_size();
	test_add();
	test_delete();
	test_take();
	test_concat();
	test_map();
	test_where();
	test_sort();
}


void test_create()
{
	ERROR_CODE = 0;

	const size_t int_size = sizeof(int);

	vector* int_vec0 = CreateEmpty(3, int_size);
	assert(int_vec0->elements);
	

	vector* int_vec1 = CreateEmpty(0, int_size);
	assert(int_vec1->elements == NULL);
	

	vector* int_vec2 = CreateEmpty(3, NULL);
	assert(int_vec2 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;


	size_t size = rand() % 10 + 1;
	int* nums = RandIntArray(size);
	vector* int_vec3 = CreateFrom(size, int_size, (void*)nums);
	for (size_t i = 0; i < size; i++)
	{
		assert(*((int*)int_vec3->elements + i) == nums[i]);
	}

	vector* int_vec4 = CreateFrom(0, int_size, (void*)nums);
	assert(int_vec4 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;


	vector* int_vec5 = CreateFrom(3, NULL, (void*)nums);
	assert(int_vec5 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;


	vector* int_vec6 = CreateFrom(4, int_size, NULL);
	assert(int_vec6 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;

	size = rand() % 10 + 1;
	vector* int_vec7 = CreateZeroInt(size);
	assert(int_vec7->elements);

	size = rand() % 10 + 1;
	nums = RandIntArray(size);
	vector* int_vec8 = CreateIntFrom(size, (void*)nums);
	for (size_t i = 0; i < size; i++)
	{
		assert(*((int*)int_vec8->elements + i) == nums[i]);
	}
	
	const size_t str_size = sizeof(char*);

	size = rand() % 10 + 1;

	vector* str_vec0 = CreateEmpty(size, str_size);
	assert(str_vec0->elements);


	vector* str_vec1 = CreateEmpty(0, str_size);
	assert(str_vec1->elements == NULL);


	vector* str_vec2 = CreateEmpty(3, NULL);
	assert(str_vec2 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;

	
	size = rand() % 10 + 1;
	char** strings = RandStringArray(size);
	vector* str_vec3 = CreateFrom(size, str_size, (void*)strings);
	for (size_t i = 0; i < size; i++)
	{
		assert(!strcmp(*((char**)str_vec3->elements + i), strings[i]));
	}

	vector* str_vec4 = CreateFrom(0, str_size, (void*)strings);
	assert(str_vec4 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;


	vector* str_vec5 = CreateFrom(3, NULL, (void*)strings);
	assert(str_vec5 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;


	vector* str_vec6 = CreateFrom(4, str_size, NULL);
	assert(int_vec6 == NULL);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;

	
	size = rand() % 10 + 1;
	vector* str_vec7 = CreateZeroString(size);
	assert(str_vec7->elements);


	size = rand() % 10 + 1;
	strings = RandStringArray(size);
	vector* str_vec8 = CreateStringFrom(size, (void*)strings);
	for (size_t i = 0; i < size; i++)
	{
		assert(!strcmp(*((char**)str_vec8->elements + i), strings[i]));
	}
}

void test_copy()
{
	ERROR_CODE = 0;

	const size_t int_size = sizeof(int);

	vector* int_vec0 = CreateEmpty(3, int_size);
	vector* int_vec0_copy = Copy(int_vec0);
	for (size_t i = 0; i < 3; i++)
	{
		assert(*((int*)int_vec0->elements + i) == *((int*)int_vec0_copy->elements + i));
	}

	size_t size = rand() % 10 + 1;
	int* nums = RandIntArray(size);
	vector* int_vec1 = CreateFrom(size, int_size, (void*)nums);
	vector* int_vec1_copy = Copy(int_vec1);
	for (size_t i = 0; i < size; i++)
	{
		assert(*((int*)int_vec1->elements + i) == *((int*)int_vec1_copy->elements + i));
	}

	vector* int_vec2 = CreateEmpty(0, int_size);
	vector* int_vec2_copy = Copy(int_vec2);
	assert(int_vec2_copy->elements == NULL && int_vec2_copy->vec_size == 0);

	const size_t str_size = sizeof(char*);

	vector* str_vec0 = CreateEmpty(3, str_size);
	vector* str_vec0_copy = Copy(str_vec0);
	assert(str_vec0_copy->elements);

	size = rand() % 10 + 1;
	char** strings = RandStringArray(size);
	vector* str_vec1 = CreateFrom(size, str_size, (void*)strings);
	vector* str_vec1_copy = Copy(str_vec1);
	for (size_t i = 0; i < size; i++)
	{
		assert(!strcmp(*((char**)str_vec1->elements + i), *((char**)str_vec1_copy->elements + i)));
	}

	vector* str_vec2 = CreateEmpty(0, str_size);
	vector* str_vec2_copy = Copy(str_vec2);
	assert(str_vec2_copy->elements == NULL && str_vec2_copy->vec_size == 0);

	vector* str_vec3 = CreateFrom(size, str_size, (void*)strings);
	vector* str_vec3_copy_1 = CreateEmpty(size, str_size);
	CopyElements(str_vec3, str_vec3_copy_1);
	for (size_t i = 0; i < size; i++)
	{
		assert(!strcmp(*((char**)str_vec3->elements + i), *((char**)str_vec3_copy_1->elements + i)));
	}
	vector* str_vec3_copy_2 = CreateEmpty(size + 2, str_size);
	assert(CopyElements(str_vec3, str_vec3_copy_2) == -1);
	assert(ERROR_CODE == 4);
	ERROR_CODE = 0;

	
}

void test_get_set()
{
	ERROR_CODE = 0;

	const size_t int_size = sizeof(int);

	size_t size = rand() % 10 + 1;
	int* nums = RandIntArray(size);
	vector* int_vec1 = CreateIntFrom(size, (void*)nums);
	for (size_t i = 0; i < size; i++)
	{
		assert(Get_i(i, int_vec1) == (int*)int_vec1->elements + i);
		assert(*(int*)Get_i_s(i, int_vec1) == *((int*)int_vec1->elements + i));
		assert(Get_i_s(i, int_vec1) != (int*)int_vec1->elements + i);
	}
	assert(Get_i(size + 3, int_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;
	assert(Get_i(6, NULL) == NULL && ERROR_CODE == 4);
	ERROR_CODE = 0;
	assert(Get_i_s(size + 2, int_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;
	assert(Get_i_s(rand() % size, NULL) == NULL && ERROR_CODE == 4);
	ERROR_CODE = 0;

	int new_int_value = 777;
	int new_pos = rand() % size;
	assert(Set_i(new_pos, &new_int_value, int_vec1) == 0 && *((int*)int_vec1->elements + new_pos) == new_int_value);

	new_pos = rand() % size;
	vector* int_vec1_changed = Set_i_s(new_pos, &new_int_value, int_vec1);
	assert(*((int*)int_vec1_changed->elements + new_pos) == new_int_value);
	
	assert(Set_i(size + 6, &new_int_value, int_vec1) == -1 && ERROR_CODE == 2);
	ERROR_CODE = 0;
	
	assert(Set_i_s(size + 7, &new_int_value, int_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	const size_t str_size = sizeof(char*);

	size = rand() % 10 + 1;
	char** strings = RandStringArray(size);
	vector* str_vec1 = CreateStringFrom(size, (void*)strings);
	for (size_t i = 0; i < size; i++)
	{
		assert(Get_i(i, str_vec1) == (char**)str_vec1->elements + i);
		assert(*(char**)Get_i_s(i, str_vec1) == *((char**)str_vec1->elements + i));
		assert(Get_i_s(i, str_vec1) != (char**)str_vec1->elements + i);
	}
	assert(Get_i(size + rand() % 10, str_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;
	assert(Get_i(6, NULL) == NULL && ERROR_CODE == 4);
	ERROR_CODE = 0;
	assert(Get_i_s(size + rand() % 10, str_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;
	assert(Get_i_s(6, NULL) == NULL && ERROR_CODE == 4);
	ERROR_CODE = 0;

	char* new_str_value = "cwq8139-]cqcwe";
	int new_index = rand() % size;
	assert(Set_i(new_index, &new_str_value, str_vec1) == 0 && !strcmp(*((char**)str_vec1->elements + new_index), new_str_value));

	vector* str_vec1_changed = Set_i_s(new_index, &new_str_value, str_vec1);
	assert(!strcmp(*((char**)str_vec1_changed->elements + new_index), new_str_value));

	assert(Set_i(size + 34, &new_str_value, str_vec1) == -1 && ERROR_CODE == 2);
	ERROR_CODE = 0;

	assert(Set_i_s(size + 3, &new_str_value, str_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

}

void change_size()
{
	ERROR_CODE = 0;
	
	const size_t int_size = sizeof(int);

	size_t size = rand() % 10 + 2;
	int* nums = RandIntArray(size);
	vector* int_vec1 = CreateIntFrom(size, (void*)nums);

	size_t new_size = size + rand() % 10;
	vector* int_vec2 = Resize(new_size, int_vec1);
	for (size_t i = 0; i < new_size; i++)
	{
		if (i < size)
		{
			assert(*((int*)int_vec1->elements + i) == *((int*)int_vec2->elements + i));
		}
	}

	new_size = size - 1;
	vector* int_vec3 = Resize(new_size, int_vec1);
	for (size_t i = 0; i < new_size; i++)
	{
		assert(*((int*)int_vec1->elements + i) == *((int*)int_vec2->elements + i));
		
	}

	vector* int_vec4 = Resize(0, int_vec1);
	assert(int_vec4->elements == NULL && int_vec4->vec_size == 0);


	const size_t str_size = sizeof(char*);

	size = rand() % 10 + 2;
	char** strings = RandStringArray(size);
	vector* str_vec1 = CreateStringFrom(size, (void*)strings);

	new_size = size + rand() % 10;
	vector* str_vec2 = Resize(new_size, str_vec1);
	for (size_t i = 0; i < new_size; i++)
	{
		if (i < size)
		{
			assert(!strcmp(*((char**)str_vec1->elements + i), *((char**)str_vec2->elements + i)));
		}
	}

	new_size = size - 1;
	vector* str_vec3 = Resize(new_size, str_vec1);
	for (size_t i = 0; i < new_size; i++)
	{
		assert(!strcmp(*((char**)str_vec1->elements + i), *((char**)str_vec2->elements + i)));
	}

	vector* str_vec4 = Resize(0, str_vec1);
	assert(str_vec4->elements == NULL && str_vec4->vec_size == 0);

}
void test_add()
{
	ERROR_CODE = 0;
	const size_t int_size = sizeof(int);

	int nums[5] = { 4, -8, 0, 382, 21 };
	int new_num = 555;
	vector* int_vec1 = CreateIntFrom(5, (void*)nums);
	
	vector* int_vec2 = Insert(&new_num, 78, int_vec1);
	assert(int_vec2 == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	vector* int_vec3 = Insert(&new_num, 0, int_vec1);
	assert(*(int*)Get_i(0, int_vec3) == new_num);
	for (size_t i = 1; i <= 5; i++)
	{
		assert(*(int*)Get_i(i, int_vec3) == *(int*)Get_i(i - 1, int_vec1));
	}

	vector* int_vec4 = Insert(&new_num, 2, int_vec1);
	assert(*(int*)Get_i(2, int_vec4) == new_num);
	assert(*(int*)Get_i(1, int_vec4) == nums[1]);
	assert(*(int*)Get_i(4, int_vec4) == nums[3]);

	vector* int_vec5 = Append(&new_num, int_vec1);
	assert(*(int*)Get_i(5, int_vec5) == new_num);
	assert(*(int*)Get_i(1, int_vec5) == nums[1]);

	const size_t str_size = sizeof(char*);

	char* strings[5] = { "qwef","w23f2", "[][wfwvq","gqb34gea", "eqfqwa" };
	char* new_str = "heeeeelp";
	vector* str_vec1 = CreateStringFrom(5, (void*)strings);

	vector* str_vec2 = Insert(&new_str, 78, str_vec1);
	assert(str_vec2 == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	vector* str_vec3 = Insert(&new_str, 0, str_vec1);
	assert(strcmp(*(char**)Get_i(0, str_vec3), new_str) == 0);
	for (size_t i = 1; i <= 5; i++)
	{
		assert(strcmp(*(char**)Get_i(i, str_vec3), *(char**)Get_i(i - 1, str_vec1)) == 0);
	}

	vector* str_vec4 = Insert(&new_str, 2, str_vec1);
	assert(strcmp(*(char**)Get_i(2, str_vec4), new_str) == 0);
	assert(strcmp(*(char**)Get_i(1, str_vec4), strings[1]) == 0);
	assert(strcmp(*(char**)Get_i(4, str_vec4), strings[3]) == 0);

	vector* str_vec5 = Append(&new_str, str_vec1);
	assert(strcmp(*(char**)Get_i(5, str_vec5), new_str) == 0);
	assert(strcmp(*(char**)Get_i(1, str_vec5), strings[1]) == 0);
}

void test_delete()
{
	ERROR_CODE = 0;
	const size_t int_size = sizeof(int);

	int nums[5] = { 4, -8, 0, 382, 21 };
	vector* int_vec1 = CreateIntFrom(5, (void*)nums);
	vector* int_vec0 = CreateEmpty(0, int_size);

	vector* int_vec2 = DeleteAt(1, int_vec1);
	assert(*(int*)Get_i_s(0, int_vec2) == nums[0] && *(int*)Get_i_s(1, int_vec2) == nums[2]);

	vector* int_vec3 = DeleteAt(7, int_vec1);
	assert(int_vec3 == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	vector* int_vec4 = DeleteAt(0, int_vec0);
	assert(int_vec4 == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;
	
	assert(Pop(34, int_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;
	assert(Pop(0, int_vec0) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;


	int p_num1 = *(int*)Pop(1, int_vec1);
	assert(p_num1 == -8 && *(int*)Get_i_s(0, int_vec1) == nums[0] && *(int*)Get_i_s(1, int_vec1) == nums[2]);

	const size_t str_size = sizeof(char*);

	char* strings[5] = { "qwef","w23f2", "[][wfwvq","gqb34gea", "eqfqwa" };
	vector* str_vec1 = CreateStringFrom(5, (void*)strings);
	vector* str_vec0 = CreateEmpty(0, str_size);

	vector* str_vec2 = DeleteAt(1, str_vec1);
	assert(strcmp(*(char**)Get_i_s(0, str_vec2), strings[0]) == 0 && strcmp(*(char**)Get_i_s(1, str_vec2), strings[2]) == 0);

	vector* str_vec3 = DeleteAt(7, str_vec1);
	assert(str_vec3 == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	vector* str_vec4 = DeleteAt(0, str_vec0);
	assert(str_vec4 == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	assert(Pop(34, str_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;
	assert(Pop(0, str_vec0) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;


	char* p_str1 = *(char**)Pop(1, str_vec1);
	assert(strcmp(p_str1, strings[1]) == 0 && strcmp(*(char**)Get_i_s(0, str_vec1), strings[0]) == 0 && strcmp(*(char**)Get_i_s(1, str_vec1), strings[2]) == 0);

}

void test_take()
{
	ERROR_CODE = 0;
	const size_t int_size = sizeof(int);

	int nums[5] = { 4, -8, 0, 382, 21 };
	vector* int_vec1 = CreateIntFrom(5, (void*)nums);
	
	assert(Take(0, 6, int_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	assert(Take(13, 56, int_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	assert(Take(4, 1, int_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	vector* int_vec2 = Take(1, 3, int_vec1);
	assert(int_vec2->vec_size == 2);
	for (size_t i = 0, j = 1; i < 2; i++, j++)
	{
		assert(*(int*)Get_i_s(i, int_vec2) == *(int*)Get_i_s(j, int_vec1));
	}

	const size_t str_size = sizeof(char*);

	char* strings[6] = { "t", "qwef","w23f2", "[][wfwvq","gqb34gea", "eqfqwa" };
	vector* str_vec1 = CreateStringFrom(6, (void*)strings);

	assert(Take(0, 8, str_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	assert(Take(13, 56, str_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	assert(Take(4, 1, str_vec1) == NULL && ERROR_CODE == 2);
	ERROR_CODE = 0;

	vector* str_vec2 = Take(2, 5, str_vec1);
	assert(str_vec2->vec_size == 3);
	for (size_t i = 0, j = 2; i < 3; i++, j++)
	{
		assert(strcmp(*(char**)Get_i_s(i, str_vec2), *(char**)Get_i_s(j, str_vec1)) == 0);
	}
}


void test_concat()
{
	ERROR_CODE = 0;
	const size_t int_size = sizeof(int);

	int nums_1[3] = { 34, 0, -23423 };
	int nums_2[5] = { 653, 3421, -342, 0, 1331};
	int nums_1_2[8] = { 34, 0, -23423,653, 3421, -342, 0, 1331};
	vector* int_vec0 = CreateZeroInt(0);
	vector* int_vec1 = CreateIntFrom(3, nums_1);
	vector* int_vec2 = CreateIntFrom(5, nums_2);
	vector* int_vec3 = Concat(int_vec1, int_vec2);
	assert(int_vec3->vec_size == 8);
	for (size_t i = 0; i < 8; i++)
	{
		assert(*(int*)Get_i_s(i, int_vec3) == nums_1_2[i]);
	}
	vector* int_vec4 = Concat(int_vec3, int_vec0);
	for (size_t i = 0; i < 8; i++)
	{
		assert(*(int*)Get_i_s(i, int_vec3) == *(int*)Get_i_s(i, int_vec4));
	}
	vector* int_vec5 = Concat(int_vec0, int_vec4);
	for (size_t i = 0; i < 8; i++)
	{
		assert(*(int*)Get_i_s(i, int_vec5) == *(int*)Get_i_s(i, int_vec4));
	}

	const size_t str_size = sizeof(char*);

	char* strings_1[4] = { "qq","","342w","=-0o-ijq" };
	char* strings_2[2] = { "34","e23ax" };
	char* strings_1_2[6] = { "qq","","342w","=-0o-ijq" ,"34","e23ax" };
	
	vector* str_vec0 = CreateZeroString(0);
	vector* str_vec1 = CreateStringFrom(4, strings_1);
	vector* str_vec2 = CreateStringFrom(2, strings_2);
	vector* str_vec3 = Concat(str_vec1, str_vec2);
	assert(str_vec3->vec_size == 6);
	for (size_t i = 0; i < 6; i++)
	{
		assert(strcmp(*(char**)Get_i_s(i, str_vec3), strings_1_2[i]) == 0);
	}
	vector* str_vec4 = Concat(str_vec3, str_vec0);
	for (size_t i = 0; i < 6; i++)
	{
		assert(strcmp(*(char**)Get_i_s(i, str_vec3), *(char**)Get_i_s(i, str_vec4)) == 0);
	}
	vector* str_vec5 = Concat(str_vec0, str_vec4);
	for (size_t i = 0; i < 6; i++)
	{
		assert(strcmp(*(char**)Get_i_s(i, str_vec5), *(char**)Get_i_s(i, str_vec4)) == 0);
	}
	
}

void test_map()
{
	ERROR_CODE = 0;
	const size_t int_size = sizeof(int);

	int nums[5] = { 653, 3421, -342, 0, 1331 };
	vector* int_vec0 = CreateZeroInt(0);
	vector* int_vec1 = CreateIntFrom(5, nums);
	vector* int_vec2 = Map(int_vec1, MultBySix);
	for (size_t i = 0; i < 5; i++)
	{
		assert(*(int*)Get_i_s(i, int_vec1) * 6 == *(int*)Get_i_s(i, int_vec2));
	}
	vector* int_vec3 = Map(int_vec0, MultBySix);
	assert(int_vec3->vec_size == 0 && int_vec3->elements == NULL);

	const size_t str_size = sizeof(char*);

	char* strings[6] = { "qq","","342w","=-0o-ijq" ,"34","e23ax" };
	vector* str_vec0 = CreateZeroString(0);
	vector* str_vec1 = CreateStringFrom(6, strings);
	vector* str_vec2 = Map(str_vec1, Capitalize);
	for (size_t i = 0; i < 6; i++)
	{
		char* el_1 = *(char**)Get_i_s(i, str_vec1);
		char* el_2 = *(char**)Get_i_s(i, str_vec2);

		int j = 0;
		while (1)
		{
			char* position_1 = el_1 + j;
			char* position_2 = el_2 + j;

			if (*position_1 == 0)
			{
				break;
			}
			if ('a' <= *position_1 && *position_2 <= 'z')
			{
				assert(*position_2 == *position_1 - ('a' - 'A'));
			}
			j++;
		}
	}
	vector* str_vec3 = Map(str_vec0, Capitalize);
	assert(str_vec3->vec_size == 0 && str_vec3->elements == NULL);

}

void test_where()
{
	ERROR_CODE = 0;
	const size_t int_size = sizeof(int);

	int nums_1[6] = { 4,13,76,0,-6 };
	int nums_1_even[4] = { 4,76,0,-6 };
	vector* int_vec0 = CreateZeroInt(0);
	vector* int_vec1 = CreateIntFrom(6, nums_1);
	vector* int_vec2 = Where(int_vec1, IsEven);
	for (size_t i = 0; i < 4; i++) 
	{
		assert(*(int*)Get_i_s(i, int_vec2) == nums_1_even[i]);
	}

	int nums_2[4] = { 5,13,79,1 };
	vector* int_vec3 = CreateIntFrom(4, nums_2);
	vector* int_vec4 = Where(int_vec3, IsEven);
	assert(int_vec4->vec_size == 0 && int_vec4->elements == NULL);
	vector* int_vec5 = Where(int_vec0, IsEven);
	assert(int_vec5->vec_size == 0 && int_vec5->elements == NULL);


	const size_t str_size = sizeof(char*);

	char* strings_1[5] = { "32gwf", "cdfqq", "CWdes", "141dq[]]", "" };
	char* strings_1_c[2] = {"cdfqq", "CWdes"};
	vector* str_vec0 = CreateZeroString(0);
	vector* str_vec1 = CreateStringFrom(5, strings_1);
	vector* str_vec2 = Where(str_vec1, StartsWithC);
	for (size_t i = 0; i < 2; i++)
	{
		assert(strcmp(*(char**)Get_i_s(i, str_vec2), strings_1_c[i]) == 0);
	}

	char* strings_2[4] = { "dwfqevac", "q3", "", "=1dqd	" };
	vector* str_vec3 = CreateStringFrom(4, strings_2);
	vector* str_vec4 = Where(str_vec3, StartsWithC);
	assert(str_vec4->vec_size == 0 && str_vec4->elements == NULL);
	vector* str_vec5 = Where(str_vec0, StartsWithC);
	assert(str_vec5->vec_size == 0 && str_vec5->elements == NULL);
}

void test_sort()
{
	ERROR_CODE = 0;
	const size_t int_size = sizeof(int);

	int nums[4] = { 7,21,0,-345 };
	int nums_s[4] = { -345,0,7,21 };
	vector* int_vec0 = CreateZeroInt(0);
	vector* int_vec1 = CreateIntFrom(4, nums);
	vector* int_vec2 = IntVecSort(int_vec0);
	assert(int_vec2->vec_size == 0 && int_vec2->elements == NULL);
	vector* int_vec3 = IntVecSort(int_vec1);
	for (size_t i = 0; i < 4; i++)
	{
		assert(*(int*)Get_i_s(i, int_vec3) == nums_s[i]);
	}

	const size_t str_size = sizeof(char*);

	char* strings[4] = { "","qabd", "b]ed", "asrf" };
	char* strings_s[4] = { "", "asrf", "b]ed", "qabd" };
	vector* str_vec0 = CreateZeroString(0);
	vector* str_vec1 = CreateStringFrom(4, strings);
	vector* str_vec2 = StringVecSort(str_vec0);
	assert(str_vec2->vec_size == 0 && str_vec2->elements == NULL);
	vector* str_vec3 = StringVecSort(str_vec1);
	for (size_t i = 0; i < 4; i++)
	{
		assert(strcmp(*(char**)Get_i_s(i, str_vec3), strings_s[i]) == 0);
	}
	
}

// make array of integers
int* RandIntArray(size_t size)
{
	srand(time(NULL));

	int* int_array = malloc(sizeof(int) * size);
	
	for (int i =0;i<size;i++)
	{
		*(int_array + i) = rand() % 1000 - 500;
	}

	return int_array;
}

// make array of strings
char** RandStringArray(size_t size)
{
	srand(time(NULL));

	char** str_array = malloc(sizeof(char*) * size);
	char** position = str_array;
	
	for (size_t i = 0; i < size; i++)
	{
		const size_t str_size = rand() % 9 + 3;
		char* string = malloc((str_size + 1) * sizeof(char));
		for (size_t j = 0; j < str_size; j++)
		{
			*(string + j) = 'a' + rand() % 26;
		}
		*(string + str_size) = '\0';
		memcpy(position, &string, sizeof(char*));
		position++;
	}

	return str_array;
}


//functions for map and where testing
void* MultBySix(void* num)
{
	*(int*)num = *(int*)num * 6;
	return num;
}

void* Capitalize(void* str)
{
	const size_t str_len = strlen(*(char**)str);
	char* new_str = malloc((str_len + 1) * sizeof(char));
	if (!new_str)
	{
		ERROR_CODE = 1;
		return NULL;
	}
	if (strcpy_s(new_str, (str_len + 1) * sizeof(char), *(char**)str))
	{
		ERROR_CODE = 1;
		return NULL;
	}
	int i = 0;
	while (1)
	{
		char* position = new_str + i;
		if (*position == 0)
		{
			break;
		}
		if ('a' <= *position && *position <= 'z')
		{
			*position = *position - ('a' - 'A');
		}
		i++;
	}

	return &new_str;
}

bool IsEven(void* num)
{
	return *(int*)num % 2 == 0;
}

bool StartsWithC(void* str)
{
	return **(char**)str == 'c' || **(char**)str == 'C';
}
