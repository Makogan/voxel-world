//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
*   @file       tools.cpp
*	@author 	Camilo Talero
*
*
*	Version: 0.0.2
*
*	@brief Implementation of miscellaneous helping functions and structures
*
*   Perlin noise implementation was done following the information at:
*   http://eastfarthing.com/blog/2015-04-21-noise/
*
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "tools.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Perlin Noise Functions:
*/
//========================================================================================
//TODO: document

int const size = 256; //Size of the perlin noise grid
int const mask = size-1; //hashing mask

int perm[ size ];
float vec_field_x[ size ], vec_field_y[ size ];

/**
* Initialize the perlin noise grid. We basically rotate a 2D vector 2PI units in the 
* counter clockwise direction and assign a random location to it in a lookup table
*/
void vec_field_init()
{
    for ( int index = 0; index < size; ++index ) 
    {
        //scramble the index tbale
        int other = rand() % ( index + 1 );
        if ( index > other )
            perm[ index ] = perm[ other ];
        perm[ other ] = index;
        //create a random 2D vector
        vec_field_x[ index ] = cosf( 2.0f * M_PI * index / size );
        vec_field_y[ index ] = sinf( 2.0f * M_PI * index / size );
    }
}

/**
* Function to smooth out the transition from each grid cell to another
* f(x)=1-6*|x|^5-15|x|^4+10|x|^3
*/
double inline fade(double d)
{
  d = abs(d);
  if(d>1)
    return 0;
  return 1.f-d*d*d*(d*(d*6-15)+10);//1-6*|x|^5-15|x|^4+10|x|^3
}

/**
*  Return the length of the vector (x,y) for radial fading.
*/
double inline length(double x, double y)
{
  return sqrt(x*x+y*y);
}

/**
* 2D convolution surflet function, returns a scalar based on the gradient at (x,y)
*/
double inline surflet(double x, double y, double grad_x, double grad_y)
{
    return fade(length(x,y)) * ( grad_x * x + grad_y * y );
}

/**
* 2D Perlin Noise funtion
*/
double perlin_noise(double x, double y)
{
    //We work only with positive values
    x = abs(x);
    y = abs(y);
    //Get the (x_i,y_i) square in the global 2D  grid
    int xi = (int(x));
    int yi = (int(y));
    
    double result = 0;
    //Iterate through the neighbours (neighbour radius of 1)
    for(int grid_y=yi; grid_y <= yi+1; grid_y++)
    {
        for(int grid_x=xi; grid_x <= xi+1; grid_x++)
        {
            //find the entry in the hash table for our gradient vetor.
            int hash = perm[(perm[grid_x & mask] + grid_y) & mask];
            //get the surflet value of the local point in the grid square 
            //and the gradient at the current point
            //and add the influence of each gradient vector to the current point
            result += surflet(x-grid_x, y-grid_y, vec_field_x[hash], vec_field_y[hash]);
        }
    }
    
    return (result);
}

/**
* Composite 2D noise function. Combines multiple iterations of Perlin noise at different
* sampling rates and amplitudes and merges them using octaves to create more complex 
* noise functions
*/
double noise_2D(double x, double y)
{
    double total=0;
    double frequency=0.05;
    double amplitude=10.d;
    double maxValue=0.d;
    double persistence = 0.5;

    for(uint i=0; i<5; i++)
    {
        total += perlin_noise(x*frequency, y*frequency)*amplitude;

        maxValue+=amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    //cout << total << endl;
    double p=total/maxValue;
    /*if(abs(p)<0.01)
    p+=perlin_noise(x*1,y*1);
    if(p>=0)
    p=1;
    else
    p=-1;*/
    //cout <<  p << endl;
    return p;
}
//########################################################################################