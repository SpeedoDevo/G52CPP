#pragma once
#include <random>
#include <ctime>
/*
Grow an array - has to be a basic C-type array, where the values can just be copied.
Example usage:
	if ( (m_iFontCount+1) > m_iArraySize )
		m_pArrayFonts = GrowArray( m_pArrayFonts, m_iArraySize, 10 );
*/
template <class T> 
T* GrowArray( T* pOldArray, int& iSize, int iGrowBy )
{
	T* pNewArray = new T[iSize+iGrowBy];
	if ( pOldArray > 0 )
	{
		for ( int i = 0 ; i < iSize ; i++ )
		{
			pNewArray[i] = pOldArray[i];
		}
		delete[] pOldArray;
	}
	iSize += iGrowBy;
	return pNewArray;
}

// Template function to swap two values over, storing one in a temporary variable of the correct type and using assignment operator
template <typename T>
inline void Swap( T& v1, T& v2 )
{
	T t = v1;
	v1 = v2;
	v2 = t;
}

// Constant for PI
#define MY_PI       3.14159265358979323846

template <typename T>
double GetAngle( T tX1,T tY1,T tX2,T tY2 )
{
	double dAngle = MY_PI / 2; // When X1==X2
	if ( tX1 != tX2 )
		dAngle = atan( (double)(tY2 - tY1) / (double)(tX2 - tX1) );
	else if ( tY2 < tY1 )
		dAngle += MY_PI;

	if ( tX2 < tX1 )
		dAngle += MY_PI;

	return dAngle;
}

template <typename T>
inline T randBetween(T min, T max)
{
	return rand() % (max - min + 1) + min;
}

template <typename T>
inline T normalRandBetween(T min, T max, T deviation = 0)
{
	static std::default_random_engine generator(time(0));


	deviation = deviation == 0 ? (max - min) / 8 : deviation;
	T mean = min + (max - min) /2;

	std::normal_distribution<double> distribution(mean, deviation);

	return (T)distribution(generator);
}

template <typename T>
inline T realRandBetween(T min, T max)
{
	static std::default_random_engine generator(time(0));

	std::uniform_real_distribution<double> distribution(min, max);

	return (T)distribution(generator);
}