#include <variant>

template <typename A>
struct Node;

template <typename A>
using BTree = std::variant<A, struct Node<A>>;

template <typename A>
struct Node {
    A x;
    BTree<A> left;
    BTree<A> right;
};

int main (void)
{
	return 0;
}
