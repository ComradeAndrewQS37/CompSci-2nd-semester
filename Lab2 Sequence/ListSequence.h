#pragma once

#include "LinkedList.h"
#include "Sequence.h"

#include "ICollection.h"
#include "IEnumerable.h"


template <class T>
class ListSequenceEnumerator;

template <class T>
class ListSequence : public Sequence<T>, public ICollection<T>, public IEnumerable<T>
{
private:
	LinkedList<T>* list_buffer_;
public:
	// constructors
	ListSequence(T* items, int count);
	ListSequence();
	ListSequence(const ListSequence<T>& array_seq);

	// operators
	ListSequence& operator= (const ListSequence& array_seq);
	T& operator[] (const int index) override;

	// destructor
	~ListSequence();

	// decomposition
	T GetFirst() const override;
	T GetLast() const override;
	T Get(int index) const override;
	void Set(int index, T item) override;
	ListSequence<T>* GetSubsequence(int start_index, int end_index) override;
	int GetLength() const override;

	// operations
	void Append(T item) override;
	void Prepend(T item) override;
	void InsertAt(T item, int index) override;
	void Remove(int index) override;

	ListSequence<T>* Concat(Sequence<T>* sequence) override;
	ListSequence<ListSequence<T>*>* Split(bool (*condition)(const T));
	
	template <typename newType>
	ListSequence<newType>* Map(newType(*transform)(const T));

	ListSequence<T>* Where(bool (*filter)(const T)) override;
	int FindFirst(bool (*condition)(const T));
	ListSequence<int>* FindAll(bool (*condition)(const T));

	struct Iterator
	{
	private:
		typename LinkedList<T>::Iterator list_iterator;
	public:
		explicit Iterator(ListSequence* list) : list_iterator(LinkedList<T>::Iterator(list->list_buffer_)) {}
		explicit Iterator(ListSequence* list, int n) : list_iterator(LinkedList<T>::Iterator(list->list_buffer_) + n) {}
		explicit Iterator(typename LinkedList<T>::Iterator iterator) : list_iterator(iterator){}

		T operator*() const { return *list_iterator; }

		Iterator& operator++() { ++list_iterator; return *this; }

		Iterator& operator--() { --list_iterator; return *this; }

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

		Iterator& operator+(int n) { return Iterator(list_iterator + n); }

		Iterator& operator-(int n) { return Iterator(list_iterator - n); }

		Iterator& operator+=(int n) { list_iterator += n; return *this; }

		Iterator& operator-=(int n) { list_iterator -= n; return *this; }

		friend bool operator==(const Iterator& it1, const Iterator& it2)
		{
			return it1.list_iterator == it2.list_iterator;
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2)
		{
			return it1.list_iterator != it2.list_iterator;
		}

		Iterator Reset()
		{
			return Iterator(list_iterator.Reset());
		}
	};

	Iterator begin()
	{
		return Iterator(list_buffer_->begin());
	}

	Iterator end()
	{
		return Iterator(list_buffer_->end());
	}

	T GetElement(int count) override
	{
		return Get(count);
	}

	int GetCount() override
	{
		return GetLength();
	}

	ListSequenceEnumerator<T>* GetEnumerator() override;

};

template <class T>
class ListSequenceEnumerator : public IEnumerator<T>
{
public:
	typename ListSequence<T>::Iterator iterator_;

	ListSequenceEnumerator(typename ListSequence<T>::Iterator iterator) :iterator_(iterator) {}

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
ListSequenceEnumerator<T>* ListSequence<T>::GetEnumerator()
{
	auto* en = new ListSequenceEnumerator<T>(begin());
	return en;
}

template<typename... Types>
ListSequence<std::tuple<Types...>>* Zip(ListSequence<Types>*... sequences)
{
	auto* zipped_sequence = new ListSequence<std::tuple<Types...>>();
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
std::tuple<ListSequence<T1>*, ListSequence<T2>*> Unzip(ListSequence<std::tuple<T1, T2>>* zipped)
{
	auto* sequence1 = new ListSequence<T1>();
	auto* sequence2 = new ListSequence<T2>();
	typename ListSequence<std::tuple<T1, T2>>::Iterator iterator = zipped->begin();
	typename ListSequence<std::tuple<T1, T2>>::Iterator end = zipped->end();
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
ListSequence<T>::ListSequence(T* items, int count)
{
	list_buffer_ = new LinkedList<T>(items, count);
}

template <class T>
ListSequence<T>::ListSequence()
{
	list_buffer_ = new LinkedList<T>();
}

template <class T>
ListSequence<T>::ListSequence(const ListSequence<T>& array_seq)
{
	LinkedList<T> buff_ref = *array_seq.list_buffer_;
	list_buffer_ = new LinkedList<T>(buff_ref);
}

template <class T>
ListSequence<T>& ListSequence<T>::operator= (const ListSequence<T>& array_seq)
{
	if (this == &array_seq)
	{
		return this;
	}

	list_buffer_ = array_seq.list_buffer_;
}

template <class T>
ListSequence<T>::~ListSequence()
{
	delete list_buffer_;
}

template <class T>
T ListSequence<T>::GetFirst() const
{
	return list_buffer_->GetFirst();
}

template <class T>
T ListSequence<T>::GetLast() const
{
	return list_buffer_->GetLast();
}

template <class T>
T ListSequence<T>::Get(int index) const
{
	return list_buffer_->Get(index);
}

template <class T>
void ListSequence<T>::Set(int index, T item)
{
	list_buffer_->Set(index, item);
}

template <class T>
T& ListSequence<T>::operator[] (const int index)
{
	return list_buffer_->operator[](index);
}

template <class T>
ListSequence<T>* ListSequence<T>::GetSubsequence(int start_index, int end_index)
{
	ListSequence* subsequence = new ListSequence();
	subsequence->list_buffer_ = list_buffer_->GetSubList(start_index, end_index);
	return subsequence;
}

template <class T>
int ListSequence<T>::GetLength() const
{
	return list_buffer_->GetLength();
}

template <class T>
void ListSequence<T>::Append(T item)
{
	list_buffer_->Append(item);
}

template <class T>
void ListSequence<T>::Prepend(T item)
{
	list_buffer_->Prepend(item);
}

template <class T>
void ListSequence<T>::InsertAt(T item, int index)
{
	list_buffer_->InsertAt(item, index);
}

template <class T>
void ListSequence<T>::Remove(int index)
{
	list_buffer_->Remove(index);
}

template <class T>
ListSequence<T>* ListSequence<T>::Concat(Sequence<T>* sequence)
{
	ListSequence* list_sequence = static_cast<ListSequence<T>*>(sequence);
	ListSequence* newSequence = new ListSequence();
	newSequence->list_buffer_ = list_buffer_->Concat(list_sequence->list_buffer_);
	return newSequence;
}

template <class T>
ListSequence<ListSequence<T>*>* ListSequence<T>::Split(bool (*condition)(const T))
{
	ListSequence* newSequence = new ListSequence<ListSequence<T>*>();
	newSequence->list_buffer_ = list_buffer_->Split(condition);
	return newSequence;
}

template <class T>
template <typename newType>
ListSequence<newType>* ListSequence<T>::Map(newType(*transform)(const T))
{
	auto* new_sequence = new ListSequence<newType>();
	for (int i = 0; i < GetLength(); i++)
	{
		new_sequence->Append(transform(Get(i)));
	}

	return new_sequence;
}

template <class T>
ListSequence<T>* ListSequence<T>::Where(bool (*filter)(const T))
{
	auto* new_sequence = new ListSequence<T>();
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
int ListSequence<T>::FindFirst(bool (*condition)(const T))
{
	return list_buffer_->FindFirst(condition);
}

template <class T>
ListSequence<int>* ListSequence<T>::FindAll(bool (*condition)(const T))
{
	ListSequence* found_items = new ListSequence<int>();
	found_items->list_buffer_ = list_buffer_->FindAll(condition);

	return found_items;
}
