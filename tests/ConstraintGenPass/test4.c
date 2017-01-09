struct A {
    int x;
    int b;
    int y[10];
    int z;
};

void foo() {
    int *c;
    struct A a;

    c = &(a.b);
}
