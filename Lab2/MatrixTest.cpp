#include "MatrixTest.h"

#include <complex>

constexpr double eps = 0.000001;

void matrix_test_construction_get_set()
{
	auto m1 = Matrix<int>(3,4,1);
	assert(m1.GetHeight() == 3);
	assert(m1.GetWidth() == 4);

	for(int i = 0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			assert(m1(i, j) == 1);
		}
	}

	auto m2 = m1.ChangeElement(2, 1, -20);
	assert(m2.GetElement(2, 1) == -20);
	

	int* nums2 = RandIntSeq(12);
	
	auto m3 = Matrix<int>(4, 3, nums2);
	{
		int k = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				assert(m3.GetElement(i, j) == nums2[k]);
				k++;
			}
		}
	}
	auto m4 = Matrix<int>(4, 3, nums2, false);
	{
		int k = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				assert(m4(j, i) == nums2[k]);
				k++;
			}
		}
	}

	double* nums3 = RandDoubleSeq(20);
	auto seq1 = ArraySequence<double>(nums3,20);
	auto m5 = Matrix<double>(5, 4, seq1);
	{
		int k = 0;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				assert(m5(i, j) - seq1[k] < eps);
				k++;
			}
		}
	}
	
	
}
void matrix_test_operations()
{
	double nums1[9] = { 9.56,2.3,-78.345,45.3,16,37.4,0,12,8.44 };
	auto m1 = Matrix<double>(3, 3, nums1);
	double nums2[9] = { 942.4,124,0.04,45.5,6,3.237,0,1.2,45.53 };
	auto m2 = Matrix<double>(3, 3, nums2);
	auto m3 = 3 * m1 + 2 * m2 - 7.5 * (m1 - m2);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			assert(m3(i, j) - (3 * m1(i, j) + 2 * m2(i, j) - 7.5 * (m1(i, j) - m2(i, j))) < eps);
		}
	}

	int nums[9] = { 0,2,4,5,1,6,1,2,2 };
	auto m4 = Matrix<int>(3, 3, nums);
	assert(m4.Det() == 28);
	
	using complex = std::complex<int>;
	complex nums3[4] = { complex(1,2),complex(0,-12), complex(-41,0), complex(11,2) };
	auto m5 = Matrix<complex>(2, 2, nums3);
	assert(m5.Norm() == std::complex<int>(41, 0));
	

	auto m6= m4.InterchangeColumns(1, 2);
	for (int i = 0; i < 3; i++)
	{
		assert(m6(i, 1) == m4(i, 2));
	}
	auto m7 = m4.InterchangeRows(0, 1);
	for (int i = 0; i < 3; i++)
	{
		assert(m7(0, i) == m4(1, i));
	}
	auto m8 = m4.MultiplyColumn(1, -1);
	for (int i = 0; i < 3; i++)
	{
		assert(m8(i, 1) == -1*m4(i, 1));
	}
	auto m9 = m4.MultiplyRow(2, 3);
	for (int i = 0; i < 3; i++)
	{
		assert(m9(2, i) == 3* m4(2, i));
	}
	auto m10 = m4.MultiplyRowAndAdd(0, -1, 2);
	for (int i = 0; i < 3; i++)
	{
		assert(m10(2, i) == -1 * m4(0, i) + m4(2,i));
	}
	auto m11 = m4.MultiplyColumnAndAdd(2, 4, 1);
	for (int i = 0; i < 3; i++)
	{
		assert(m11(i, 1) == 4 * m4(i, 2) + m4(i,1));
	}

	
}
