#include <iostream>
#include "HMM.hpp"
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_set>

std::unordered_set<std::string> wordset;
std::vector<std::string> words;
std::vector<int> index;
//std::vector< std::vector<int> > sequences;
int maxIndex = -1;

bool inVector(std::string in);
int indexInVector(std::string in);
std::string getWord(int i);


bool inVector(std::string in)
{
    if(wordset.find(in) != wordset.end()) return true;
    else return false;
}

int indexInVector(std::string in)
{
    std::unordered_set<std::string>::const_iterator i = wordset.find(in);
    if (i != wordset.end()) return 0;
    else return -1;
}

std::string getWord(int i)
{
    for(int j=0;j<(int)index.size();++j)
    {
        if(i == index[j])
            return words[j];
    }
    return "";
}

int main()
{
    std::string line;
    std::ifstream myfile ("poems.txt");
    //std::ifstream myfile("ShakespeareSonnets.txt");

    std::vector<int>sequence;

    if (myfile.is_open())
    {
        /* Skip copyright notices etcetera */
        //for(int i = 0; i < 288; ++i) getline(myfile,line);
        /* Actual used code*/
        while ( getline (myfile,line) )
        {
            //std::cout << line << std::endl;
            //spara ord + Baumwelch + add
            //std::vector<int>sequence;
            std::istringstream iss;
            iss.str(line);
            while (!iss.eof())
            {
                std::string temp;
                iss >> temp;
                //std::cerr << temp << std::endl;
                if(!inVector(temp))
                {
                    maxIndex++;
                    wordset.insert(temp);
                    words.push_back(temp);
                    index.push_back(maxIndex);
                    sequence.push_back(maxIndex);
                }
                else
                {
                    sequence.push_back(indexInVector(temp));
                }
            }
            //sequences.push_back(sequence);
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";
    std::cout << "Reading done" << std::endl;
    HMM model(maxIndex+1,maxIndex+1);
    model.reset();

    std::cerr << "sequence.size(): "<< sequence.size() << "  " << std::endl;

    //for(int i=0;i<(int)temp.size();++i)
    //    std::cerr << temp[i] << " ";      // <------------Här skrivs 0:an ut, Det är för att den första sekvensen har längden 1 och endast
    //std::cerr << std::endl;               //              innehåller en 0:a.

    std::cerr << "Baum-Welch" << std::endl;
    model.BaumWelch(sequence);
    model.add();
    std::cerr << "Baum-Welch + add done" << std::endl;
    std::vector<int> test =  {1,2,3,1,2,3,1,1,2,3,1,2,3,1};
    std::vector<int> ny = model.Viterbi(test);

    for(int i=0;i<(int)ny.size();++i)
        std::cerr << getWord(ny[i]) << " ";
    std::cerr << "\n" << std::endl;

	return 0;
}
