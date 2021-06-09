#pragma once

#include <iostream>
#include <new>
#include <string>

#include "ICollection.h"
#include "IEnumerable.h"

template <class T>
struct Node
{
	T data;
	Node* prev;
	Node* next;
};

template <class T>
class LinkedListEnumerator;;

template <class T>
class LinkedList :public ICollection<T>, public IEnumerable<T>
{
private:
	Node<T>* head_;
	Node<T>* tail_;
	int length_;
public:
	// constructors
	LinkedList(const T* items, int size);
	LinkedList();
	LinkedList(const LinkedList<T>& list);

	// operators
	LinkedList& operator= (const LinkedList& l_list);
	T& operator[] (const int index);

	// destructor
	~LinkedList();

	// decomposition
	int GetLength() const;
	T GetFirst() const;
	T GetLast() const;
	T Get(int index) const;
	void Set(int index, T value);
	LinkedList<T>* GetSubList(int start_index, int end_index);

	// edit
	void Prepend(T item);
	void Append(T item);
	void InsertAt(T new_item, int index);
	void Remove(int index);

	// operations
	LinkedList<T>* Concat(LinkedList<T>* list);
	int FindFirst(bool (*condition)(const T)); //if no such elements, returns - 1
	LinkedList<int>* FindAll(bool (*condition)(const T));
	LinkedList<LinkedList<T>*>* Split(bool (*condition)(const T));

	struct Iterator
	{
	private:
		Node<T>* elem_ptr;
		LinkedList* list_;
	public:
		Iterator() :elem_ptr(nullptr), list_(nullptr) {}
		Iterator(LinkedList* list) :elem_ptr(list->head_), list_(list) {}
		Iterator(Node<T>* elem, LinkedList* list) : elem_ptr(elem), list_(list) {}

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
			return list_->begin();
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
		return GetLength();
	}

	LinkedListEnumerator<T>* GetEnumerator() override;

};

template <class T>
class LinkedListEnumerator : public IEnumerator<T>
{
public:
	typename LinkedList<T>::Iterator iterator_;

	LinkedListEnumerator(typename LinkedList<T>::Iterator iterator) :iterator_(iterator) {}

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
LinkedListEnumerator<T>* LinkedList<T>::GetEnumerator()
{
	auto* en = new LinkedListEnumerator<T>(begin());
	return en;
}

template <class T>
LinkedList<T>::LinkedList(const T* items, int size)
{
	if (size <= 0)
	{
		throw std::invalid_argument("Invalid size of list: " + std::to_string(size));
	}

	head_ = nullptr;
	tail_ = nullptr;
	length_ = size;

	Node<T>* prev_node = nullptr;
	Node<T>* current_node = nullptr;

	for (int i = 0; i < size; i++)
	{
		current_node = new Node<T>;
		if (i == 0)
		{
			head_ = current_node;
		}
		else
		{
			prev_node->next = current_node;
		}

		current_node->prev = prev_node;
		current_node->data = *(items + i);

		if (i == size - 1)
		{
			current_node->next = nullptr;
			tail_ = current_node;
		}

		prev_node = current_node;
	}
}

template <class T>
LinkedList<T>::LinkedList()
{
	length_ = 0;
	head_ = nullptr;
	tail_ = nullptr;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list)
{
	length_ = list.length_;
	head_ = nullptr;
	tail_ = nullptr;

	Node<T>* prev_node = nullptr;
	Node<T>* current_node = nullptr;

	Node<T>* old_list_ptr = list.head_;

	for (int i = 0; i < list.length_; i++)
	{
		current_node = new Node<T>;
		if (i == 0)
		{
			head_ = current_node;
		}
		else
		{
			prev_node->next = current_node;
		}
		current_node->prev = prev_node;
		current_node->data = old_list_ptr->data;
		if (i == list.length_ - 1)
		{
			current_node->next = nullptr;
			tail_ = current_node;
		}
		prev_node = current_node;
		old_list_ptr = old_list_ptr->next;
	}
}

template <class T>
LinkedList<T>& LinkedList<T>::operator= (const LinkedList& l_list)
{
	// self assignment check
	if (this == &l_list)
	{
		LinkedList& ref = this;
		return ref;
	}

	LinkedList new_l_list = LinkedList(l_list);
	length_ = new_l_list.length_;
	head_ = new_l_list.head_;
	tail_ = new_l_list.tail_;

	LinkedList& ref = this;
	return ref;
}

template <class T>
LinkedList<T>::~LinkedList()
{
	if (length_)
	{
		Node<T>* curr_node = head_;

		for (int i = 0; i < length_; i++)
		{
			Node<T>* next_node = curr_node->next;
			delete curr_node;
			curr_node = next_node;

		}
	}
}

template <class T>
T LinkedList<T>::GetFirst() const
{
	if (length_ == 0)
	{
		throw std::out_of_range("Index was out of range: list is empty");
	}

	return head_->data;
}

template <class T>
T LinkedList<T>::GetLast() const
{
	if (length_ == 0)
	{
		throw std::out_of_range("Index was out of range: list is empty");
	}

	return tail_->data;
}

template <class T>
T LinkedList<T>::Get(int index) const
{
	if (index >= length_)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}

	Node<T>* node_ptr = nullptr;
	if (index <= length_ - 1 - index)
	{
		node_ptr = head_;
		for (int i = 0; i < index; i++)
		{
			node_ptr = node_ptr->next;
		}
	}
	else
	{
		node_ptr = tail_;
		for (int i = 0; i < length_ - 1 - index; i++)
		{
			node_ptr = node_ptr->prev;
		}
	}

	return node_ptr->data;
}

template <class T>
void LinkedList<T>::Set(int index, T value)
{
	if (index >= length_)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}

	Node<T>* node_ptr = nullptr;
	if (index <= length_ - 1 - index)
	{
		node_ptr = head_;
		for (int i = 0; i < index; i++, node_ptr = node_ptr->next) {}
	}
	else
	{
		node_ptr = tail_;
		for (int i = 0; i < length_ - 1 - index; i++, node_ptr = node_ptr->prev) {}
	}

	node_ptr->data = value;
}


template <class T>
LinkedList<T>* LinkedList<T>::GetSubList(int start_index, int end_index)
{
	if (start_index > end_index || start_index < 0 || end_index < 0 ||
		start_index >= length_ || end_index > length_)
	{
		throw std::out_of_range("Index was out of range");
	}

	if (start_index == end_index)
	{
		return new LinkedList();
	}

	LinkedList<T>* sublist = new LinkedList<T>;
	sublist->length_ = end_index - start_index;

	// go to element with start_index
	Node<T>* old_list_ptr = nullptr;
	if (start_index <= length_ - 1 - start_index)
	{
		old_list_ptr = head_;
		for (int i = 0; i < start_index; i++)
		{
			old_list_ptr = old_list_ptr->next;
		}
	}
	else
	{
		old_list_ptr = tail_;
		for (int i = 0; i < length_ - 1 - start_index; i++)
		{
			old_list_ptr = old_list_ptr->prev;
		}
	}

	Node<T>* prev_node = nullptr;
	Node<T>* current_node = nullptr;

	for (int i = 0; i < sublist->length_; i++)
	{
		current_node = new Node<T>;

		if (i == 0)
		{
			sublist->head_ = current_node;
		}
		else
		{
			prev_node->next = current_node;
		}
		current_node->prev = prev_node;
		current_node->data = old_list_ptr->data;
		if (i == sublist->length_ - 1)
		{
			current_node->next = nullptr;
			sublist->tail_ = current_node;
		}
		prev_node = current_node;
		old_list_ptr = old_list_ptr->next;
	}

	return sublist;
}

template <class T>
int LinkedList<T>::GetLength() const
{
	return length_;
}

template <class T>
void LinkedList<T>::Prepend(T item)
{
	Node<T>* new_node = new Node<T>;
	new_node->prev = nullptr;
	new_node->next = head_;
	new_node->data = item;

	if (length_ != 0)
	{
		head_->prev = new_node;
	}
	head_ = new_node;
	if (length_ == 0)
	{
		tail_ = new_node;
	}
	length_++;
}

template <class T>
void LinkedList<T>::Append(T item)
{
	Node<T>* new_node = new Node<T>;
	new_node->next = nullptr;
	new_node->prev = tail_;
	new_node->data = item;

	if (length_ != 0)
	{
		tail_->next = new_node;
	}
	tail_ = new_node;
	if (length_ == 0)
	{
		head_ = new_node;
	}
	length_++;
}

template <class T>
void LinkedList<T>::InsertAt(T new_item, int index)
{
	if (index > length_ || index < 0)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}

	Node<T>* new_node = new Node<T>;
	new_node->data = new_item;

	if (length_ == 0)
	{
		new_node->prev = nullptr;
		new_node->next = nullptr;
		head_ = new_node;
		tail_ = new_node;
	}

	// go to element with start_index
	Node<T>* old_list_ptr = nullptr;
	if (index <= length_ - 1 - index)
	{
		old_list_ptr = head_;
		for (int i = 0; i < index; i++)
		{
			old_list_ptr = old_list_ptr->next;
		}
	}
	else
	{
		old_list_ptr = tail_;
		for (int i = 0; i < length_ - 1 - index; i++)
		{
			old_list_ptr = old_list_ptr->prev;
		}
	}
	new_node->next = old_list_ptr;
	new_node->prev = old_list_ptr->prev;
	new_node->prev->next = new_node;
	new_node->next->prev = new_node;

	length_++;

}

template <class T>
void LinkedList<T>::Remove(int index)
{
	if (index >= length_ || index < 0)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}

	// go to element with start_index
	Node<T>* old_elem_ptr = nullptr;
	if (index <= length_ - 1 - index)
	{
		old_elem_ptr = head_;
		for (int i = 0; i < index; i++)
		{
			old_elem_ptr = old_elem_ptr->next;
		}
	}
	else
	{
		old_elem_ptr = tail_;
		for (int i = 0; i < length_ - 1 - index; i++)
		{
			old_elem_ptr = old_elem_ptr->prev;
		}
	}

	if (old_elem_ptr == head_)
	{
		head_ = old_elem_ptr->next;
		if (old_elem_ptr->next)
		{
			old_elem_ptr->next->prev = nullptr;
		}
	}
	else if (old_elem_ptr == tail_)
	{
		tail_ = old_elem_ptr->prev;
		if (old_elem_ptr->prev)
		{
			old_elem_ptr->prev->next = nullptr;

		}
	}
	else
	{
		old_elem_ptr->prev->next = old_elem_ptr->next;
		old_elem_ptr->next->prev = old_elem_ptr->prev;
	}

	delete old_elem_ptr;

	length_--;

}

template <class T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list)
{
	LinkedList* firstNewList = new LinkedList<T>(*this);
	LinkedList* secondNewList = new LinkedList<T>(*list);
	LinkedList* concatList = new LinkedList<T>();


	if (firstNewList->length_ == 0)
	{
		concatList = secondNewList;
	}
	else if (secondNewList->length_ == 0)
	{
		concatList = firstNewList;
	}
	else if (firstNewList->length_ != 0 && secondNewList->length_ != 0)
	{
		concatList->length_ = firstNewList->length_ + secondNewList->length_;
		concatList->head_ = firstNewList->head_;
		concatList->tail_ = secondNewList->tail_;

		firstNewList->tail_->next = secondNewList->head_;
		secondNewList->head_->prev = firstNewList->tail_;
	}

	return concatList;
}


// finds first element in list with certain properties (if no such elements, returns -1)
template <class T>
int LinkedList<T>::FindFirst(bool (*condition)(const T))
{
	Node<T>* elem_ptr = head_;

	for (int i = 0; i < length_; i++)
	{
		if (condition(elem_ptr->data))
		{
			return i;
		}
		elem_ptr = elem_ptr->next;
	}

	return -1;
}

template <class T>
LinkedList<int>* LinkedList<T>::FindAll(bool (*condition)(const T))
{
	auto* found_items = new LinkedList<int>();
	auto* copy_list = new LinkedList<T>(*this);
	int new_occurrence = 0;
	int removed_elements = 0;
	while (true)
	{
		new_occurrence = copy_list->FindFirst(condition);
		if (new_occurrence == -1)
		{
			break;
		}
		found_items->Append(new_occurrence + removed_elements);
		if (new_occurrence == copy_list->GetLength() - 1)
		{
			break;
		}

		copy_list = copy_list->GetSubList(new_occurrence + 1, copy_list->GetLength());
		removed_elements += new_occurrence + 1;
	}

	return found_items;
}

template <class T>
LinkedList<LinkedList<T>*>* LinkedList<T>::Split(bool (*condition)(const T))
{
	auto* to_split = new LinkedList<T>(*this);
	auto* splitted_list = new LinkedList<LinkedList<T>*>();

	if (length_ == 0)
	{
		return splitted_list;
	}

	auto* split_indexes = to_split->FindAll(condition);

	split_indexes->Prepend(-1);
	split_indexes->Append(to_split->GetLength());
	for (int i = 0; i < split_indexes->GetLength() - 1; i++)
	{
		LinkedList<T>* new_part;
		if (split_indexes->Get(i) == to_split->GetLength() - 1)
		{
			new_part = new LinkedList<T>();
		}
		else
		{
			new_part = to_split->GetSubList(split_indexes->Get(i) + 1, split_indexes->Get(i + 1));
		}
		splitted_list->Append(new_part);
	}

	delete to_split;

	return splitted_list;
}


template <class T>
T& LinkedList<T>::operator[] (const int index)
{
	if (index >= length_)
	{
		throw std::out_of_range("Index [" + std::to_string(index) + "] was out of range");
	}

	Node<T>* node_ptr = nullptr;
	if (index <= length_ - 1 - index)
	{
		node_ptr = head_;
		for (int i = 0; i < index; i++)
		{
			node_ptr = node_ptr->next;
		}
	}
	else
	{
		node_ptr = tail_;
		for (int i = 0; i < length_ - 1 - index; i++)
		{
			node_ptr = node_ptr->prev;
		}
	}

	T& el_ref = node_ptr->data;
	return el_ref;
}

template <class T>
T LinkedList<T>::Iterator::operator*() const
{
	return elem_ptr->data;
}



template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++()
{
	if (elem_ptr)
	{
		elem_ptr = elem_ptr->next;
	}
	else
	{
		throw std::out_of_range("Iterator was out of range");
	}
	return *this;
}

template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator--()
{
	if (elem_ptr)
	{
		elem_ptr = elem_ptr->prev;
	}
	else
	{
		throw std::out_of_range("Iterator was out of range");
	}
	return *this;
}

template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++(int)
{
	Iterator& tmp = *this;
	++(*this);
	return tmp;
}

template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator--(int)
{
	Iterator& tmp = *this;
	--(*this);
	return tmp;
}

template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator+(int n)
{
	Iterator* new_iterator = new Iterator(this->elem_ptr);
	for (int i = 0; i < n; i++)
	{
		++(*new_iterator);
	}

	return *new_iterator;
}

template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator-(int n)
{
	Iterator* new_iterator = new Iterator(this->elem_ptr);
	for (int i = 0; i < n; i++)
	{
		--(*new_iterator);
	}

	return *new_iterator;
}

template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator+=(int n)
{
	for (int i = 0; i < n; i++)
	{
		++(*this);
	}

	return *this;
}

template <class T>
LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator-=(int n)
{
	for (int i = 0; i < n; i++)
	{
		--(*this);
	}

	return *this;
}

template <class T>
LinkedList<T>::Iterator LinkedList<T>::begin()
{
	return Iterator(this);
}

template <class T>
LinkedList<T>::Iterator LinkedList<T>::end()
{
	return Iterator();
}

