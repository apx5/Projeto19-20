#include <iostream>

using namespace std;

template <typename T> void Show(T value){
    cout << value << endl;
}

template<typename T> T Add(T n1, T n2){
   return n1 + n2;
}

template <typename T> double Average(T array[], int N){
    T result = T(); //construtor vazio para result do tipo T
    for(int i = 0; i < N; i++){
        result += array[i];
    }
    return (double) result/N;
}



int main (void)
{
    Show(5);
    int numbers[5] = {1,2,3,4,5};

    //podemos ver aqui a inferência de tipos e que funciona independentemente do tipo
    cout << Add(5,3) << endl;
    cout << Add('a','b') << endl;
    cout << Average(numbers,5) << endl;
    return 0;
}
