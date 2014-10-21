#include <iostream>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>



int main ()
{
    std::cout << "hej" ;
    std::unordered_set<std::string> wordset;
    std::string line;
    std::ifstream myfile ("example.txt");
    if (myfile.is_open())
    {
        std::stringstream ss;
        std::string word;
        while ( getline (myfile,line) )
        {
            std::cout << line << '\n';
            ss << line;
            while (!ss.eof())
            {
                ss >> word;
                wordset.insert(word);
            }
        }
        for (auto i = wordset.begin(); i != wordset.end(); ++i)
        {
            std::cout << (*i) <<std::endl;
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";

    return 0;
}
