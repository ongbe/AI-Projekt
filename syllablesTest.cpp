
#include <iostream>
#include <cstring>
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
    int l = ph.length();

    //7
    //'G' transforms to 'J' if before 'I', 'E', or 'Y', and it is not in 'GG'.
    //Otherwise, 'G' transforms to 'K'.
    for(int i=0; i<l ;++i)
    {
        if(ph[i] == 'g' && ph[i+1]=='g')
        {
            ph[i]='k';
            ph.erase(i+1,1);
            i--;
            l--;
        }
        else if(ph[i] == 'g' && i+1<l && i-1>0)
        {
            if (ph[i+1] == 'i' || ph[i+1] == 'e' || ph[i+1] == 'y' && ph[i-1] != 'g')
            {
                ph[i] = 'j';
            }
            else
            {
                ph[i] = 'k';
            }
        }
    }
    std::cout << "7: " << ph << std::endl;

    //1
    //Drop duplicate adjacent letters, except for C
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
    //if(ph.length()<2)
        //return ph;

    std::string s2 = ph.substr(0,2);
    if( s2 == "kn" || s2 == "gn" || s2 == "pn" || s2 == "ae" || s2 == "wr")
    {
        ph.erase(0,1);
    }
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
    l = ph.length();

    //5
    //'D' transforms to 'J' if followed by 'GE', 'GY', or 'GI'.
    //Otherwise, 'D' transforms to 'T'
    for(int i=0; i<l ;++i)
    {
        if(ph[i] == 'd')
        {
            if(ph[i+1]=='g' && ph[i+2]=='e' || ph[i+1]=='g' && ph[i+2]=='y' || ph[i+1]=='g' && ph[i+2]=='i')
            {
                ph[i]='j';
            }
            else
            {
                ph[i]='t';
            }
        }
    }
    std::cout << "5: " << ph << std::endl;


    //6 - kanske FÄRDIG
    //Drop 'G' if followed by 'H' and 'H' is not at the end or before a vowel
    //Drop 'G' if followed by 'N' or 'NED' and is at the end. (NED eller N i slutet)
    for(int i=0; i<l ;++i)
    {
        if(ph[i] == 'g' && ph[i+1] == 'h')
        {
            if ((i+1) <= (l-1) && ph[i+1] != 'a' && ph[i+1] != 'e' && ph[i+1] != 'i' && ph[i+1] != 'o' && ph[i+1] != 'u')
            {
                ph.erase(i,1);
                i--;
                l--;
            }
        }
        if(ph[i] == 'g' && ph[i+1] == 'n')
        {
            if((i+1)==(l-1) || (ph[i+2] == 'e' && ph[i+2] == 'd'))
            {
                ph.erase(i,1);
                i--;
                l--;
            }
        }
    }
    std::cout << "6: " << ph << std::endl;


    //8
    //Drop 'H' if after vowel and not before a vowel
    for(int i=0; i<l ;++i)
    {
        if(ph[i] == 'h' )
        {
            if (i-1>0)
            {
                if(ph[i-1]=='a'||ph[i-1]=='e'||ph[i-1]=='i'||ph[i-1]=='o'||ph[i-1]=='u')//if after vowel
                {
                    if(i+1<l)
                    {
                        if(ph[i+1]!='a'&&ph[i+1]!='e'&&ph[i+1]!='i'&&ph[i+1]!='o'&&ph[i+1]!='u') // if not before vowel
                        {
                            ph.erase(i,1);
                            i--;
                            l--;
                        }
                    }
                    else
                    {
                        ph.erase(i,1);
                        i--;
                        l--;
                    }
                }
            }
        }
    }
    std::cout << "8: " << ph << std::endl;

    //9
    //'CK' transforms to 'K'
    for(int i=0; i<l-1 ;++i)
    {
        if(ph[i] == 'c' && ph[i+1] == 'k')
        {
            ph.erase(i,1);
            i--;
            l--;
        }
    }
    std::cout << "9: " << ph << std::endl;

    //10
    //'PH' transforms to 'F'
    for(int i=0; i<l-1 ;++i)
    {
        if(ph[i] == 'p' && ph[i+1] == 'h')
        {
            ph[i] = 'f';
            ph.erase(i+1,1);
            i--;
            l--;
        }
    }
    std::cout << "10: " << ph << std::endl;

    //11
    //'Q' transforms to 'K'
    for(int i=0; i<l ;++i)
    {
        if(ph[i] == 'q')
        {
            ph[i] = 'k';
        }
    }
    std::cout << "11: " << ph << std::endl;

    //12
    //'S' transforms to 'X' if followed by 'H', 'IO', or 'IA'
    bool X = false;
    for(int i=0; i<l-1 ;++i)
    {
        if(ph[i]=='s' && ph[i+1]=='h' || i+2<l && ph[i]=='s' && ph[i+1]=='i' && ph[i+2]=='o' || i+2<l && ph[i]=='s' && ph[i+1]=='i' && ph[i+2]=='a')
        {
            ph[i] = 'x';
            X = true;
        }
    }
    std::cout << "12: " << ph << std::endl;

    //13
    //'T' transforms to 'X' if followed by 'IA' or 'IO'.
    //'TH' transforms to '0'. Drop 'T' if followed by 'CH'.
    for(int i=0; i<l ;++i)
    {
        if(i+2<l)
        {
            if(ph[i]=='t' && ph[i+1]=='i' && ph[i+2]=='o' || ph[i]=='t' && ph[i+1]=='i' && ph[i+2]=='a')
            {
                ph[i] = 'x';
            }
        }
        if(i+1<l)
        {
            if(ph[i]=='t' && ph[i+1]=='h')
            {
                ph[i] = '0';
                ph.erase(i+1,1);
                i--;
                l--;
            }
        }
        if(i+2<l)
        {
            if(ph[i]=='t' && ph[i+1]=='c' && ph[i+2]=='h')
            {
                ph.erase(i,1);
                i--;
                l--;
            }
        }
    }
    std::cout << "13: " << ph << std::endl;

    //14
    //'V' transforms to 'F'
    for(int i=0; i<l ;++i)
    {
        if(ph[i]=='v')
        {
            ph[i] = 'f';
        }
    }
    std::cout << "14: " << ph << std::endl;

    //15
    //'WH' transforms to 'W' if at the beginning
    //Drop 'W' if not followed by a vowel
    for(int i=0; i<l-1 ;++i)
    {
        if(ph[i]=='w')
        {
            if(ph[i+1]=='h')
            {
                ph.erase(i+1,1);
                i--;
                i--;
            }
        }
    }
    for(int i=0; i<l-1 ;++i)
    {
        if(ph[i]=='w')
        {
            if(ph[i+1]!='a'&&ph[i+1]!='e'&&ph[i+1]!='i'&&ph[i+1]!='o'&&ph[i+1]!='u')
            {
                ph.erase(i,1);
                i--;
                l--;
            }
        }
    }


    std::cout << "15: " << ph << std::endl;

    //16
    //'X' transforms to 'S' if at the beginning
    //Otherwise, 'X' transforms to 'KS'.
    if(ph[0]=='x' && X==false)
    {
        ph[0] = 's';
    }
    for(int i=0; i<l ;++i)
    {
        if(i != 0 && ph[i]=='x')
        {
            ph[i]='k';
            ph.insert(i+1,"s");
            i++;
            l++;
        }
    }
    std::cout << "16: " << ph << std::endl;

    //17
    //Drop 'Y' if not followed by a vowel
    for(int i=0; i<l-1 ;++i)
    {
        if(ph[i]=='y')
        {
            if(ph[i+1]!='a'&&ph[i+1]!='e'&&ph[i+1]!='i'&&ph[i+1]!='o'&&ph[i+1]!='u')
            {
                ph.erase(i,1);
                i--;
                l--;
            }
        }
    }
    std::cout << "17: " << ph << std::endl;

    //18
    //'Z' transforms to 'S'
    for(int i=0; i<l ;++i)
    {
        if(ph[i]=='z')
        {
            ph[i] = 's';
        }
    }
    std::cout << "18: " << ph << std::endl;

    //19 - VILL EJ HA
    //Drop all vowels unless it is the beginning


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
