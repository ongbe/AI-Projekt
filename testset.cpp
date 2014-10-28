#include <unordered_map>
#include<iostream>

std::unordered_map<int, double> bigrams;

int main()
{
    int a = 2, b = 5;
    int* tempBi[2] = {*a, *b};

    int t = (int)tempBi;
    std::cout << t << std::endl;
    for (int i = 0; i<3; i++)
    {
        int* tempBi[2] = {*i, *5};
        t = (int)tempBi;
        std::cout << t << std::endl;
    }
}
