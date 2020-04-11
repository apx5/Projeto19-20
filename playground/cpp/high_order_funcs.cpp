#include <algorithm>
#include <iostream>
#include <vector>

#define print_vector(v)                                \
    do {                                               \
        std::cout << #v "(" << v.size() << ") = { ";   \
        for (const auto e : v) {                       \
            std::cout << e << ", ";                    \
        }                                              \
        std::cout << '}' << std::endl;                 \
    } while (0)

int dobro (int x)
{
    return (x*2);
}

int main (void)
{
    std::vector<int> numbers(10);
    std::vector<int> bar;
    std::vector<int> odds;
    std::vector<int>::iterator it;

    int i = 0;
    //for (int & number : numbers) {
    for (it = numbers.begin(); it != numbers.end(); it++) {
        *it = i++;
        ///std::cout << *it << std::endl;
    }

    std::copy_if(numbers.begin(),
            numbers.end(),
            std::back_inserter(odds),
            [] (int x) { return x % 2 == 0; }
            ); // qq coisa aqui não está bem

    std::transform(numbers.begin(),
            numbers.end(),
            std::back_inserter(bar),
            dobro
            );

    std::cout << "odds " << odds.size() << std::endl;
    //for (int number : numbers) {
    for (it = odds.begin(); it != odds.end(); it++) {
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;


    std::vector<int> v(10,5);

    std::cout << 'v' << std::endl;
    for_each(v.begin(),
            v.end(),
            [] (int val) { std::cout << val << std::endl; });

    std::cout << std::endl;

    print_vector(numbers);
    print_vector(bar);
    print_vector(odds);
    print_vector(v);

    return 0;
}   
