#include <unordered_map>
#include<iostream>

std::unordered_map<int, int> bigrams;

int main()
{
    int tempBi[2] = {2, 5};
    int t = (int)tempBi;
    std::cout << bigrams[t] << std::endl;
    bigrams[t] = bigrams[t]+1;
    std::cout << bigrams[t] << std::endl;
}
