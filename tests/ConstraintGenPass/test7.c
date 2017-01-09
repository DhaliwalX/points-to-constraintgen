void foo() {
    int a = 10;
    int *b = &a;
    int **c = &b;
    int x = 20;
    *c = &x;
}
