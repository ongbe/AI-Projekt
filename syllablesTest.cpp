#include <iostream>
#include <cstring>
#include <string>
#include <cstddef>

int syllables(std::string input);
std::string phonetics(std::string input);

int main()
{
    std::string in;
    std::cin >> in;

    std::cout << "The word:  " << in << std::endl;
    std::cout << "Syllables: " << syllables(in) << std::endl;

    std::cout << "Phonetics: " << phonetics(in) << std::endl;


    return 2;
}

std::string phonetics(std::string input)
{
    std::string ph=input;

    //1
    //Drop duplicate adjacent letters, except for C
    int l = ph.length();
    for(int i=0; i<l ;++i)
    {
        if(ph[i] == ph[i+1] && ph[i]!= 'c')
        {
            ph.erase(i,1);
            i--;
            l--;
        }
    }

    std::cout << "1: " << ph << std::endl;

    //2
    //If the word begins with 'KN', 'GN', 'PN', 'AE', 'WR', drop the first letter
    //if(ph.substr(0,1))


    return ph;
}

int syllables(std::string input)
{
    int s = 0;
    std::size_t found = input.find_first_of("aeiou");
    std::size_t newfound;
    while (found!=std::string::npos)
    {
        newfound = input.find_first_of("aeiou",found+1);
        if(newfound != found+1)
            s++;
        found = newfound;
    }
    return s;
}
