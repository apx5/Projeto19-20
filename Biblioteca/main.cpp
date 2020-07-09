#include <iostream>
#include "proj.h"

using namespace std;

int main(){

    vector<int> v{1,2,3,4};

    cout << add(3,2) << endl;
    cout << add(3.4,2.1) << endl;
    cout << mymax(4.2,5.8) << endl;
    cout << mymax(4,5) << endl;
    cout << addL(v) << endl;
    v = filter(v,[=] (int x){ return x > 2;});
    cout << addL(v) << endl;
    //auto v2 = map(v,[=](int x){ return x*2;});
    //cout << addL(v2) << endl;
    cout << foldl([=] (int x, int y) {return x + y;}, 0, v) << endl;
    cout << foldr([=] (int x, int y) {return x * y;}, 1, v) << endl;
    vector<int>res = my_reverse(v);
    for(int i=0; i < res.size();i++){
        cout << res[i] << ' ';
    }
    return 0;
    
}
