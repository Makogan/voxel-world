#include <string>
#include <iostream>
#include <vector>
//#include <fstream>
//#include <cstdlib>
//#include <unistd.h>
#include <time.h>
#include <glm/glm.hpp>
#include <stdlib.h>     /* srand, rand */

using namespace std;
using namespace glm;

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

inline double fade(double d)
{
    d = abs(d);
    return d >= 1? 0: 1.d - d*d*d*(d*(d*6-15)+10);
}

inline double surflet(double x, double y, double grad_x, double grad_y)
{
    return fade( x ) * fade( y ) * ( grad_x * x + grad_y * y );
}

double perlin_noise(double x, double y)
{
    int xi = (int)x;
    int yi = (int)y;
    
    double result = 0;
    for(int grid_y=yi; grid_y <= yi+1; ++grid_y)
    {
        for(int grid_x=xi; grid_x<xi+1; ++grid_x)
        {
            int hash = perm[(perm[grid_x & mask] + grid_y & mask) & mask];
            result += surflet(x-grid_x, y-grid_y, vec_field_x[hash], vec_field_y[hash]);
        }
    }
    
    return (result);
}

int main()
{
    vec_field_init();
    srand(time(NULL));

    double x = (double) rand()/1000.d;
    double y = (double) rand()/1000.d;

    double p = perlin_noise(x,y);

    cout << p << endl;
}

/*void vec_field_init()
{
    for ( int index = 0; index < size; ++index ) 
    {
        int other = 0; //rand() % ( index + 1 );
        if ( index > other )
            perm[ index ] = perm[ other ];
        perm[ other ] = index;
        vec_field_x[ index ] = cosf( 2.0f * M_PI * index / size );
        vec_field_y[ index ] = sinf( 2.0f * M_PI * index / size );
    }
}*/