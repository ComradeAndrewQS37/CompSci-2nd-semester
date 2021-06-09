#pragma once

template <class T>
class ICollection
{
public:
	virtual T GetElement(int count) = 0;
	virtual int GetCount() = 0;
};