struct A {
    int x;
    int y[10];
    int *b;
    int z;
};

struct B {
    int m;
    struct A n;
};

struct B foo() {
    struct B b_struct;
    int c;

    b_struct.n.b = &c;
    return b_struct;
}
