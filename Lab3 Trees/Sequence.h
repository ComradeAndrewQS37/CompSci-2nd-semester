#pragma once

template <class T>
class Sequence
{
public:
	// decomposition
	virtual int GetLength() const = 0;
	virtual T GetFirst() const = 0;
	virtual T GetLast() const = 0;
	virtual T Get(int index) const = 0;
	virtual void Set(int index, T item) = 0;

	//operator
	virtual T& operator[] (int index) = 0;

	// edit operations
	virtual void Append(T item) = 0;
	virtual void Prepend(T item) = 0;
	virtual void InsertAt(T item, int index) = 0;
	virtual void Remove(int index) = 0;

	// special operations

	//Sequence<newType>* Map(newType(*transform)(const T))
	T Reduce(T(*func)(const T, const T), T start_value);
	//Sequence<std::tuple<Types...>>* Zip(Sequence<Types>*... sequences)
	//std::tuple<Sequence<T1>*, Sequence<T2>*> Unzip(Sequence<std::tuple<T1, T2>>* zipped)
	//Sequence<Sequence<T>*>* Split(bool (*condition)(const T)) = 0;
	virtual Sequence<T>* Where(bool (*filter)(const T)) = 0;
	virtual Sequence<T>* Concat(Sequence<T>* sequence) = 0;
	virtual Sequence<T>* GetSubsequence(int start_index, int end_index) = 0;


};

template <class T>
T Sequence<T>::Reduce(T(*func)(const T, const T), T start_value)
{
	T result = start_value;
	for (int i = 0; i < GetLength(); i++)
	{
		result = func(Get(i), result);
	}

	return result;

}

