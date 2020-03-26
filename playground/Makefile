EXAMPLES := \
    adder             \
    const_argc        \
    const_argcpp      \
    pair              \
    static_size_array \
    testes            \
    high_order_funcs  \


FLAGS := \
    -Wall   \
    -Wextra \

default: all

all: $(EXAMPLES)

clean:
	$(RM) $(wildcard *.o) $(EXAMPLES)

%: %.cpp
	$(CXX) $(FLAGS) $< -o $@

%: %.c
	$(CC) $(FLAGS) $< -o $@

.PHONY: all clean default
