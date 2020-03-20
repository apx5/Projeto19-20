EXAMPLES := \
    adder             \
    const_argc        \
    const_argcpp      \
    pair              \
    static_size_array \
    testes \


FLAGS := \
    -Wall   \
    -Wextra \

default: all

all: $(EXAMPLES)

clean:
	$(RM) $(wildcard *.o) $(EXAMPLES)

%: %.o
	$(CXX) $(FLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: all clean default
