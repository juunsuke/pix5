#pragma once


namespace Math
{



inline static void swap(int& v1, int& v2)
{
	// Swap two values
	int tmp = v1;
	v1 = v2;
	v2 = tmp;
}

inline static int min(int v1, int v2)
{
	return v1<v2 ? v1 : v2;
}

inline static int max(int v1, int v2)
{
	return v1>v2 ? v1 : v2;
}





}

