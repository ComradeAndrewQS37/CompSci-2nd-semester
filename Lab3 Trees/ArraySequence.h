#pragma once

#include <tuple>

#include "DynamicArray.h"
#include "Sequence.h"

#include "ICollection.h"
#include "IEnumerable.h"


template <class T>
class ArraySequenceEnumerator;

template <class T>
class ArraySequence : public Sequence<T>, public ICollection<T>, public IEnumerable<T>
{
private:
	DynamicArray<T>* array_buffer_;
public:
	// constructors
	ArraySequence(const T* items, int count);
	ArraySequence();
	ArraySequence(T item, int count);
	ArraySequence(const ArraySequence<T>& array_seq);
	ArraySequence(const ArraySequence<T>* array_seq);

	// operators
	ArraySequence& operator= (const ArraySequence& array_seq);
	T& operator[] (const int index) override;

	// destructor
	~ArraySequence();

	// decomposition
	T GetFirst() const override;
	T GetLast() const override;
	T Get(int index) const override;
	void Set(int index, T item) override;
	ArraySequence<T>* GetSubsequence(int start_index, int end_index) override;
	int GetLength() const override;

	// operations
	void Append(T item) override;
	void Prepend(T item) override;
	void InsertAt(T item, int index) override;
	void Remove(int index) override;

	ArraySequence<T>* Concat(Sequence<T>* sequence) override;
	ArraySequence<ArraySequence<T>*>* Split(bool (*condition)(const T));

	template <typename newType>
	ArraySequence<newType>* Map(newType(*transform)(const T));

	ArraySequence<T>* Where(bool (*filter)(const T)) override;
	int FindFirst(bool (*condition)(const T));
	ArraySequence<int>* FindAll(bool (*condition)(const T));

	struct Iterator
	{
	private:
		typename DynamicArray<T>::Iterator array_iterator;
	public:
		explicit Iterator(ArraySequence* array) : array_iterator(DynamicArray<T>::Iterator(array->array_buffer_)) {}
		explicit Iterator(ArraySequence* array, int n) : array_iterator(DynamicArray<T>::Iterator(array->array_buffer_) + n) {}
		explicit Iterator(typename DynamicArray<T>::Iterator iterator) : array_iterator(iterator) {}

		T operator*() const { return *array_iterator; }

		Iterator& operator++() { ++array_iterator; return *this; }

		Iterator& operator--() { --array_iterator; return *this; }

		Iterator& operator++(int)
		{
			auto tmp = this;
			++(*this);
			return tmp;
		}

		Iterator& operator--(int)
		{
			auto tmp = this;
			--(*this);
			return tmp;
		}

		Iterator& operator+(int n) { return Iterator(array_iterator + n); }

		Iterator& operator-(int n) { return Iterator(array_iterator - n); }

		Iterator& operator+=(int n) { array_iterator += n; return *this; }

		Iterator& operator-=(int n) { array_iterator -= n; return *this; }

		friend bool operator==(const Iterator& it1, const Iterator& it2)
		{
			return it1.array_iterator == it2.array_iterator;
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2)
		{
			return it1.array_iterator != it2.array_iterator;
		}

		Iterator Reset()
		{
			return Iterator(array_iterator.Reset());
		}
	};

	Iterator begin()
	{
		return Iterator(array_buffer_->begin());
	}

	Iterator end()
	{
		return Iterator(array_buffer_->end());
	}

	T GetElement(int count) override
	{
		return Get(count);
	}

	int GetCount() override
	{
		return GetLength();
	}

	ArraySequenceEnumerator<T>* GetEnumerator() override;

};

template <class T>
class ArraySequenceEnumerator : public IEnumerator<T>
{
public:
	typename ArraySequence<T>::Iterator iterator_;

	ArraySequenceEnumerator(typename ArraySequence<T>::Iterator iterator) :iterator_(iterator) {}

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
ArraySequenceEnumerator<T>* ArraySequence<T>::GetEnumerator()
{
	auto* en = new ArraySequenceEnumerator<T>(begin());
	return en;
}

template<typename... Types>
ArraySequence<std::tuple<Types...>>* Zip(ArraySequence<Types>*... sequences)
{
	auto* zipped_sequence = new ArraySequence<std::tuple<Types...>>();
	int i = 0;
	while (true)
	{
		try
		{
			auto new_tuple = std::make_tuple<Types...>(sequences->Get(i)...);
			zipped_sequence->Append(new_tuple);
			++i;
		}
		catch (std::out_of_range&)
		{
			break;
		}
	}

	return zipped_sequence;
}


template <typename T1, typename T2>
std::tuple<ArraySequence<T1>*, ArraySequence<T2>*> Unzip(ArraySequence<std::tuple<T1, T2>>* zipped)
{
	auto* sequence1 = new ArraySequence<T1>();
	auto* sequence2 = new ArraySequence<T2>();
	typename ArraySequence<std::tuple<T1, T2>>::Iterator iterator = zipped->begin();
	typename ArraySequence<std::tuple<T1, T2>>::Iterator end = zipped->end();
	while (iterator != end)
	{
		sequence1->Append(std::get<0>(*iterator));
		sequence2->Append(std::get<1>(*iterator));

		++iterator;
	}

	auto unzipped = std::make_tuple(sequence1, sequence2);
	return unzipped;

}

template <class T>
ArraySequence<T>::ArraySequence(const T* items, int count)
{
	array_buffer_ = new DynamicArray<T>(items, count);
}

template <class T>
ArraySequence<T>::ArraySequence()
{
	array_buffer_ = new DynamicArray<T>(0);
}

template <class T>
ArraySequence<T>::ArraySequence(T item, int count)
{
	array_buffer_ = new DynamicArray<T>(count);
	for (int i = 0; i < count; i++)
	{
		array_buffer_->Set(i, item);
	}
}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& array_seq)
{
	DynamicArray<T> buff_ref = *array_seq.array_buffer_;
	array_buffer_ = new DynamicArray<T>(buff_ref);
}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>* array_seq)
{
	DynamicArray<T> buff_ref = *array_seq->array_buffer_;
	array_buffer_ = new DynamicArray<T>(buff_ref);
}

template <class T>
ArraySequence<T>& ArraySequence<T>::operator= (const ArraySequence& array_seq)
{
	if (this == &array_seq)
	{
		return this;
	}

	array_buffer_ = array_seq.array_buffer_;
}

template <class T>
T& ArraySequence<T>::operator[] (const int index)
{
	return array_buffer_->operator[](index);
}

template <class T>
ArraySequence<T>::~ArraySequence()
{
	delete array_buffer_;
}

template <class T>
T ArraySequence<T>::GetFirst() const
{
	return array_buffer_->Get(0);
}

template <class T>
T ArraySequence<T>::GetLast() const
{
	return array_buffer_->Get(this->array_buffer_->GetSize() - 1);
}

template <class T>
T ArraySequence<T>::Get(int index) const
{
	return array_buffer_->Get(index);
}

template <class T>
void ArraySequence<T>::Set(int index, T item)
{
	array_buffer_->Set(index, item);
}

template <class T>
ArraySequence<T>* ArraySequence<T>::GetSubsequence(int start_index, int end_index)
{
	ArraySequence* subsequence = new ArraySequence();
	subsequence->array_buffer_ = array_buffer_->GetSubArray(start_index, end_index);
	return subsequence;
}

template <class T>
int ArraySequence<T>::GetLength() const
{
	return array_buffer_->GetSize();
}

template <class T>
void ArraySequence<T>::Append(T item)
{
	array_buffer_->Append(item);
}

template <class T>
void ArraySequence<T>::Prepend(T item)
{
	array_buffer_->Prepend(item);
}

template <class T>
void ArraySequence<T>::InsertAt(T item, int index)
{
	array_buffer_->InsertAt(index, item);
}

template <class T>
void ArraySequence<T>::Remove(int index)
{
	array_buffer_->Remove(index);
}

template <class T>
ArraySequence<T>* ArraySequence<T>::Concat(Sequence<T>* sequence)
{
	ArraySequence* arr_sequence = static_cast<ArraySequence<T>*>(sequence);
	auto* newSequence = new ArraySequence();
	newSequence->array_buffer_ = array_buffer_->Concat(arr_sequence->array_buffer_);
	return newSequence;
}

template <class T>
ArraySequence<ArraySequence<T>*>* ArraySequence<T>::Split(bool (*condition)(const T))
{
	ArraySequence* newSequence = new ArraySequence<ArraySequence<T>*>();
	newSequence->array_buffer_ = array_buffer_->Split(condition);
	return newSequence;
}

template <class T>
template <typename newType>
ArraySequence<newType>* ArraySequence<T>::Map(newType(*transform)(const T))
{
	auto* new_sequence = new ArraySequence<newType>();
	for (int i = 0; i < GetLength(); i++)
	{
		new_sequence->Append(transform(Get(i)));
	}

	return new_sequence;
}

template <class T>
ArraySequence<T>* ArraySequence<T>::Where(bool (*filter)(const T))
{
	auto* new_sequence = new ArraySequence<T>();
	Iterator it = begin();
	while (it != end())
	{
		if (filter(*it))
		{
			new_sequence->Append(*it);
		}
		++it;
	}

	return new_sequence;
}

template <class T>
int ArraySequence<T>::FindFirst(bool (*condition)(const T))
{
	return array_buffer_->FindFirst(condition);
}

template <class T>
ArraySequence<int>* ArraySequence<T>::FindAll(bool (*condition)(const T))
{
	ArraySequence* found_items = new ArraySequence<int>();
	found_items->array_buffer_ = array_buffer_->FindAll(condition);

	return found_items;
}