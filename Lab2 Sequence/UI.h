#pragma once

#include "DynamicArray.h"
#include "LinkedList.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Vector.h"
#include "Matrix.h"

template<class T>
std::ostream& operator<< (std::ostream& cout, const DynamicArray<T>& d_array)
{
	cout << "{";
	for (int i = 0; i < d_array.GetSize(); i++)
	{
		cout << " " << d_array.Get(i);
		if (i != d_array.GetSize() - 1)
		{
			cout << ",";
		}
	}
	cout << " }";
	
	return cout;
}

template<class T>
std::ostream& operator<< (std::ostream& cout, const LinkedList<T>& l_list)
{
	cout << "{";
	for (int i = 0; i < l_list.GetLength(); i++)
	{
		cout << " " << l_list.Get(i);
		if (i != l_list.GetLength() - 1)
		{
			cout << ",";
		}
	}
	cout << " }";

	return cout;
}

template<class T>
std::ostream& operator<< (std::ostream& cout, const ArraySequence<T>& sequence)
{
	cout << "{";
	for (int i = 0; i < sequence.GetLength(); i++)
	{
		cout << " " << sequence.Get(i);
		if (i != sequence.GetLength() - 1)
		{
			cout << ",";
		}
	}
	cout << " }";

	return cout;
}

template<class T>
std::ostream& operator<< (std::ostream& cout, const ListSequence<T>& sequence)
{
	cout << "{";
	for (int i = 0; i < sequence.GetLength(); i++)
	{
		cout << " " << sequence.Get(i);
		if (i != sequence.GetLength() - 1)
		{
			cout << ",";
		}
	}
	cout << " }";

	return cout;
}

template<class T>
std::ostream& operator<< (std::ostream& cout, const Vector<T>& vector)
{
	cout << "{";
	for (int i = 0; i < vector.GetDimension(); i++)
	{
		cout << " " << vector.GetCoordinate(i);
		
		if (i != vector.GetDimension() - 1)
		{
			cout << ",";
		}
	}
	cout << " }";

	return cout;
}

template<class T>
std::ostream& operator<< (std::ostream& cout, const Matrix<T>& matrix)
{
	for (int i = 0; i < matrix.GetHeight(); i++)
	{
		for (int j = 0; j < matrix.GetWidth(); j++)
		{
			if(j!=0)
			{
				cout << " ";
			}
			cout << matrix(i, j);
			
		}
		cout << std::endl;
	}


	return cout;
}


