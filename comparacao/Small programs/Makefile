all: bench_hs bench_cpp

bench_hs: bench.hs
	ghc -O3 bench.hs -o bench_hs

bench_cpp: bench.cpp
	g++ -Wall -Wextra --std=c++17 -O3 bench.cpp -o bench_cpp
