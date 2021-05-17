int qwq(int a, int b, int c) {
    return 1;
}
int main(/*int argc, char argv[5]*/) {
    // COMPILE(argv[1]);
    // printf("YES BANGDREAM!");
    int b = 0, c = 0, d = 5, e = 6;
    int* p = &b;
    int a = (b + c) / d % e * 5 + *p + qwq(a, *p, 4);
    return 1;
}