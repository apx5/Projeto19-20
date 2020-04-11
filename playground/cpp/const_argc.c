void foo (const int * x)
{
    (void) x;
}

int main (void)
{
    int x = 42;
    foo(&x);
    x = 69;
    return 0;
}
