void foo() {
    int a;
    int x;
    int *b;
    int **c;

    x = 20;
    a = 10;
    // assignments that matter
    b = &a;
    c = &b;
    *c = &x;
}
