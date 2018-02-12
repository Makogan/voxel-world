//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       tools.hpp
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.2
 *
 *	@brief Header for the definition of a generic chunk object
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include "system-libraries.hpp"

using namespace std;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Structure definitions
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/**
 *   A generic circular array class, can be considered a ciircular vector 
 */
template <typename T> class cirArray
{
    private:
        vector<T> array;
        int start;

    public:
        cirArray();
        cirArray(uint size);

        void shift(int);

        T& operator[](int);
        void operator=(T);

        uint size();
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Circular array Class implementation:
*/
//========================================================================================

/**
 * Default constructor of the class 
 */
template <typename T> cirArray<T>::cirArray() : cirArray(0){}

/**
 * Parametrized constructor of the class, creates a circular array of  \a size \a elements
 */
template <typename T> cirArray<T>::cirArray(uint size)
{
    array = vector<T>(size);

    start=0;
}

/**
 * [] operator, returns the element at index \a i \a 
 */
template <typename T> T& cirArray<T>::operator[](int i)
{
    int n = array.size();
    return array[(start + i+n)%n];
}

/**
 * shift the circular array by \a i \a units
 */ 
template <typename T> void cirArray<T>::shift(int i)
{
    int n = array.size();
    start = (start+i+n)%n;
}

/**
 * Returns the current size (number of elements) of the circular array 
 */
template <typename T> uint cirArray<T>::size()
{
    return array.size();
}

//########################################################################################

//========================================================================================
/*
*	Perlin Noise Fucntion declaration:
*/
//========================================================================================

double noise_2D(double x, double y);
void vec_field_init();

//########################################################################################