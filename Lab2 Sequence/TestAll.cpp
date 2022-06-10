#include "TestAll.h"

void test_all()
{
	// containers test
	test_LinkedList();
	test_DynamicArray();
	test_ListSequence();
	test_ArraySequence();

	// vector test
	vector_test_construction_get_set();
	vector_test_operations();

	// matrix test
	matrix_test_construction_get_set();
	matrix_test_operations();
	
	std::cout << "All tests passed successfully" << std::endl;
}