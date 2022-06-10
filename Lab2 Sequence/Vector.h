#pragma once

#include "ArraySequence.h"
#include "myConcepts.h"
#include <string>
#include <cstddef>
#include <complex>



template<class T>
class Vector
{
private:
	ArraySequence<T>* values_;

	// special functions for map reduce
	static T MultiplyTuplePair(const std::tuple<T, T>);
	static T SelfMultiply(const T a);
	static T SumTwoElements(const T a, const T b);
	static T ComplexNumberNormSquare(const T a);
	static T AbsoluteValue(const T a);
	static T ChooseGreater(const T a, const T b);
public:
	// constructors
	Vector();
	Vector(int dimension, T value);
	Vector(int dimension, T* values);
	Vector(const Vector<T>* vector);
	Vector(ArraySequence<T>* values);

	// operators
	const T& operator[] (const int index) const;
	Vector<T>& operator-() const;
	Vector<T>& operator+(const Vector& other_vec) const;
	Vector<T>& operator-(const Vector& other_vec) const;
	Vector<T>& operator*(int scalar) const;
	Vector<T>& operator*(double scalar) const;
	//Vector<T>& operator*(int scalar, const Vector<T>& vec);
	//Vector<T>& operator*(double scalar, const Vector<T>& vec);
	friend bool operator==(const Vector<T>& vec1, const Vector<T>& vec2);
	friend bool operator!=(const Vector<T>& vec1, const Vector<T>& vec2);

	// decomposition
	int GetDimension() const;
	T GetCoordinate(int index) const;
	Vector<T>& ChangeCoordinate(int index, T new_coordinate) const;

	// operations
	Vector<T>& VectorSum(const Vector<T>& other_vector) const;
	Vector<T>& ScalarMult(int scalar) const requires IntMultipliable<T>;
	Vector<T>& ScalarMult(double scalar) const requires DoubleNumbersMultipliable<T>;
	T DotProduct(const Vector<T>& other_vector) const;
	T Norm() const requires std::is_integral_v<T>;
	T Norm() const requires std::is_floating_point_v<T>;
	auto Norm() const->decltype(std::abs(GetCoordinate(0))) requires Complex<T>
	{
		if (GetDimension() == 0)
		{
			throw std::invalid_argument("Cannot find norm of zero-dimensional vectors");
		}

		decltype(std::abs(GetCoordinate(0))) max_abs_value = std::abs(GetCoordinate(0));
		for(int i=1;i<GetDimension();i++)
		{
			max_abs_value = max_abs_value > std::abs(GetCoordinate(i)) ? max_abs_value : std::abs(GetCoordinate(i));
		}

		return max_abs_value;
	}
	
};

template<class T>
Vector<T>::Vector()
{
	values_ = new ArraySequence<T>();
}

template<class T>
Vector<T>::Vector(int dimension, T value)
{
	if (dimension == 0)
	{
		values_ = new ArraySequence<T>();
	}
	values_ = new ArraySequence<T>(value, dimension);
}

template<class T>
Vector<T>::Vector(int dimension, T* values)
{
	if (dimension == 0)
	{
		values_ = new ArraySequence<T>();
	}
	values_ = new ArraySequence<T>(values, dimension);
}

template<class T>
Vector<T>::Vector(ArraySequence<T>* values)
{
	values_ = values;
}

template<class T>
Vector<T>::Vector(const Vector<T>* vector)
{
	values_ = new ArraySequence<T>(*vector->values_);
}

template<class T>
int Vector<T>::GetDimension() const
{
	return values_->GetLength();
}

template<class T>
T Vector<T>::GetCoordinate(int index) const
{
	return values_->Get(index);
}

template<class T>
const T& Vector<T>::operator[] (const int index) const
{
	const T& ref = this->values_->operator[](index);
	return ref;
}

template<class T>
Vector<T>& Vector<T>::ChangeCoordinate(int index, T new_coordinate) const
{
	auto* new_vec = new Vector(this);
	new_vec->values_->Set(index, new_coordinate);

	return *new_vec;
}

template<class T>
Vector<T>& Vector<T>::VectorSum(const Vector<T>& other_vector) const
{
	if (GetDimension() != other_vector.GetDimension())
	{
		throw std::invalid_argument("Only vectors of same dimension can be summed");
	}

	const int vec_size = GetDimension();

	auto* new_vec = new Vector(this);
	for (int i = 0; i < vec_size; i++)
	{
		new_vec->values_->Set(i, new_vec->values_->Get(i) + other_vector.GetCoordinate(i));
	}

	return *new_vec;
}


template<class T>
Vector<T>& Vector<T>::ScalarMult(int scalar) const requires IntMultipliable<T>
{
	auto* new_vec = new Vector(this);
	for (int i = 0; i < GetDimension(); i++)
	{
		new_vec->values_->Set(i, static_cast<T>(new_vec->values_->Get(i) * scalar));
	}
	return *new_vec;
}

template<class T>
Vector<T>& Vector<T>::ScalarMult(double scalar) const requires DoubleNumbersMultipliable<T>
{
	auto* new_vec = new Vector(this);
	for (int i = 0; i < GetDimension(); i++)
	{
		new_vec->values_->Set(i, static_cast<T>( new_vec->values_->Get(i) * scalar));
	}
	return *new_vec;
}

template<class T>
T Vector<T>::MultiplyTuplePair(const std::tuple<T, T> pair)
{
	return std::get<0>(pair) * std::get<1>(pair);
}

template<class T>
T Vector<T>::SelfMultiply(const T a)
{
	return a * a;
}

template<class T>
T Vector<T>::SumTwoElements(const T a, const T b)
{
	return a + b;
}

template<class T>
T Vector<T>::ComplexNumberNormSquare(const T a)
{
	return std::conj(a) * a;
}

template<class T>
T Vector<T>::AbsoluteValue(const T a)
{
	if (a < 0)
	{
		return -a;
	}
	return a;
}

template<class T>
T Vector<T>::ChooseGreater(const T a, const T b)
{
	T abs_a = std::abs(a);
	T abs_b = std::abs(b);

	return abs_a > abs_b ? abs_a : abs_b;
}




template<class T>
T Vector<T>::DotProduct(const Vector<T>& other_vector) const
{
	if (GetDimension() != other_vector.GetDimension())
	{
		throw std::invalid_argument("Cannot find dot product of vectors with different dimension");
	}

	if(GetDimension() == 0)
	{
		throw std::invalid_argument("Cannot find dot product of zero-dimensional vectors");
	}
	
	ArraySequence<std::tuple<T,T>>* pairs = Zip(values_, other_vector.values_);
	ArraySequence<T>* multiplied_pairs = pairs->Map(MultiplyTuplePair);
	T result = multiplied_pairs->Reduce(SumTwoElements, static_cast<T>(GetCoordinate(0) * 0.0));

	return result;
}



template<class T>
T Vector<T>::Norm() const requires std::is_integral_v<T>
{
	if (GetDimension() == 0)
	{
		throw std::invalid_argument("Cannot find norm of zero-dimensional vectors");
	}
	
	ArraySequence<T>* abs_values = values_->Map(AbsoluteValue);
	T accumulator = GetCoordinate(0) * static_cast<T>(0);
	T result = abs_values->Reduce(SumTwoElements, accumulator);

	return result;
}

template<class T>
T Vector<T>::Norm() const requires std::is_floating_point_v<T>
{
	if (GetDimension() == 0)
	{
		throw std::invalid_argument("Cannot find norm of zero-dimensional vectors");
	}

	ArraySequence<T>* squared_values = values_->Map(SelfMultiply);
	T accumulator = GetCoordinate(0) * static_cast<T>(0.0);
	T result = static_cast<T>(std::sqrt(squared_values->Reduce(SumTwoElements, accumulator)));

	return result;
}



template<class T>
Vector<T>& Vector<T>::operator*(int scalar) const
{
	return this->ScalarMult(scalar);
}

template<class T>
Vector<T>& operator*(int scalar, const Vector<T>& vec)
{
	return vec.ScalarMult(scalar);

}

template<class T>
Vector<T>& Vector<T>::operator*(double scalar) const
{
	return this->ScalarMult(scalar);
}

template<class T>
Vector<T>& operator*(double scalar, const Vector<T>& vec)
{
	return vec.ScalarMult(scalar);

}

template<class T>
Vector<T>& Vector<T>::operator-() const
{
	return this->ScalarMult(-1);
}


template<class T>
Vector<T>& Vector<T>::operator+(const Vector& other_vec) const
{
	return this->VectorSum(other_vec);
}

template<class T>
Vector<T>& Vector<T>::operator-(const Vector& other_vec) const
{
	return this->VectorSum(-other_vec);
}

template<class T>
bool operator==(const Vector<T>& vec1, const Vector<T>& vec2)
{
	if(vec1.GetDimension() != vec2.GetDimension())
	{
		return false;
	}

	for (int i = 0; i < vec1.GetDimension(); i++)
	{
		if(vec1.GetCoordinate() != vec2.GetCoordinate())
		{
			return false;
		}
	}

	return true;
}

template<class T>
bool operator!=(const Vector<T>& vec1, const Vector<T>& vec2)
{
	if (vec1.GetDimension() != vec2.GetDimension())
	{
		return true;
	}

	for (int i = 0; i < vec1.GetDimension(); i++)
	{
		if (vec1.GetCoordinate() != vec2.GetCoordinate())
		{
			return true;
		}
	}

	return false;
}
