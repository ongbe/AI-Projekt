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
    if(ph.length()<2)
        return ph;

    std::string s2 = ph.substr(0,2);
    if( s2 == "kn" || s2 == "gn" || s2 == "pn" || s2 == "ae" || s2 == "wr")
        ph.erase(0,1);
    std::cout <<"2: " << ph <<std::endl;

    //3
    //Drop 'B' if after 'M' at the end of the word
    if(ph[ph.length()-1] == 'b' && ph[ph.length()-2] == 'm')
        ph.erase(ph.length()-1,1);
    std::cout <<"3: " << ph <<std::endl;

    //4
    //'C' transforms to 'X' if followed by 'IA' or 'H'
    //(unless in latter case, it is part of '-SCH-', in which case it transforms to 'K').
    //'C' transforms to 'S' if followed by 'I', 'E', or 'Y'. Otherwise, 'C' transforms to 'K'.
    std::size_t found = input.find_first_of('c');
    while (found!=std::string::npos)
    {
        if(ph[found+1] == 'i')
        {
            if(ph[found+2] == 'a')
                ph[found] = 'x';
            else
                ph[found] = 's';
        }
        else if(ph[found+1]=='h' && ph[found-1] != 's')
            ph[found] = 'x';

        else if(ph[found+1] == 'i' || ph[found+1] == 'e' || ph[found+1] == 'y')
            ph[found] = 's';

        else
            ph[found] = 'k';
        found = input.find_first_of('c',found+1);
    }
    std::cout <<"4: " << ph <<std::endl;



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
