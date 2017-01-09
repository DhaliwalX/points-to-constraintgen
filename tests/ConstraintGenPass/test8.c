struct RT {
  char A;
  int *B[10][20];
  char C;
};
struct ST {
  int X;
  double Y;
  struct RT Z;
};

int *foo() {
  struct ST* s;
  int a = 10;
  s[1].Z.B[5][13] = &a;
  int *c = s[4].Z.B[5][43];
}
