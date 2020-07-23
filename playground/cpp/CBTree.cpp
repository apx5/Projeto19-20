template <typename A>
struct BTree {
    enum {
        BTree_Leaf,
        BTree_Node,
    } variant;

    union {
        A leaf;
        struct {
            A x;
            BTree<A> left;
            BTree<A> right;
        } node;
    } tree;
};

int main (void)
{
	return 0;
}
