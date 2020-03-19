void foo_ref (const int & x)
{
    (void) x;
}

void foo_ptr (const int * x)
{
    (void) x;
}

int main (void)
{
    int x = 42;
    foo_ptr(&x);
    foo_ref(x);
    x = 69;
    return 0;
}
