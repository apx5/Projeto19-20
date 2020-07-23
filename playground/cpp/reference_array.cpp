#include <iostream>
#include <cstring>

using namespace std;

template<typename T,int size> T arraySum(T (&arr)[size]) {
    T ret = {}; //constructor

    for(int i = 0; i < size;i++) ret +=arr[i];
    //std::cout << typeid(T).name() << std::endl;
    return ret;
}

//o array é passado por referencia. size é calculado em 
//tempo de compilação por isso qualquer non-type value
//tem de ser const

int main (void)
{
    int array[]{1,2,3,4};

    int sum = arraySum(array);

    cout << sum << endl;

    return 0;
}
