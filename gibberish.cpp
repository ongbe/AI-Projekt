#include <cmath>
#include <iostream>

void printer(int a[5])
{
    for (int i = 0; i <= 5; i++)
    {
        std::cout << a[i] << "\t";
    }
    std::cout << std::endl;
    return;
}

int main()
{
    int a[5] = {1, 4, 5, 6, 2}, b[5] = {2, 1, 4, 6, 3};
    printer(a);
    printer(b);
    int c[5] = a*b;
    printer(c);
    return 0;
}
