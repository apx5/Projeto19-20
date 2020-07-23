#include <variant>

template <typename A>
struct Node {
    A x;
    Node<A> left;
    Node<A> right;
};

template <typename A>
using BTree = std::variant<A, struct Node<A>>;

int main (void)
{
	return 0;
}
