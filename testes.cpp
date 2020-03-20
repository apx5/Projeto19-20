#include <iostream>

using namespace std;

template <int N> struct Factorial {
    static const int result = N * Factorial<N-1>::result;
};
 
template <> struct Factorial<0> {
    static const int result = 1;
};

template <class T>
    T GetMax (T a, T b){
    T result;
    result = a>b?a:b;
    return (result);
}

struct NIL {
    typedef NIL Head;
    typedef NIL Tail;
};
 
template <typename H, typename T=NIL> struct Lst {
    typedef H Head;
    typedef T Tail;
};
 
template <int N> struct Int{ static const int result = N; };


template <typename LST, int N> struct Position{
    static const int result = Position<typename LST::Tail,N-1>::result;
};

template <typename LST> struct Position<LST,0>{
    static const int result=LST::Head::result;
    };


int main() {
    typedef Lst< Int<1>, Lst< Int<2>, Lst< Int<3> > > > lista;
    
    cout << Factorial<5>::result << endl;
    int i = 5, j = 6;
    cout << GetMax<int>(i,j) << endl;
    cout << Position<lista,2>::result << endl;
    return 0;
}
