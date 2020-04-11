EXAMPLES := \
    adder             \
    const_argc        \
    const_argcpp      \
    high_order_funcs  \
    lazy_eval         \
    pair              \
    static_size_array \
    testes            \

# -pthread for lazy_eval
FLAGS := \
    -Wall      \
    -Wextra    \
    -std=c++17 \
    -pthread   \

default: all

all: $(EXAMPLES)

clean:
	$(RM) $(wildcard *.o) $(EXAMPLES)

%: %.cpp
	$(CXX) $(FLAGS) $< -o $@

%: %.c
	$(CC) $(FLAGS) $< -o $@

.PHONY: all clean default
