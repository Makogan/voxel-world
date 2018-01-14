//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Implementation of miscellaneous helping functions and structures
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

int const size = 256;
int const mask = size-1;

int perm[ size ];
float vec_field_x[ size ], vec_field_y[ size ];

void vec_field_init()
{
    for ( int index = 0; index < size; ++index ) 
    {
        int other = rand() % ( index + 1 );
        if ( index > other )
            perm[ index ] = perm[ other ];
        perm[ other ] = index;
        vec_field_x[ index ] = cosf( 2.0f * M_PI * index / size );
        vec_field_y[ index ] = sinf( 2.0f * M_PI * index / size );
    }
}

double fade(double d)
{
  d = abs(d);
  if(d>1)
    return 0;
  return 1.f-d*d*d*(d*(d*6-15)+10);
}

double test(double x, double y)
{
  return sqrt(x*x+y*y);
}

double surflet(double x, double y, double grad_x, double grad_y)
{
    return fade(test(x,y)) * ( grad_x * x + grad_y * y );
}

double perlin_noise(double x, double y)
{
    x = abs(x);
    y = abs(y);
    int xi = (int(x));
    int yi = (int(y));
    
    double result = 0;
    for(int grid_y=yi; grid_y <= yi+1; ++grid_y)
    {
        for(int grid_x=xi; grid_x <= xi+1; ++grid_x)
        {
            int hash = perm[(perm[grid_x & mask] + grid_y) & mask];
            result += surflet(x-grid_x, y-grid_y, vec_field_x[hash], vec_field_y[hash]);
        }
    }
    
    return (result);
}

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