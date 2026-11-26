
template <int N>
constexpr int factorial()
{
    static_assert(N >=0 && "Factorial of negative value not well defined");

    int f {1};
    for (int i {1}; i <= N; ++i) {
        f = f*i;
    }

    return f;

}

int main()
{
    static_assert(factorial<0>() == 1);
    static_assert(factorial<3>() == 6);
    static_assert(factorial<5>() == 120);

    // factorial<-3>(); // should fail to compile

    return 0;
}