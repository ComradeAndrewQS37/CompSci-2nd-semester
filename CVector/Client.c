#include <Client.h>

#define CHECK_VECTOR(vec)\
	if(!(vec)){ErrorExit();}

#define SCAN_DECIMAL(number)							\
	printf(">>> ");										\
	int res = scanf_s("%d", &(number));					\
	while (res == 0)									\
	{													\
		printf("\nInput valid value, please\n>>> ");	\
		while (1)										\
		{												\
			if (getchar() == '\n')						\
			{											\
				break;									\
			}											\
		}												\
		res = scanf_s("%d", &(number));					\
	}													\
	getchar();											\

#define SCAN_STRING(str)								\
	(str) = malloc(sizeof(char) * 256);					\
	if(!(str))											\
	{													\
		ERROR_CODE = FailedMemAlloc;					\
		ErrorExit();									\
	}													\
	printf(">>> ");										\
	if(!fgets(string, 256, stdin))						\
	{													\
		ERROR_CODE = UnknownError;						\
		ErrorExit();									\
	}													\
	string[strcspn((str), "\n")] = 0;					\



char errorMessage[5][40] =
{
	"failed memory allocation",
	"index out of range",
	"type conflicts between two vectors",
	"invalid argument",
	"unknown error"
};

// 0 - undefined, 1 - int, 2 - string 
int type_vec1 = 0;
int type_vec2 = 0;

void ClientStart()
{
	printf("Hi! We are ready to work with vector and test it\n");
	while (1) 
	{
		MainMenu();
	}
}

void MainMenu()
{
	printf("\nChoose what you want to do:\n1) Run automatic tests\n2) Try basic functions\n3) Finish work\n");
	int choice = ChoiceScan(1, 3);
	switch (choice)
	{
		// auto tests
		case 1:
			test_all();
			printf("\nCongrats! It seems that all tests went successfully.\n");
			return;
		// manual testing
		case 2:
			printf("\nOkay, we are ready to start working with vectors\n");
			ClientVector();
			return;
		// exit
		default:
			exit(0);
	}
}

int ChoiceScan(int min, int max)
{
	int option;
	printf(">>> ");
	int res = scanf_s("%d", &option);
	while (1)
	{
		if (getchar()=='\n')
		{
			break;
		}
	}
	while (option < min || option > max || res == 0)
	{
		printf("\nChoose valid option, please\n>>> ");
		res = scanf_s("%d", &option);
		while (1)
		{
			if (getchar() == '\n')
			{
				break;
			}
		}
	}

	return option;
	
}

void ClientVector()
{
	printf("\nFirstly, we need to create a vector.\n");
	vector* vec1 = ScanVector();
	CHECK_VECTOR(vec1)
	
	printf("\nFine, here's your vector:\n");
	PrintVector(vec1, type_vec1);
	
	printf("\nOkay, now we can already work with this vector or create the second one.");
	vector* vec2 = NULL;
	while(1)
	{
		if (WorkWithOneVector(vec1))
		{
			// if user wants to create new vector
			while (1)
			{
				vec2 = ScanVector();
				CHECK_VECTOR(vec2)

				printf("\nFine, here's your vector:\n");
				PrintVector(vec2, type_vec2);

				if (type_vec1 == type_vec2) {
					vector* concat_vec = Concat(vec1, vec2);
					if (concat_vec == NULL && ERROR_CODE == TypeConflict)
					{
						// if vectors have different types
						ERROR_CODE = OK;
					}
					else
					{
						CHECK_VECTOR(concat_vec)
							printf("\nBoth vectors have the same element type, so we can concatenate them. Here's what we get then:\n");
						PrintVector(concat_vec, type_vec1);
					}
				}
				
				printf("\nDo you want to work with this new vector?\n1) Yes\n2) No, exit\n");
				if (ChoiceScan(1, 2) == 2)
				{
					exit(0);
				}
				
				vec1 = vec2;
				type_vec1 = type_vec2;
				type_vec2 = 0;
				break;
				
			}
		}
	}
	
}


vector* ScanVector()
{
	vector* vec;
	
	printf("\nWhat type would new vector be?\n1) Integer\n2) String\n");
	int type = ChoiceScan(1, 2);
	if (!type_vec1)
	{
		type_vec1 = type;
	}else
	{
		type_vec2 = type;
	}
	
	printf("Got it. And what size should it be? \n>>> ");
	int size;
	int res = scanf_s("%d", &size);
	while (size <= 0 || res == 0)
	{
		printf("\nInput valid value, please (size should be positive)\n>>> ");	
		while (1)
		{
			if (getchar() == '\n')
			{
				break;
			}
		}
		res = scanf_s("%d", &(size));
	}
	
	printf("\nOkay. Should it be filled with random data or with your data?\n1) Random\n2) My data\n");
	int source = ChoiceScan(1, 2);
	
	if (type == 1)
	{
		int* data = malloc(sizeof(int) * size);
		int* position = data;
		if (source == 1)
		{
			srand(time(NULL));
			for (int i = 0; i < size; i++)
			{
				int number = rand() % 100 - 10;
				memcpy(position, &number, sizeof(int));
				position++;
			}
			 
			
		}else
		{
			printf("\nType every new number on new line\n");
			for (int i = 0; i < size; i++)
			{
				int number;
				SCAN_DECIMAL(number)
				memcpy(position, &number, sizeof(int));
				position++;

			}
			
		}
		vec = CreateIntFrom(size, data);
		
		
	}else
	{
		char** data = malloc(sizeof(char*) * size);
		char** position = data;
		if (source == 1) {
			srand(time(NULL));
			for (int i = 0; i < size; i++)
			{
				const size_t str_size = rand() % 7 + 3;
				char* string = malloc((str_size + 1) * sizeof(char));
				for (size_t j = 0; j < str_size; j++)
				{
					*(string + j) = 'a' + rand() % 26;
				}
				*(string + str_size) = '\0';
				memcpy(position, &string, sizeof(char*));
				position++;
			}
		}else
		{
			printf("\nType every new string on new line\n");
			for (int i = 0; i < size; i++)
			{
				char* string;
				SCAN_STRING(string)

				memcpy(position, &string, sizeof(char*));
				position++;
			}
		}
		vec = CreateStringFrom(size, data);
	}

	return vec;
	
}


void PrintVector(vector* vec, int vec_type)
{
	if (vec->vec_size == 0)
	{
		printf("[.....Empty vector.....]\n");
		return;
	}
	if (vec_type == 1)
	{
		for (size_t i = 0; i < vec->vec_size; i++)
		{
			int* position = (int*)vec->elements + i;
			printf("%d\n", *position);
		}
	}
	else if (vec_type == 2)
	{
		for (size_t i = 0; i < vec->vec_size; i++)
		{
			char* position = *((char**)vec->elements + i);
			printf("%s\n", position);
		}
	}else
	{
		ERROR_CODE = UnknownError;
		ErrorExit();
	}
}

// returns 1 if user wants to create new vector
int WorkWithOneVector(vector* vec)
{
	printf("\nWhat you'd like to do? \
			\n1) Change element in this vector\n2) Insert element to this vector\n3) Delete element from this vector \
			\n4) Sort this vector\n5) Test Map and Where functions\n6) Create new vector\n7) Print this vector\n8) Finish work\n");
	int index;
	void* value;
	vector* new_vec;
	
	
	switch (ChoiceScan(1, 8))
	{
	case 1:// change element
		if (vec->vec_size == 0)
		{
			printf("\nVector is empty, no elements to change here\n");
			break;
		}
		if (vec->vec_size == 1)
		{
			printf("\nVector has only one element, so we'll change it\n");
			index = 0;
		}
		else 
		{
			printf("\nWhat element you'd like to change? (type index)\n");
			index = ChoiceScan(0, vec->vec_size - 1);
		}
		printf("\nEnter the new element, please\n");
		
		if (type_vec1 == 1)
		{
			int num;
			SCAN_DECIMAL(num)
			value = (void*)&num;
		}
		else
		{
			char* string;
			SCAN_STRING(string)
			value = (void*)&string;
		}
		
		new_vec = Set_i_s(index, value, vec);
		CHECK_VECTOR(new_vec)
		*vec = *new_vec;
		
		printf("\nHere's edited vector\n");
		PrintVector(vec, type_vec1);
		break;
	case 2:// insert element
		if (vec->vec_size == 0)
		{
			printf("\nThere is only one place, where we can insert the element, as vector's empty\n");
			index = 0;
		}
		else
		{
			printf("\nWhere would you like to insert new element? (type index)\n");
			index = ChoiceScan(0, vec->vec_size);
		}
		printf("\nEnter the new element, please\n");
		
		if (type_vec1 == 1)
		{
			int num;
			SCAN_DECIMAL(num)
			value = (void*)&num;
		}
		else
		{
			char* string;
			SCAN_STRING(string)
			value = (void*)&string;
		}
		
		new_vec = Insert(value, index, vec);
		CHECK_VECTOR(new_vec)
		*vec = *new_vec;
		
		printf("\nHere's edited vector\n");
		PrintVector(vec, type_vec1);
		break;
	case 3:// delete element
		if (vec->vec_size == 0)
		{
			printf("\nOur vector already has no elements, nothing to delete\n");
			break;
		}
		if (vec->vec_size == 1)
		{
			printf("\nOur vector has only one element, so if you delete it, the vector will be empty. I warned you\n");
			printf("Delete the last element?\n1) Yes\n2) No\n");
			if (ChoiceScan(1, 2) == 1)
			{
				index = 0;
			}
			else
			{
				break;
			}
		}
		else 
		{
			printf("\nWhat element would you like to delete? (type index)\n");
			index = ChoiceScan(0, vec->vec_size - 1);
		}

		new_vec = DeleteAt(index, vec);
		CHECK_VECTOR(new_vec)
		*vec = *new_vec;
		
		printf("\nHere's edited vector\n");
		PrintVector(vec, type_vec1);
		break;
	case 4:// sort vector
		new_vec = ClientSort(vec, type_vec1);
		CHECK_VECTOR(new_vec)
		*vec = *new_vec;
		
		printf("\nOkay, I've sorted this vector especially for you\n");
		PrintVector(vec, type_vec1);
		break;
	case 5:// map and where test
		printf("\nFirstly, let's test Map function.\n");
		if (type_vec1 == 1)
		{
			printf("We have vector of integers and MultBySix function which can multiply all numbers by six.\n");
			new_vec = Map(vec, MultBySix);
			CHECK_VECTOR(new_vec)

			printf("\nAnd here's the result\n");
			PrintVector(new_vec, type_vec1);
		}
		else if (type_vec1 == 2)
		{
			printf("We have vector of strings and Capitalize function which can capitalize all latin symbols all strings.\n");
			new_vec = Map(vec, Capitalize);
			CHECK_VECTOR(new_vec)

			printf("\nAnd here's the result\n");
			PrintVector(new_vec, type_vec1);
		}else
		{
			ERROR_CODE = UnknownError;
			ErrorExit();
			break;
		}

		printf("\nNow let's test Where function.\n");
		if (type_vec1 == 1)
		{
			printf("We have vector of integers and IsEven function which leaves only even numbers in vector\n");
			new_vec = Where(vec, IsEven);
			CHECK_VECTOR(new_vec)

			printf("\nAnd here's the result\n");
			PrintVector(new_vec, type_vec1);
		}
		else if (type_vec1 == 2)
		{
			printf("We have vector of strings and StartsWithC function which leaves only strings starting with 'c' in our vector\n");
			new_vec = Where(vec, StartsWithC);
			CHECK_VECTOR(new_vec)

			printf("\nAnd here's the result\n");
			PrintVector(new_vec, type_vec1);
		}
		else
		{
			ERROR_CODE = UnknownError;
			ErrorExit();
			break;
		}
		break;
	case 6:// create new vector
		return 1;
	case 7:// print vector
		printf("\nFine, here's your vector:\n");
		PrintVector(vec, type_vec1);
		break;
	default:// exit
		exit(0);
	}
	
	return 0;
}

void ErrorExit()
{
	printf("\nUnfortunately, we can't continue working because of %s", errorMessage[ERROR_CODE - 1]);
	exit(1);
}

vector* ClientSort(vector* vec, int type_vec)
{
	if (type_vec == 1)
	{
		return IntVecSort(vec);
	}
	if (type_vec == 2)
	{
		return StringVecSort(vec);
	}

	ERROR_CODE = UnknownError;
	ErrorExit();
	return NULL;
}
