EXAMPLES := \
    adder             \
    class_templates   \
    const_argc        \
    const_argcpp      \
    high_order_funcs  \
    lazy_eval         \
    pair              \
    ranges            \
    reference_array   \
    static_size_array \
    testes            \

# -pthread for lazy_eval
FLAGS := \
    -I../../include/ \
    -Wall            \
    -Wextra          \
    -pthread         \

CXXFLAGS := $(FLAGS) \
    -std=c++17 \

CFLAGS := $(FLAGS)

default: all

all: $(EXAMPLES)

clean:
	$(RM) $(wildcard *.o) $(EXAMPLES)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean default
