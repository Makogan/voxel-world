#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <time.h>

using namespace std;

template <typename T> class Circular_array
{
    private:
        vector<T> array;
        int start;

    public:
        Circular_array(uint size);

        void shift(int i);

        T& operator[](int i);
        void operator=(T);
};

template <typename T> Circular_array<T>::Circular_array(uint size)
{
    array = vector<T>(size);

    start=0;
}

template <typename T> T& Circular_array<T>::operator[](int i)
{
    int n = array.size();
    return array[((start + i)%n+n)%n];
}

template <typename T> void Circular_array<T>::shift(int i)
{
    int n = array.size();
    start = ((start+i)%n+n)%n;
}

int main()
{
    Circular_array<int> test(10);

    for(int i=0; i<10; i++)
    {
        test[i] = i;
    }

    for(int i=0; i<10; i++)
    {
        cout << test[i] << " ";
    }
    cout << endl;

    test.shift(1);

    for(int i=0; i<10; i++)
    {
        cout << test[i] << " ";
    }
    cout << endl;

    test.shift(-2);
    
        for(int i=0; i<10; i++)
        {
            cout << test[i] << " ";
        }
        cout << endl;
}