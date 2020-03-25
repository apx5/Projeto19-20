#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <algorithm>

using namespace std;
vector<int> numbers(10);
vector<int> bar(numbers.size(),0);
vector<int> odds(numbers.size());

int dobro(int x){
    return (x*2);
}



int main(int argc, char const *argv[])
{
    
    vector<int>::iterator it;
    int i = 0;
    for(it = numbers.begin(); it != numbers.end();it++){
        *it = i++;
        ///cout << *it << endl;
    }

    copy_if(numbers.begin(),numbers.end(),std::back_inserter(odds), [] (const int x) { return x % 2 == 0;}); // qq coisa aqui não está bem
    
    std:transform(numbers.begin(),numbers.end(),bar.begin(),dobro);


    cout << odds.size() << endl;
    for(it = odds.begin(); it != odds.end();it++){
        cout << *it << endl;
    }

    
    vector<int> v(10,5);

    for_each( v.begin(), v.end(), [] (int val) { 
        std::cout << val;
    } );
    
}   
