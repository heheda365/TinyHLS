
void expr(int a, int b, int c, int d, int& r1, int &r2, int &r3) {
    int x1 = a * b;
    r1 = x1 + b;
    r2 = x1 + c;
    r3 = x1 + d;
}