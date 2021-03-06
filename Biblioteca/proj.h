#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>
#include <iterator>


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

template<typename T> T addL(std::vector<T> v){
    return std::accumulate(v.begin(), v.end(), 0, add<T>);
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

template<typename T, typename P> std::vector<T> filter(std::vector<T> v,P f){
    std::vector<T> res;
    std::copy_if(v.begin(),v.end(),back_inserter(res),f);
    return res;
}

/*
map :: (a -> b) -> [a] -> [b]
map f [] = []
map f (h:t) = f h : map f t
*/

template<typename T, typename F> decltype(auto) map(std::vector<T> v, F f) {
    std::vector<decltype(f(T()))> res(v.size());
    transform(v.begin(),v.end(),res.begin(),f);
    return res;

}


/*
foldl :: Foldable t => (b -> a -> b) -> b -> t a -> b
foldl f b [] = b
foldl f b (h:t) = foldl f (f b h) t

*/

template<typename A, typename B, typename F> B foldl(F f, B b, std::vector<A> v){
    return std::accumulate(v.begin(), v.end(), b, f);
} 


//reverse = foldl (flip (:)) []
template<typename T> std::vector<T> my_reverse(std::vector<T> v){
    return foldl([] (std::vector<T> vec, T t){ vec.insert(vec.begin(),t); return vec;},std::vector<T>(),v);
}

// foldr f b = foldl (flip f) b . reverse
template<typename A, typename B, typename F> B foldr(F f, B b, std::vector<A> v){
    return foldl([f] (A a, B b) { return f(b,a);}, b , my_reverse(v));
} 


template<typename A, typename B, typename F> decltype(auto) zipWith(F f, std::vector<A> a, std::vector<B> b){
    std::vector<decltype(f(A(),B()))> res;
    for(auto i = a.begin(), j = b.begin(); i != a.end() && j != b.end(); i++, j++){
        res.push_back(f(std::forward<A>(*i), std::forward<B>(*j)));
    }
    return res;  
}

template<typename A, typename B> std::vector<std::pair<A,B>> zip(std::vector<A> a, std::vector<B> b){
    return zipWith<A,B,decltype(std::make_pair<A,B>)>(std::make_pair<A,B>, a, b);
}

template<typename F, typename T> std::vector<T> iterate_n(F f, T a, int N){
    std::vector<T> res;
    res.push_back(a);
    for(int i = 1; i < N; i++){
        a = f(a);
        res.push_back(a);
    }
    return res;
} 

template<typename T> T succ(T a){
    return a + 1;
}