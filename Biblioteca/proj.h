#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>
#include <iterator>
		


using namespace std;

/*
Função que recebe dois elementos e devolve como resultado a soma dos dois
add:: Num a => a -> a -> a
add a b = a + b
*/

template <typename T> T add(T x, T y){ // T representa um tipo de dados. Desde que esse tipo tenha o operador+ definido, não ocorrem erros.
    return x + y;
}

/*
Função que recebe uma lista de inteiros e devolve a sua soma
addL:: [Int] -> Int
addL [] = 0
addL (h:t) = h + addL t
*/

template<typename T> T addL(vector<T> v){
    return accumulate(v.begin(), v.end(), 0, add<T>);
}

/*
Função que devolve o maior de dois elementos
max:: (Num a, Ord a) => a -> a -> a
max a b | a > b = a
        | otherwise = b
*/

template<typename T> T mymax(T x, T y){
    return x > y? x : y;
}

/*
filter :: (a -> Bool) -> [a] -> [a]
filter f [] = []
filter f (h:t)  | f h = h: filter f t
                | otherwise = filter f t
*/

template<typename T, typename P> vector<T> filter(vector<T> v,P f){
    vector<T> res;
    copy_if(v.begin(),v.end(),back_inserter(res),f);
    return res;
}

/*
map :: (a -> b) -> [a] -> [b]
map f [] = []
map f (h:t) = f h : map f t
*/
/*
template<typename T, typename F,typename R> auto map(vector<T> v, F f) -> vector<R> {
    vector<R> res(v.size());
    transform(v.begin(),v.end(),res.begin(),f);
    return res;

}
*/

/*
foldl :: Foldable t => (b -> a -> b) -> b -> t a -> b

foldl f b [] = b
foldl f b (h:t) = foldl f (f b h) t

*/

template<typename A, typename B, typename F> B foldl(F f, B b, vector<A> v){
    return accumulate(v.begin(), v.end(), b, f);
} 


//reverse = foldl (flip (:)) []
template<typename T> vector<T> my_reverse(vector<T> v){
    return foldl([] (vector<T> vec, T t){ vec.insert(vec.begin(),t); return vec;},vector<T>(),v);
}

// foldr f b = foldl (flip f) b . reverse
template<typename A, typename B, typename F> B foldr(F f, B b, vector<A> v){
    return foldl([f] (A a, B b) { return f(b,a);}, b , my_reverse(v));
} 

