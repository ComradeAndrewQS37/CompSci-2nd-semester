#include "VectorTest.h"

#include <complex>

constexpr double eps = 0.000001;

void vector_test_construction_get_set()
{
	auto vec1 = Vector<int>(6, 0);
	assert(vec1.GetDimension() == 6);
	for (int i = 0; i < 6; i++)
	{
		assert(vec1.GetCoordinate(i) == 0);
	}
	
	int* nums2 = RandIntSeq(46);
	auto vec2 = Vector<int>(46, nums2);
	for (int i = 0; i < 46; i++)
	{
		assert(vec2.GetCoordinate(i) == nums2[i]);
	}
	try
	{
		int t = vec2.GetCoordinate(100);
		assert(0);
	}
	catch(std::out_of_range&)
	{
		assert(1);
	}
	catch(...)
	{
		assert(0);
	}

	auto vec3 = Vector<double>();
	assert(vec3.GetDimension() == 0);

	double* nums4 = RandDoubleSeq(37);
	auto vec4 = Vector<double>(37, nums4);
	auto old_coordinate = vec4.GetCoordinate(4);
	auto vec5 = vec4.ChangeCoordinate(4, old_coordinate - 456.89);
	assert(vec5.GetCoordinate(4) - (old_coordinate - 456.89) < eps);
	
}

void vector_test_operations()
{
	int* nums1 = RandIntSeq(59);
	auto vec1 = Vector<int>(59, nums1);
	int* nums2 = RandIntSeq(59);
	auto vec2 = Vector<int>(59, nums2);
	auto vec3 = vec1.VectorSum(vec2);
	for (int i = 0; i < 59; i++)
	{
		assert(vec3.GetCoordinate(i) == vec1.GetCoordinate(i) + vec2.GetCoordinate(i));
	}

	auto vec4 = Vector<int>(17, 0);
	try
	{
		auto t = vec1.VectorSum(vec4);
		assert(0);
	}
	catch(std::invalid_argument&)
	{
		assert(1);
	}
	catch(...)
	{
		assert(0);
	}

	auto vec5 = vec1 + vec2;
	for (int i = 0; i < 59; i++)
	{
		assert(vec5[i] == vec1[i] + vec2[i]);
	}
	auto vec6 = 3 * vec1 - vec2 * 17;
	for (int i = 0; i < 59; i++)
	{
		assert(vec6[i] == 3 * vec1[i] - 17 * vec2[i]);
	}
	auto vec7 = vec1.ScalarMult(3);
	for (int i = 0; i < 59; i++)
	{
		assert(vec7[i] == 3 * vec1[i]);
	}
	auto vec8 = -vec1;
	for (int i = 0; i < 59; i++)
	{
		assert(vec8[i] == (-1) * vec1[i]);
	}

	double* nums9 = RandDoubleSeq(37);
	auto vec9 = Vector<double>(37, nums9);
	double* nums10 = RandDoubleSeq(37);
	auto vec10 = Vector<double>(37, nums10);

	auto vec11 = 3.589 * vec9 * 8 - vec10 * 2.001;
	for (int i = 0; i < 37; i++)
	{
		assert(vec11[i] - (3.589 * vec9[i] * 8 - vec10[i] * 2.001) < eps);
	}

	double prod = 0.0;
	for (int i = 0; i < 37; i++)
	{
		prod += vec9[i] * vec10[i];
	}
	assert(vec9.DotProduct(vec10) - prod < eps);

	int* nums12 = RandIntSeq(77);
	auto vec12 = Vector<int>(77, nums12);
	int vec12_norm = 0;
	for (int i = 0; i < 77; i++)
	{
		vec12_norm += abs(nums12[i]);
	}
	assert(vec12_norm == vec12.Norm());

	double* nums13 = RandDoubleSeq(68);
	auto vec13 = Vector<double>(68, nums13);
	double vec13_norm = 0.0;
	for (int i = 0; i < 68; i++)
	{
		vec13_norm += nums13[i] * nums13[i];
	}
	assert(std::sqrt(vec13_norm) - vec13.Norm() < eps);

	using complex = std::complex<double>;
	complex nums14[3] = { complex(11.51,5.003), complex(-6,0), complex(1.15,2) };
	auto vec14_norm = std::abs(nums14[0]);
	for (auto& i : nums14)
	{
		vec14_norm = vec14_norm > std::abs(i)? vec14_norm:std::abs(i);
	}
	auto vec14 = Vector<complex>(3,nums14);
	assert(vec14_norm - vec14.Norm() < eps);

	auto vec15 = vec14 * 13;
	for (int i = 0; i < 3; i++)
	{
		assert(nums14[i].real() * 13 - vec15[i].real() < eps && nums14[i].imag() * 13 - vec15[i].imag() < eps);
	}
	auto vec16 = -9.89 * vec14;
	for (int i = 0; i < 3; i++)
	{
		assert(-9.89 * nums14[i].real() - vec16[i].real() < eps && -9.89 * nums14[i].imag() - vec16[i].imag() < eps);
	}

}
