#pragma once
#include "ArraySequence.h"
#include "myConcepts.h"
#include <complex>

template<class T>
class Matrix
{
private:
	int height_;
	int width_;
	ArraySequence<T>* elements_;

	// special functions for map reduce
	static T SumTupleElements(const std::tuple<T> pair);
	static T SubtractTupleElements(const std::tuple<T> pair);

public:
	// constructors
	Matrix(const Matrix& matrix);
	Matrix(const Matrix* matrix);
	Matrix(int height, int width, T fill_value);
	Matrix(int height, int width, const T** values, bool byRows = true);
	Matrix(int height, int width, const ArraySequence<ArraySequence<T>*>& values, bool byRows = true);
	Matrix(int height, int width, const T* lin_values, bool byRows = true);
	Matrix(int height, int width, const ArraySequence<T>& lin_values, bool byRows = true);
	
	// operators
	T operator()(int row, int col) const;
	Matrix<T>& operator-() const;
	Matrix<T>& operator+(const Matrix& other_matrix) const;
	Matrix<T>& operator-(const Matrix& other_matrix) const;
	auto& operator*(int scalar) const requires IntMultipliable<T>
	{
		return this->ScalarMult(scalar);
	}

	auto& operator*(double scalar) const requires DoubleNumbersMultipliable<T>
	{
		return this->ScalarMult(scalar);
		
	}
	friend bool operator==(const Matrix<T>& matrix1, const Matrix<T>& matrix2);
	friend bool operator!=(const Matrix<T>& matrix1, const Matrix<T>& matrix2);

	// decomposition
	int GetHeight() const;
	int GetWidth() const;
	T GetElement(int row, int col) const;
	Matrix<T>& ChangeElement(int row, int col, T new_element) const;

	// operations
	Matrix<T>& MatrixSum(const Matrix<T>& other_matrix) const;
	Matrix<T>& MatrixSubtract(const Matrix<T>& other_matrix) const;
	T Det() const;
	T Norm() const;
	
	auto& ScalarMult(int scalar) const requires IntMultipliable<T>
	{
		auto* new_sequence = new ArraySequence<decltype(scalar* GetElement(0, 0))>(scalar * GetElement(0, 0), height_ * width_);

		for (int i = 0; i < height_; i++)
		{
			for (int j = 0; j < width_; j++)
			{
				new_sequence->Set(i * width_ + j, GetElement(i, j) * scalar);

			}
			
		}
		auto* new_matrix = new Matrix(height_, width_, new_sequence);
		return *new_matrix;
	}
	auto& ScalarMult(double scalar) const requires DoubleNumbersMultipliable<T>
	{
		auto* new_sequence = new ArraySequence<decltype(scalar* GetElement(0, 0))>(scalar * GetElement(0, 0), height_ * width_);

		for (int i = 0; i < height_; i++)
		{
			for (int j = 0; j < width_; j++)
			{
				new_sequence->Set(i * width_ + j, GetElement(i, j) * scalar);

			}

		}
		auto* new_matrix = new Matrix(height_, width_, new_sequence);
		return *new_matrix;
	}

	// elementary row operations
	Matrix<T>& InterchangeRows(int row1, int row2) const;
	Matrix<T>& MultiplyRow(int row, int scalar) const requires IntMultipliable<T>;
	Matrix<T>& MultiplyRow(int row, double scalar) const requires DoubleNumbersMultipliable<T>;
	Matrix<T>& MultiplyRowAndAdd(int row_to_mult, int scalar, int other_row) const requires IntMultipliable<T>;
	Matrix<T>& MultiplyRowAndAdd(int row_to_mult, double scalar, int other_row) const requires DoubleNumbersMultipliable<T>;

	// elementary column operations
	Matrix<T>& InterchangeColumns(int col1, int col2) const;
	Matrix<T>& MultiplyColumn(int col, int scalar) const requires IntMultipliable<T>;
	Matrix<T>& MultiplyColumn(int col, double scalar) const requires DoubleNumbersMultipliable<T>;
	Matrix<T>& MultiplyColumnAndAdd(int col_to_mult, int scalar, int other_col) const requires IntMultipliable<T>;
	Matrix<T>& MultiplyColumnAndAdd(int col_to_mult, double scalar, int other_col) const requires DoubleNumbersMultipliable<T>;
};


template<class T>
Matrix<T>::Matrix(const Matrix& matrix)
{
	height_ = matrix.height_;
	width_ = matrix.width_;
	elements_ = new ArraySequence<T>(matrix.elements_);
}

template<class T>
Matrix<T>::Matrix(const Matrix* matrix)
{
	height_ = matrix->height_;
	width_ = matrix->width_;
	elements_ = new ArraySequence<T>(matrix->elements_);
}

template<class T>
Matrix<T>::Matrix(int height, int width, T fill_value) {
	if (height <= 0 || width <= 0)
	{
		throw std::invalid_argument("Matrix height and width must be positive");
	}
	height_ = height;
	width_ = width;
	elements_ = new ArraySequence<T>(fill_value, height * width);
}

template<class T>
Matrix<T>::Matrix(int height, int width, const T** values, bool byRows) {
	if (height <= 0 || width <= 0)
	{
		throw std::invalid_argument("Matrix height and width must be positive");
	}
	height_ = height;
	width_ = width;

	if (byRows)
	{
		elements_ = new ArraySequence<T>(*values, width);
		for (int i = 1; i < height; i++)
		{
			elements_->Concat(new ArraySequence<T>(*(values + i), width));
		}
	}
	else
	{
		elements_ = new ArraySequence<T>(values[0][0], values * width);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				elements_->Set(i * width + j, *(*(values + j) + i));
			}
		}
	}
}

template<class T>
Matrix<T>::Matrix(int height, int width, const ArraySequence<ArraySequence<T>*>& values, bool byRows) {
	if (height <= 0 || width <= 0)
	{
		throw std::invalid_argument("Matrix height and width must be positive");
	}
	height_ = height;
	width_ = width;

	if(byRows && values.GetLength() != height || !byRows && values.GetLength() != width)
	{
		throw std::invalid_argument("Matrix cannot be created: wrong size of passed Sequence");
	}
	
	
	if (byRows)
	{
		elements_ = new ArraySequence<T>(*values[0]);
		for (int i = 1; i < height; i++)
		{
			elements_->Concat(new ArraySequence<T>(*values[i], width));
		}
	}
	else
	{
		elements_ = new ArraySequence<T>(values[0]->Get(0), height * width);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				elements_->Set(i * width + j, values[j]->Get(i));
			}
		}
	}
}

template<class T>
Matrix<T>::Matrix(int height, int width, const T* lin_values, bool byRows) {
	if (height <= 0 || width <= 0)
	{
		throw std::invalid_argument("Matrix height and width must be positive");
	}
	
	height_ = height;
	width_ = width;
	
	if (byRows)
	{
		elements_ = new ArraySequence<T>(lin_values, width * height);
	}
	else
	{
		elements_ = new ArraySequence<T>(lin_values[0], height * width);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				elements_->Set(i * width + j, *(lin_values + j * height + i));
			}
		}
	}
}

template<class T>
Matrix<T>::Matrix(int height, int width, const ArraySequence<T>& values, bool byRows) {
	if (height <= 0 || width <= 0)
	{
		throw std::invalid_argument("Matrix height and width must be positive");
	}
	height_ = height;
	width_ = width;

	if (values.GetLength() != height * width)
	{
		throw std::invalid_argument("Matrix cannot be created: wrong size of passed Sequence");
	}
	if(height * width == 0)
	{
		this->elements_ = new ArraySequence<T>();
		return;
	}
	if (byRows)
	{
		elements_ = new ArraySequence<T>(values);
	}
	else
	{
		elements_ = new ArraySequence<T>(values.Get(0), height * width);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				elements_->Set(i * width + j, values.Get(j * height + i));
			}
		}
	}
}

template <class T>
T Matrix<T>::operator()(int row, int col) const
{
	if (row >= height_ || col >= width_)
	{
		throw std::invalid_argument("Row and column values were out of range");
	}
	
	return elements_->Get(row * width_ + col);
}


template <class T>
Matrix<T>& Matrix<T>::operator-() const
{
	return this->ScalarMult(-1);
}

template <class T>
Matrix<T>& Matrix<T>::operator+(const Matrix& other_matrix) const
{
	return this->MatrixSum(other_matrix);
}

template <class T>
Matrix<T>& Matrix<T>::operator-(const Matrix& other_matrix) const
{
	return this->MatrixSubtract(other_matrix);
}


template <class T>
auto& operator*(int scalar, const Matrix<T>& matrix) requires IntMultipliable<T>
{
	return matrix.ScalarMult(scalar);
}

template <class T>
auto& operator*(double scalar, const Matrix<T>& matrix) requires DoubleNumbersMultipliable<T>
{
	return matrix.ScalarMult(scalar);
}

template <class T>
bool operator==(const Matrix<T>& matrix1, const Matrix<T>& matrix2)
{
	if(matrix1.height_ != matrix2.height_ || matrix1.width_ !=matrix2.width_)
	{
		return false;
	}
	for (int i = 0; i < matrix1.height_ * matrix1.width_; i++)
	{
		if(matrix1.elements_[i] != matrix2.elements_[i])
		{
			return false;
		}
	}

	return true;
}

template <class T>
bool operator!=(const Matrix<T>& matrix1, const Matrix<T>& matrix2)
{
	if (matrix1.height_ != matrix2.height_ || matrix1.width_ != matrix2.width_)
	{
		return true;
	}
	for (int i = 0; i < matrix1.height_ * matrix1.width_; i++)
	{
		if (matrix1.elements_[i] != matrix2.elements_[i])
		{
			return true;
		}
	}

	return false;
}

template <class T>
int Matrix<T>::GetHeight() const
{
	return height_;
}
template <class T>
int Matrix<T>::GetWidth() const
{
	return width_;
}

template <class T>
T Matrix<T>::GetElement(int row, int col) const
{
	if(row>=height_ || col>=width_)
	{
		throw std::exception("Row and column values were out of range");
	}
	return elements_->Get(row * width_ + col);
}

template <class T>
Matrix<T>& Matrix<T>::ChangeElement(int row, int col, T new_element) const
{
	if (row >= height_ || col >= width_)
	{
		throw std::exception("Row and column values were out of range");
	}
	
	auto* new_matrix = new Matrix(*this);
	new_matrix->elements_->Set(row * width_ + col, new_element);

	return *new_matrix;
}

template <class T>
T Matrix<T>::SumTupleElements(const std::tuple<T> pair)
{
	return std::get<0>(pair) + std::get<1>(pair);
}

template <class T>
T Matrix<T>::SubtractTupleElements(const std::tuple<T> pair)
{
	return std::get<0>(pair) - std::get<1>(pair);
}

template <class T>
Matrix<T>& Matrix<T>::MatrixSum(const Matrix<T>& other_matrix) const
{
	if (height_ != other_matrix.height_ || width_ != other_matrix.width_)
	{
		throw std::invalid_argument("Only matrices with same sizes can be summed");
	}
	auto* new_matrix = new Matrix<T>(this);

	for (int i = 0; i < width_ * height_; i++)
	{
		new_matrix->elements_->operator[](i) += other_matrix.elements_->operator[](i);
	}

	return *new_matrix;
	
}

template <class T>
Matrix<T>& Matrix<T>::MatrixSubtract(const Matrix<T>& other_matrix) const
{
	if (height_ != other_matrix.height_ || width_ != other_matrix.width_)
	{
		throw std::invalid_argument("Only matrices with same sizes can be subtracted");
	}
	auto* new_matrix = new Matrix<T>(this);

	for (int i = 0; i < width_ * height_; i++)
	{
		new_matrix->elements_->operator[](i) -= other_matrix.elements_->operator[](i);
	}

	return *new_matrix;

}

template <class T>
T Matrix<T>::Det() const
{
	if(width_!=height_)
	{
		throw std::invalid_argument("Cannot find determinant of not square matrix");
	}

	T zero = static_cast<T>(0.0 * elements_->Get(0));
	T det = zero;

	if (width_ == 1)
	{
		det += elements_->Get(0);
	}
	if (width_ == 2)
	{
		det += elements_->Get(0) * elements_->Get(3) - elements_->Get(1) * elements_->Get(2);
	}
	else{

		for (int i = 0; i < width_; i++)
		{

			if (this->GetElement(0, i) != zero)
			{
				
				auto new_elements = ArraySequence<T>(*elements_->GetSubsequence(width_, height_ * width_));
				for (int j = height_ - 2; j >= 0; j--)
				{
					new_elements.Remove(j * width_ + i);
				}

				auto new_minor = Matrix(height_ - 1, width_ - 1, new_elements);
				int sign = (i % 2 == 0) ? 1 : -1;
				if (sign == 1)
				{
					det += this->GetElement(0, i) * new_minor.Det();
				}else
				{
					det += this->GetElement(0, i) * -new_minor.Det();
				}
				
			}
		}
	}
	return det;
}

template <class T>
T Matrix<T>::Norm() const
{
	auto max_value = std::abs(elements_->Get(0));
	auto iterator = elements_->begin();
	while(iterator !=elements_->end())
	{
		max_value = max_value > std::abs(*iterator) ? max_value : std::abs(*iterator);
		++iterator;
	}

	return max_value;
}


template <class T>
Matrix<T>& Matrix<T>::InterchangeRows(int row1, int row2) const
{
	if (row1 < 0 || row2 < 0 || row1 >= height_ || row2 >= height_)
	{
		throw std::invalid_argument("Invalid values for row numbers");
	}
	
	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < width_; i++)
	{
		T& value_1 = new_matrix->elements_->operator[](row1* width_ + i);
		T& value_2 = new_matrix->elements_->operator[](row2* width_ + i);
		std::swap(value_1, value_2);
	}

	return *new_matrix;
}

template <class T>
Matrix<T>& Matrix<T>::MultiplyRow(int row, int scalar) const requires IntMultipliable<T>
{
	if (row < 0 ||row >= height_ )
	{
		throw std::invalid_argument("Invalid value for row number");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < width_; i++)
	{
		T& value = new_matrix->elements_->operator[](row* width_ + i);
		value *= scalar;
	}

	return *new_matrix;
	
}
template <class T>
Matrix<T>& Matrix<T>::MultiplyRow(int row, double scalar) const requires DoubleNumbersMultipliable<T>
{
	if (row < 0 || row >= height_)
	{
		throw std::invalid_argument("Invalid value for row number");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < width_; i++)
	{
		T& value = new_matrix->elements_->operator[](row * width_ + i);
		value *= scalar;
	}

	return *new_matrix;
}
template <class T>
Matrix<T>& Matrix<T>::MultiplyRowAndAdd(int row_to_mult, int scalar, int other_row) const requires IntMultipliable<T>
{
	if (row_to_mult < 0 || other_row < 0 || row_to_mult >= height_ || other_row >= height_)
	{
		throw std::invalid_argument("Invalid values for row numbers");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < width_; i++)
	{
		T value_to_mult = new_matrix->elements_->operator[](row_to_mult * width_ + i);
		value_to_mult *= scalar;
		T& other_value = new_matrix->elements_->operator[](other_row * width_ + i);
		other_value += value_to_mult;
	}

	return *new_matrix;

}
template <class T>
Matrix<T>& Matrix<T>::MultiplyRowAndAdd(int row_to_mult, double scalar, int other_row) const requires DoubleNumbersMultipliable<T>
{
	if (row_to_mult < 0 || other_row < 0 || row_to_mult >= height_ || other_row >= height_)
	{
		throw std::invalid_argument("Invalid values for row numbers");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < width_; i++)
	{
		T value_to_mult = new_matrix->elements_->operator[](row_to_mult* width_ + i);
		value_to_mult *= scalar;
		T& other_value = new_matrix->elements_->operator[](other_row* width_ + i);
		other_value += value_to_mult;
	}

	return *new_matrix;
}

template <class T>
Matrix<T>& Matrix<T>::InterchangeColumns(int col1, int col2) const
{
	if (col1 < 0 || col2 < 0 || col1 >= height_ || col2 >= height_)
	{
		throw std::invalid_argument("Invalid values for column numbers");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < height_; i++)
	{
		T& value_1 = new_matrix->elements_->operator[](i* width_ + col1);
		T& value_2 = new_matrix->elements_->operator[](i* width_ + col2);
		std::swap(value_1, value_2);
	}

	return *new_matrix;
}
template <class T>
Matrix<T>& Matrix<T>::MultiplyColumn(int col, int scalar) const requires IntMultipliable<T>
{
	if (col < 0 || col >= height_)
	{
		throw std::invalid_argument("Invalid value for column number");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < height_; i++)
	{
		T& value = new_matrix->elements_->operator[](i* width_ + col);
		value *= scalar;
	}

	return *new_matrix;
}
template <class T>
Matrix<T>& Matrix<T>::MultiplyColumn(int col, double scalar) const requires DoubleNumbersMultipliable<T>
{
	if (col < 0 || col >= height_)
	{
		throw std::invalid_argument("Invalid value for column number");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < height_; i++)
	{
		T& value = new_matrix->elements_->operator[](i* width_ + col);
		value *= scalar;
	}

	return *new_matrix;
}
template <class T>
Matrix<T>& Matrix<T>::MultiplyColumnAndAdd(int col_to_mult, int scalar, int other_col) const requires IntMultipliable<T>
{
	if (col_to_mult < 0 || other_col < 0 || col_to_mult >= height_ || other_col >= height_)
	{
		throw std::invalid_argument("Invalid values for column numbers");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < height_; i++)
	{
		T value_to_mult = new_matrix->elements_->operator[](i * width_ + col_to_mult);
		value_to_mult *= scalar;
		T& other_value = new_matrix->elements_->operator[](i * width_ + other_col);
		other_value += value_to_mult;
	}

	return *new_matrix;
}
template <class T>
Matrix<T>& Matrix<T>::MultiplyColumnAndAdd(int col_to_mult, double scalar, int other_col) const requires DoubleNumbersMultipliable<T>
{
	if (col_to_mult < 0 || other_col < 0 || col_to_mult >= height_ || other_col >= height_)
	{
		throw std::invalid_argument("Invalid values for column numbers");
	}

	Matrix<T>* new_matrix = new Matrix<T>(this);
	for (int i = 0; i < height_; i++)
	{
		T value_to_mult = new_matrix->elements_->operator[](i* width_ + col_to_mult);
		value_to_mult *= scalar;
		T& other_value = new_matrix->elements_->operator[](i* width_ + other_col);
		other_value += value_to_mult;
	}

	return *new_matrix;
}