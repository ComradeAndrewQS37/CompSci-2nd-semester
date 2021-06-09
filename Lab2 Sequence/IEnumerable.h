#pragma once

template<class T>
class IEnumerator
{
public:
	virtual void MoveNext() = 0;
	virtual void Reset() = 0;
	virtual T GetCurrent() = 0;
};

template<class T>
class IEnumerable
{
public:
	virtual IEnumerator<T>* GetEnumerator() = 0;
};