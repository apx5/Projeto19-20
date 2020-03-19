/*
 * This probably isn't too useful
 */
template <int n>
int adder (int x)
{
    return n + x;
}

int main (void)
{
    return adder<2>(40);
}
