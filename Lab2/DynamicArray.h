#pragma once


#include <iostream>
#include <new>
#include <string>

#include "ICollection.h"
#include "IEnumerable.h"

template <class T>
class DynamicArrayEnumerator;

template <class T>
class DynamicArray:public ICollection<T>, public IEnumerable<T>
{
private:
	int size_;
	T* first_;
public:

	// constructors
	DynamicArray(const T* items, int size);
	DynamicArray(int size);
	DynamicArray(const DynamicArray<T>& dynamicArray);

	// operators
	DynamicArray& operator= (const DynamicArray& d_array);
	T& operator[] (const int index);

	// destructor
	~DynamicArray();

	// decomposition
	int GetSize() const;
	T Get(int index) const;
	void Set(int index, T value);
	DynamicArray<T>* GetSubArray(int start_index, int end_index);

	// change array
	void Resize(int new_size);
	void Prepend(T item);
	void Append(T item);
	void InsertAt(int index, T item);
	void Remove(int index);
	
	// operations
	DynamicArray<T>* Concat(DynamicArray<T>* array);
	int FindFirst(bool (*condition)(const T));
	int FindLast(bool (*condition)(const T));
	DynamicArray<int>* FindAll(bool (*condition)(const T));
	DynamicArray<DynamicArray<T>*>* Split(bool (*condition)(const T));
	
	struct Iterator
	{
	private:
		T* elem_ptr;
		DynamicArray* array_;
	public:
		Iterator() :elem_ptr(nullptr), array_(nullptr) {}
		Iterator(DynamicArray* array) : elem_ptr(array->first_) , array_(array){}
		Iterator(T* elem, DynamicArray* array) : elem_ptr(elem), array_(array) {}

		T operator*() const;
		Iterator& operator++();
		Iterator& operator--();
		Iterator& operator++(int);
		Iterator& operator--(int);
		Iterator& operator+(int n);
		Iterator& operator-(int n);
		Iterator& operator+=(int n);
		Iterator& operator-=(int n);

		friend bool operator==(const Iterator& it1, const Iterator& it2)
		{
			return it1.elem_ptr == it2.elem_ptr;
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2)
		{
			return it1.elem_ptr != it2.elem_ptr;
		}

		Iterator Reset()
		{
			return array_->begin();
		}

	};

	Iterator begin();
	Iterator end();

	T GetElement(int count) override
	{
		return Get(count);
	}
	int GetCount() override
	{
		return GetSize();
	}

	DynamicArrayEnumerator<T>* GetEnumerator() override;

};

template <class T>
class DynamicArrayEnumerator: public IEnumerator<T>
{
public:
	typename DynamicArray<T>::Iterator iterator_;

	DynamicArrayEnumerator(typename DynamicArray<T>::Iterator iterator):iterator_(iterator){}
	
	void MoveNext() override
	{
		++iterator_;
	}
	void Reset() override
	{
		iterator_ = iterator_.Reset();
	}
	T GetCurrent() override
	{
		return *iterator_;
	}
};

template <class T>
DynamicArrayEnumerator<T>* DynamicArray<T>::GetEnumerator()
{
	auto* en = new DynamicArrayEnumerator<T>(begin());
	return en;
}


template <class T>
DynamicArray<T>::DynamicArray(const T* items, int size)
{
	if (size <= 0)
	{
		throw std::invalid_argument("Invalid size of array: " + std::to_string(size));
	}
	size_ = size;
	first_ = new T[size];

	std::copy_n(items, size, first_);

}

template <class T>
DynamicArray<T>::DynamicArray(int size)
{
	if (size < 0)
	{
		throw std::invalid_argument("Invalid size of array: " + std::to_string(size));
	}
	if (size == 0)
	{
		size_ = 0;
		first_ = nullptr;
	}
	else
	{
		size_ = size;
		first_ = new T[size];
	}
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamicArray)
{
	size_ = dynamicArray.size_;
	if (size_ == 0)
	{
		first_ = nullptr;
	}
	else
	{
		first_ = new T[size_];

		std::copy_n(dynamicArray.first_, size_, first_);

	}
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator= (const DynamicArray& d_array)
{
	// self assignment check
	if (this == &d_array)
	{
		DynamicArray& ref = this;
		return ref;
	}

	size_ = d_array.size_;

	delete[] first_;
	first_ = new T[size_];

	std::copy_n(d_array.first_, size_, first_);

	DynamicArray& ref = this;
	return ref;
}

template <class T>
DynamicArray<T>::~DynamicArray()
{
	delete[] first_;
}

template <class T>
int DynamicArray<T>::GetSize() const
{
	return size_;
}

template <class T>
T DynamicArray<T>::Get(int index) const
{
	if (index >= size_ || index < 0)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}


	return *(first_ + index);
}

template <class T>
void DynamicArray<T>::Set(int index, T value)
{
	if (index >= size_ || index < 0)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}
	*(first_ + index) = value;
}

template <class T>
void DynamicArray<T>::Resize(int new_size)
{
	if (new_size < 0)
	{
		throw std::invalid_argument("Invalid size of array: " + std::to_string(new_size));
	}
	if (new_size == 0)
	{
		size_ = 0;
		delete[] first_;
		first_ = nullptr;
	}
	else
	{
		const int elements_to_copy = size_ > new_size ? new_size : size_;
		size_ = new_size;

		T* buff = first_;
		first_ = new T[new_size];
		if (elements_to_copy)
		{
			std::copy_n(buff, elements_to_copy, first_);
		}

		delete[] buff;
	}
}

template <class T>
void DynamicArray<T>::Prepend(T item)
{
	T* new_elements = new T[size_ + 1];
	*(new_elements) = item;
	std::copy_n(first_, size_, new_elements + 1);

	delete[] first_;
	first_ = new_elements;
	size_++;

}

template <class T>
void DynamicArray<T>::Append(T item)
{
	Resize(size_ + 1);
	Set(size_ - 1, item);
}

template <class T>
void DynamicArray<T>::InsertAt(int index, T item)
{
	if (index >= size_ || index < 0)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}
	if (index == 0)
	{
		Prepend(item);
	}
	else if (index == size_ - 1)
	{
		Append(item);
	}
	else
	{
		T* new_elements = new T[size_ + 1];
		*(new_elements + index) = item;

		std::copy_n(first_, index, new_elements);
		std::copy_n(first_ + index, size_ - index, new_elements + index + 1);

		delete[] first_;
		this->first_ = new_elements;
		size_++;
	}
}

template <class T>
void DynamicArray<T>::Remove(int index)
{
	if (index >= size_ || index < 0)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}

	T* buff = new T[size_ - index - 1];
	std::copy_n(first_ + index + 1, size_ - index - 1, buff);
	std::copy_n(buff, size_ - index - 1, first_ + index);
	Resize(size_ - 1);
}

template <class T>
DynamicArray<T>* DynamicArray<T>::GetSubArray(int start_index, int end_index)
{
	if (start_index > end_index || start_index < 0 || end_index < 0 ||
		start_index >= size_ || end_index > size_)
	{
		throw std::out_of_range("Index was out of range");
	}

	if (start_index == end_index)
	{
		return new DynamicArray(0);
	}

	int new_size = end_index - start_index;
	DynamicArray* subArray = new DynamicArray(new_size);
	if (new_size == 0)
	{
		return subArray;
	}

	subArray->first_ = new T[new_size];
	std::copy_n(first_ + start_index, new_size, subArray->first_);

	return subArray;
}

template <class T>
DynamicArray<T>* DynamicArray<T>::Concat(DynamicArray<T>* array)
{
	DynamicArray* new_array = new DynamicArray(size_ + array->size_);

	new_array->first_ = new T[new_array->size_];
	std::copy_n(first_, size_, new_array->first_);
	std::copy_n(array->first_, array->size_, new_array->first_ + size_);

	return new_array;
}

template <class T>
int DynamicArray<T>::FindFirst(bool (*condition)(const T))
{
	for (int i = 0; i < size_; i++)
	{
		if (condition(Get(i)))
		{
			return i;
		}
	}

	return -1;
}

template <class T>
int DynamicArray<T>::FindLast(bool (*condition)(const T))
{
	for (int i = size_ - 1; i >= 0; i--)
	{
		if (condition(Get(i)))
		{
			return i;
		}
	}

	return -1;
}

template <class T>
DynamicArray<int>* DynamicArray<T>::FindAll(bool (*condition)(const T))
{
	DynamicArray<int>* found_items = new DynamicArray<int>(0);
	DynamicArray* copy_array = new DynamicArray<T>(*this);
	int new_occurrence = 0;
	int removed_elements = 0;
	while (true)
	{
		new_occurrence = copy_array->FindFirst(condition);
		if (new_occurrence == -1)
		{
			break;
		}
		found_items->Append(new_occurrence + removed_elements);
		if (new_occurrence == copy_array->GetSize() - 1)
		{
			break;
		}

		copy_array = copy_array->GetSubArray(new_occurrence + 1, copy_array->GetSize());
		removed_elements += new_occurrence + 1;
	}

	return found_items;
}

 template <class T>
DynamicArray<DynamicArray<T>*>* DynamicArray<T>::Split(bool (*condition)(const T))
{
	auto* to_split = new DynamicArray<T>(*this);
	auto* splitted_array = new DynamicArray<DynamicArray<T>*>(0);

	if (size_ == 0)
	{
		return splitted_array;
	}

	auto* split_indexes = to_split->FindAll(condition);

	split_indexes->Prepend(-1);
	split_indexes->Append(to_split->GetSize());
	for (int i = 0; i < split_indexes->GetSize() - 1; i++)
	{
		DynamicArray<T>* new_part;
		if (split_indexes->Get(i) == to_split->GetSize() - 1)
		{
			new_part = new DynamicArray<T>(0);
		}
		else
		{
			new_part = to_split->GetSubArray(split_indexes->Get(i) + 1, split_indexes->Get(i + 1));
		}
		splitted_array->Append(new_part);
	}

	delete to_split;

	return splitted_array;
}

template <class T>
T& DynamicArray<T>::operator[] (const int index)
{
	if (index >= size_)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}
	T& el_ref = *(first_ + index);
	return el_ref;
}

template <class T>
T DynamicArray<T>::Iterator::operator*() const
{
	return *elem_ptr;
}

template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator++()
{
	if(*this == array_->end())
	{
		throw std::out_of_range("Iterator gone out of range");
	}
	++elem_ptr;
	return *this;
}

template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator--()
{
	if (*this == array_->begin())
	{
		throw std::out_of_range("Iterator gone out of range");
	}
	--elem_ptr;
	return *this;
}
template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator++(int)
{
	Iterator tmp = *this;
	++(*this);
	return tmp;
}

template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator--(int)
{
	Iterator tmp = *this;
	--(*this);
	return tmp;
}

template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator+(int n)
{
	Iterator* new_iterator = new Iterator(this->elem_ptr);
	for (int i = 0; i < n; i++)
	{
		++(*new_iterator);
	}

	return *new_iterator;
}

template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator-(int n)
{
	Iterator* new_iterator = new Iterator(this->elem_ptr);
	for (int i = 0; i < n; i++)
	{
		--(*new_iterator);
	}

	return *new_iterator;
}

template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator+=(int n)
{
	for (int i = 0; i < n; i++)
	{
		++(*this);
	}

	return *this;
}


template <class T>
DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator-=(int n)
{
	for (int i = 0; i < n; i++)
	{
		--(*this);
	}

	return *this;
}	

template <class T>
DynamicArray<T>::Iterator DynamicArray<T>::begin()
{
	return DynamicArray<T>::Iterator(this);
}

template <class T>
DynamicArray<T>::Iterator DynamicArray<T>::end()
{
	return DynamicArray<T>::Iterator(first_ + size_, this);
}
