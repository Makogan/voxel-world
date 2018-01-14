//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Header for the definition of a generic chunk object
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

template <typename T> cirArray<T>::cirArray() : cirArray(0){}

template <typename T> cirArray<T>::cirArray(uint size)
{
    array = vector<T>(size);

    start=0;
}

template <typename T> T& cirArray<T>::operator[](int i)
{
    int n = array.size();
    return array[(start + i+n)%n];
}

template <typename T> void cirArray<T>::shift(int i)
{
    int n = array.size();
    start = (start+i+n)%n;
}

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