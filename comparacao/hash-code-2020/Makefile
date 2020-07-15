EXECS := \
	Solve_hs \
	solve_cpp

GHC := ghc

CXXFLAGS := \
	--std=c++17 \
	-I../fplus/include/ \
	-O3 \
	-Wall \
	-Wextra \

HSFLAGS := \
	-O3 \

all: $(EXECS)

%_cpp: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

%_hs: %.hs
	$(GHC) $(HSFLAGS) $< -o $@

clean:
	$(RM) *.hi *.o $(EXECS)

fplus: ../fplus/

../fplus/:
	git clone https://github.com/Dobiasd/FunctionalPlus/ ../fplus/
	cd ../fplus && git checkout d8ae16d4b4523e66a13417896c1e32984a445dc2

.PHONY: all fplus
