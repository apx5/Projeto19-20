#include <iostream>
#include "proj.h"

using namespace std;

int main(){

    vector<int> v{1,2,3,4};
    vector<int> v2{1,2,3,4,5,6};

    cout << add(3,2) << endl;
    cout << add(3.4,2.1) << endl;
    cout << mymax(4.2,5.8) << endl;
    cout << mymax(4,5) << endl;
    cout << addL(v) << endl;
    v = filter(v,[=] (int x){ return x > 2;});
    cout << addL(v) << endl;
    auto v3 = map(v,[=](int x){ return x*2;});
    cout << addL(v3) << endl;
    cout << foldl([=] (int x, int y) {return x + y;}, 0, v) << endl;
    cout << foldr([=] (int x, int y) {return x * y;}, 1, v) << endl;
    vector<int>res = my_reverse(v);
    for(int i=0; i < res.size();i++){
        cout << res[i] << ' ';
    }
    cout << endl;

    std::vector<int> c = zipWith(add<int>,v,v2);

    for(int i=0; i < c.size();i++){
        cout << c[i] << ' ';
    }
    cout << endl;

    std::vector<std::pair<int,int>> z = zip(v,v2);

    for(int i=0; i < z.size();i++){
        cout << z[i].first << "," << z[i].second << "; ";
    }
    cout << endl;

    std::vector<int> x = iterate_n(succ<int>,0,5);

    for(int i=0; i < x.size();i++){
        cout << x[i]<< "; ";
    }
    cout << endl;

    return 0;
  
}
