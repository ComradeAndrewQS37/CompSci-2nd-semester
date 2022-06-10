#pragma once

template <class T>
concept Complex = requires(T a)
{
	{a.real()};
	{a.imag()};
};


template <class T>
concept IntMultipliable = requires(T a, int b)
{
	{a * b};
};

template <class T>
concept DoubleNumbersMultipliable = requires(T a, double b)
{
	{a * b};
};
