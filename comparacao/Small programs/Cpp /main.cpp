#include <iostream>
#include "prelude.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>
#include <chrono>
#include <tuple>

using namespace std::chrono;

int main(int argc, char const *argv[])
{
    std::vector<int> input_vector;

    std::ifstream file("input_file");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            input_vector.push_back(stoi(line));
        }
        file.close();
    }

    auto add = [] (int x){ return x + x;};
    auto even = [](int x) { return x % 2 == 0; };

    auto start = high_resolution_clock::now();

    //std::vector<int> ret = map(add,input_vector);
    //std::vector<int> ret = filter(even,input_vector);

    //std::vector<int> ret = reverse(input_vector);

    auto ret = zip(input_vector, input_vector);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Took " << duration.count() << " milliseconds" <<  std::endl;

    return 0;
}
