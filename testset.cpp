#include <unordered_map>
#include<iostream>

std::unordered_map<int*, double> bigrams;
std::unordered_map<int, double> words;

void increment_all(int* start, int* stop)
{
    int * current = start;
    while (current != stop)
    {
        ++(*current);
        ++current;
    }
}

int main()
{
    int a = 2, b = 5;
    int t[2] = {2,5};
    std::cout << t << "\t" << bigrams[t] << std::endl;
    std::cout << ++bigrams[t] << std::endl;

int meh[5] = {1,2,3,4,5};
        int * current = meh;
        words[3]++;
    int lastW = 3;
    std::cout<< "THIS: " << (int)lastW << words[3]++ <<words[lastW] << std::endl;
    for (auto i : {1, 2, 3, 4, 5})
    {
        int temp[2] {*(current++), 5};
        std::cout << current << "\t" << temp << "\t" << ++bigrams[temp] << std::endl;
    }
    std::cout << bigrams[t] << std::endl;
}
