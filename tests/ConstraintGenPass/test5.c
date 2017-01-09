struct A {
    int x;
    int y[10];
    int *b;
    int z;
};

void foo() {
    int c;
    struct A a;

    a.b = &c;
}
