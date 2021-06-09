#pragma once


template <class T>
concept OrderedType = requires(T a, T b)
{
	{a < b};
	{a == b};
};