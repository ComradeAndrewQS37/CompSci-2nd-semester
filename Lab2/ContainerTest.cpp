#include "ContainerTest.h"

void test_LinkedList()
{
	// constructors tests
	int* items = RandIntSeq(18);
	LinkedList<int> list1 = LinkedList<int>(items, 18);
	for (int i = 0; i < 18; i++)
	{
		assert(list1[i] == items[i]);
	}

	LinkedList<int> list2 = LinkedList<int>(list1);
	for (int i = 0; i < 18; i++)
	{
		assert(list2[i] == items[i]);
		assert(list2[i] == list1[i]);
	}

	// getters tests
	assert(list1.GetFirst() == list1[0]);
	assert(list1.GetLast() == list1[17]);
	for (int i = 0; i < 18; i++)
	{
		assert(list2.Get(i) == items[i]);
		assert(list2.Get(i) == list1[i]);
	}

	try
	{
		list1.Set(36, 89);
	}
	catch(std::out_of_range& e)
	{
		assert(strcmp(e.what(), "Index [36] was out of range") == 0);
	}
	catch(...)
	{
		assert(0);
	}

	// sublist tests
	LinkedList<int>* list3 = list1.GetSubList(3, 13);
	for (int i = 0, j = 3; j < 13; i++, j++)
	{
		assert(list1[j] == list3->Get(i));
	}

	// add and remove elements tests
	list2.Prepend(3);
	assert(list2[0] == 3);
	for (int i = 1; i < 19; i++)
	{
		assert(list2[i] == items[i - 1]);
	}

	list2.Remove(0);
	for (int i = 0; i < 18; i++)
	{
		assert(list2[i] == items[i]);
	}

	list2.Append(-45);
	assert(list2[18] == -45);
	for (int i = 0; i < 18; i++)
	{
		assert(list2[i] == items[i]);
	}
	list2.Remove(18);

	list2.InsertAt(23, 9);
	for (int i = 0; i < 19; i++)
	{
		if (i < 9)
		{
			assert(list2[i] == items[i]);
		}
		else if (i == 9)
		{
			assert(list2[i] == 23);
		}
		else
		{
			assert(list2[i] == items[i - 1]);
		}
	}

	// concatenation test
	int* new_items = RandIntSeq(26);
	LinkedList<int> list4 = LinkedList<int>(new_items, 26);
	LinkedList<int>* list5 = list1.Concat(&list4);
	for (int i = 0; i < list1.GetLength() + list4.GetLength(); i++)
	{
		if (i < list1.GetLength())
		{
			assert(list5->Get(i) == items[i]);
		}
		else
		{
			assert(list5->Get(i) == new_items[i - list1.GetLength()]);
		}
	}

	// find elem test
	int nums[6] = { -3,66,7,-12,0,1 };
	LinkedList<int> list6 = LinkedList<int>(nums, 6);
	assert(list6.FindFirst(IsDivisibleBySix) == 1);
}

void test_DynamicArray()
{
	// constructors testing

	int* items = RandIntSeq(22);
	DynamicArray<int> array1 = DynamicArray<int>(items, 22);
	for (int i = 0; i < 22; i++)
	{
		assert(array1[i] == items[i]);
	}

	DynamicArray<int> array2 = DynamicArray<int>(array1);
	for (int i = 0; i < 22; i++)
	{
		assert(array2[i] == items[i]);
		assert(array2[i] == array1[i]);
	}

	// getters and setters testing
	for (int i = 0; i < 22; i++)
	{
		assert(array2.Get(i) == items[i]);
	}

	array2.Set(4, -67);
	assert(array2[4] == -67);

	// resize tests
	array1.Resize(25);
	assert(array1.GetSize() == 25);
	array1[24] = 45;
	assert(array1[24] == 45);

	array1.Resize(4);
	assert(array1.GetSize() == 4);
	try
	{
		array1[8] = 5;
	}
	catch (const std::out_of_range& e)
	{
		assert(strcmp(e.what(), "Index [8] was out of range") == 0);
	}
	catch (...)
	{
		assert(0);
	}

	// add and remove elements tests
	auto array9 = DynamicArray<int>(items, 22);
	array9.Prepend(3);
	assert(array9[0] == 3);
	for (int i = 1; i < 23; i++)
	{
		assert(array9[i] == items[i - 1]);
	}

	array9.Remove(0);
	for (int i = 0; i < 22; i++)
	{
		assert(array9[i] == items[i]);
	}

	array9.Append(-45);
	assert(array9[22] == -45);
	for (int i = 0; i < 22; i++)
	{
		assert(array9[i] == items[i]);
	}
	array9.Remove(22);

	array9.InsertAt(9, 23);
	for (int i = 0; i < 19; i++)
	{
		if (i < 9)
		{
			assert(array9[i] == items[i]);
		}
		else if (i == 9)
		{
			assert(array9[i] == 23);
		}
		else
		{
			assert(array9[i] == items[i - 1]);
		}
	}
	array9.Remove(9);
	for (int i = 0; i < 22; i++)
	{
		assert(array9[i] == items[i]);
	}


	// subarray tests
	DynamicArray<int> array3 = DynamicArray<int>(items, 22);
	DynamicArray<int>* array4 = array3.GetSubArray(4, 18);
	for (int i = 0, j = 4; j < 18; i++, j++)
	{
		assert(array4->Get(i) == items[j]);
	}

	// concatenation tests
	int* nums1 = RandIntSeq(13);
	int* nums2 = RandIntSeq(32);
	DynamicArray<int> array5 = DynamicArray<int>(nums1, 13);
	DynamicArray<int> array6 = DynamicArray<int>(nums2, 32);
	DynamicArray<int>* array7 = array5.Concat(&array6);
	for (int i = 0; i < 13; i++)
	{
		assert(nums1[i] == array7->Get(i));
	}
	for (int i = 13, j = 0; j < 32; i++, j++)
	{
		assert(nums2[j] == array7->Get(i));
	}

	// find elem test
	int nums3[6] = { 4,-32,67,6,0,24 };
	DynamicArray<int> array8 = DynamicArray<int>(nums3, 6);
	assert(array8.FindFirst(IsDivisibleBySix) == 3);


}

void test_ListSequence()
{
	int* nums1 = RandIntSeq(17);
	auto seq1 = ListSequence<int>(nums1, 17);
	for (int i = 0; i < 17; i++)
	{
		assert(nums1[i] == seq1[i]);
	}

	auto seq2 = seq1;
	auto seq3 = ListSequence<int>(seq1);
	for (int i = 0; i < 17; i++)
	{
		assert(seq1[i] == seq2[i] && seq1[i] == seq3[i]);
	}

	// get set test
	int* nums2 = RandIntSeq(25);
	auto seq4 = ListSequence<int>(nums2, 25);

	assert(seq4.GetFirst() == seq4[0] && seq4.GetLast() == seq4[seq4.GetLength() - 1] && seq4.GetLast() == nums2[24]);
	for (int i = 0; i < seq4.GetLength(); i++)
	{
		assert(seq4[i] == seq4.Get(i));
	}

	int new_val1 = seq4[3] - 45;
	seq4[3] = seq4[3] - 45;
	assert(seq4[3] == new_val1);

	int new_val2 = seq4[7] + 67;
	seq4.Set(7, new_val2);
	assert(seq4[7] == new_val2);


	// insert test
	int* nums3 = RandIntSeq(27);
	auto seq5 = ListSequence<int>(nums3, 27);

	seq5.Append(-111);
	assert(seq5.GetLength() == 28 && seq5[27] == -111);

	seq5.Prepend(999);
	assert(seq5.GetLength() == 29 && seq5[0] == 999);

	for (int i = 1, j = 0; i < 27; i++, j++)
	{
		assert(nums3[j] == seq5[i]);
	}

	auto seq6 = ListSequence<int>(nums3, 27);
	seq6.InsertAt(0, 5);
	assert(seq6.GetLength() == 28 && seq6[5] == 0);
	for (int i = 0, j = 0; i < 27; i++, j++)
	{
		if (j == 5)
		{
			j++;
		}
		assert(nums3[i] == seq6[j]);
	}


	//concatenation test
	int* nums4 = RandIntSeq(46);
	auto seq7 = ListSequence<int>(nums1, 19);
	auto seq8 = ListSequence<int>(nums4, 46);
	auto* seq9 = seq7.Concat(&seq8);
	assert(seq9->GetLength() == 19 + 46);
	for (int i = 0; i < seq9->GetLength(); i++)
	{
		if (i < 19)
		{
			assert(seq9->Get(i) == seq7[i]);
		}
		else
		{
			assert(seq9->Get(i) == seq8[i - 19]);
		}
	}

	//subsequence test
	int nums5[8] = { 7,4,92,-156,91,45,-2,0 };
	auto seq10 = ListSequence<int>(nums5, 8);
	ListSequence<int>* seq11 = seq10.GetSubsequence(3, 6);
	for (int i = 0, j = 3; j < 6; i++, j++)
	{
		assert(seq11->Get(i) == nums5[j]);
	}

	// map test
	int nums6[9] = { 9,23,-78,45,16,37,0,12,8 };
	double nums7[9] = { 9,23,-78,45,16,37,0,12,8 };
	for (double& i : nums7)
	{
		i /= 2;
	}
	auto seq12 = ListSequence<int>(nums6, 9);
	ListSequence<double>* seq13 = seq12.Map(DivideByTwo);
	for (int i = 0; i < 9; i++)
	{
		assert(static_cast<double>(nums7[i]) - seq13->Get(i) < 0.000001);
	}

	// reduce test
	int* nums8 = RandIntSeq(19);
	int nums8_sum = 0;
	for (int i = 0; i < 19; i++)
	{
		nums8_sum += nums8[i];
	}
	auto seq14 = ListSequence<int>(nums8, 19);
	int seq14_sum = seq14.Reduce(SumBin, 0);
	assert(nums8_sum == seq14_sum);

	// zip unzip test
	int* ints = RandIntSeq(9);
	double* doubles = RandDoubleSeq(7);
	auto intSeq = ListSequence<int>(ints, 9);
	auto doubleSeq = ListSequence<double>(doubles, 7);
	auto* zipped = Zip(&intSeq, &doubleSeq);
	for (int i = 0; i < zipped->GetLength(); i++)
	{
		assert(std::get<0>(zipped->Get(i)) == ints[i]);
		assert(std::get<1>(zipped->Get(i)) == doubles[i]);
	}
	auto unzipped = Unzip(zipped);
	auto* seq_buff1 = std::get<0>(unzipped);
	for (int i = 0; i < seq_buff1->GetLength(); i++)
	{
		assert(seq_buff1->Get(i) == intSeq[i]);
	}
	auto* seq_buff2 = std::get<1>(unzipped);
	for (int i = 0; i < seq_buff2->GetLength(); i++)
	{
		assert(seq_buff2->Get(i) == doubleSeq[i]);
	}

	// where test
	int nums9[10] = { 4,6,0,19,-56,458,-13, 66, 222222, -89 };
	int nums9_div[4] = { 6,0,66, 222222 };
	auto seq15 = ListSequence<int>(nums9, 10);
	auto* seq16 = seq15.Where(IsDivisibleBySix);
	assert(seq16->GetLength() == 4);
	for (int i = 0; i < 4; i++)
	{
		assert(nums9_div[i] == seq16->Get(i));
	}
}


void test_ArraySequence()
{
	int* nums1 = RandIntSeq(19);
	auto seq1 = ArraySequence<int>(nums1, 19);
	for (int i = 0; i < 19; i++)
	{
		assert(nums1[i] == seq1[i]);
	}

	auto seq2 = seq1;
	auto seq3 = ArraySequence<int>(seq1);
	for (int i = 0; i < 19; i++)
	{
		assert(seq1[i] == seq2[i] && seq1[i] == seq3[i]);
	}

	// get set test
	int* nums2 = RandIntSeq(25);
	auto seq4 = ArraySequence<int>(nums2, 25);

	assert(seq4.GetFirst() == seq4[0] && seq4.GetLast() == seq4[seq4.GetLength() - 1] && seq4.GetLast() == nums2[24]);
	for (int i = 0; i < seq4.GetLength(); i++)
	{
		assert(seq4[i] == seq4.Get(i));
	}

	int new_val1 = seq4[3] - 45;
	seq4[3] = seq4[3] - 45;
	assert(seq4[3] == new_val1);

	int new_val2 = seq4[7] + 67;
	seq4.Set(7, new_val2);
	assert(seq4[7] == new_val2);


	// insert test
	int* nums3 = RandIntSeq(27);
	auto seq5 = ArraySequence<int>(nums3, 27);

	seq5.Append(-111);
	assert(seq5.GetLength() == 28 && seq5[27] == -111);

	seq5.Prepend(999);
	assert(seq5.GetLength() == 29 && seq5[0] == 999);

	for (int i = 1, j = 0; i < 27; i++, j++)
	{
		assert(nums3[j] == seq5[i]);
	}

	auto seq6 = ArraySequence<int>(nums3, 27);
	seq6.InsertAt(0, 5);
	assert(seq6.GetLength() == 28 && seq6[5] == 0);
	for (int i = 0, j = 0; i < 27; i++, j++)
	{
		if (j == 5)
		{
			j++;
		}
		assert(nums3[i] == seq6[j]);
	}


	//concatenation test
	int* nums4 = RandIntSeq(46);
	auto seq7 = ArraySequence<int>(nums1, 19);
	auto seq8 = ArraySequence<int>(nums4, 46);
	auto* seq9 = seq7.Concat(&seq8);
	assert(seq9->GetLength() == 19 + 46);
	for (int i = 0; i < seq9->GetLength(); i++)
	{
		if (i < 19)
		{
			assert(seq9->Get(i) == seq7[i]);
		}
		else
		{
			assert(seq9->Get(i) == seq8[i - 19]);
		}
	}

	//subsequence test
	int nums5[8] = { 7,4,92,-156,91,45,-2,0 };
	auto seq10 = ArraySequence<int>(nums5, 8);
	auto* seq11 = seq10.GetSubsequence(3, 6);
	for (int i = 0, j = 3; j < 6; i++, j++)
	{
		assert(seq11->Get(i) == nums5[j]);
	}

	// map test
	int nums6[9] = { 9,23,-78,45,16,37,0,12,8 };
	double nums7[9] = { 9,23,-78,45,16,37,0,12,8 };
	for (double& i : nums7)
	{
		i /= 2;
	}
	auto seq12 = ArraySequence<int>(nums6, 9);
	ArraySequence<double>* seq13 = seq12.Map(DivideByTwo);
	for (int i = 0; i < 9; i++)
	{
		assert(static_cast<double>(nums7[i]) - seq13->Get(i) < 0.000001);
	}

	// reduce test
	int* nums8 = RandIntSeq(19);
	int nums8_sum = 0;
	for (int i = 0; i < 19; i++)
	{
		nums8_sum += nums8[i];
	}
	auto seq14 = ArraySequence<int>(nums8, 19);
	int seq14_sum = seq14.Reduce(SumBin, 0);
	assert(nums8_sum == seq14_sum);

	// zip unzip test
	int* ints = RandIntSeq(9);
	double* doubles = RandDoubleSeq(7);
	auto intSeq = ArraySequence<int>(ints, 9);
	auto doubleSeq = ArraySequence<double>(doubles, 7);
	auto* zipped = Zip(&intSeq, &doubleSeq);
	for (int i = 0; i < zipped->GetLength(); i++)
	{
		assert(std::get<0>(zipped->Get(i)) == ints[i]);
		assert(std::get<1>(zipped->Get(i)) == doubles[i]);
	}
	auto unzipped = Unzip(zipped);
	auto* seq_buff1 = std::get<0>(unzipped);
	for (int i = 0; i < seq_buff1->GetLength(); i++)
	{
		assert(seq_buff1->Get(i) == intSeq[i]);
	}
	auto* seq_buff2 = std::get<1>(unzipped);
	for (int i = 0; i < seq_buff2->GetLength(); i++)
	{
		assert(seq_buff2->Get(i) == doubleSeq[i]);
	}

	// where test
	int nums9[10] = { 4,6,0,19,-56,458,-13, 66, 222222, -89 };
	int nums9_div[4] = { 6,0,66, 222222 };
	auto seq15 = ArraySequence<int>(nums9, 10);
	auto* seq16 = seq15.Where(IsDivisibleBySix);
	assert(seq16->GetLength() == 4);
	for (int i = 0; i < 4; i++)
	{
		assert(nums9_div[i] == seq16->Get(i));
	}
}

// get sequence of random integer values
int* RandIntSeq(int size)
{
	srand(time(NULL));
	int* seq = new int[size];
	for (int i = 0; i < size; i++)
	{
		*(seq + i) = rand() % 50 - 25;
	}

	return seq;
}

// get sequence of random double values
double* RandDoubleSeq(int size)
{
	srand(time(NULL));
	double* seq = new double[size];
	for (int i = 0; i < size; i++)
	{
		*(seq + i) = static_cast<double>((rand() % 100 - 50)/4.6);
	}

	return seq;
}


// special functions for tests
bool IsDivisibleBySix(const int num)
{
	return (num % 6) == 0;
}

double DivideByTwo(const int num)
{
	double new_num = static_cast<double>(num) / 2;
	return new_num;
}

int SumBin(const int n, const int m)
{
	return n + m;
}