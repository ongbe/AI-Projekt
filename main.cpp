#include <iostream>
#include "HMMv2.hpp"
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <cmath>

//std::unordered_set<std::string> wordset;
std::unordered_map<std::string,int> wordToInt;
std::unordered_map<int, std::string> intToWord;
int maxIndex = -1;

bool inVector(std::string in);
int indexInVector(std::string in);


bool inVector(std::string in)
{
    return wordToInt.find(in) != wordToInt.end();
}

int indexInVector(std::string in)
{
    return wordToInt[in];
}

int main()
{
    std::string line;
    //std::ifstream myfile ("poems.txt");
    std::ifstream myfile("ShakespeareSonnets.txt");

    std::vector<int>sequence;

    if (myfile.is_open())
    {
        /* Skip copyright notices etcetera */
        for(int i = 0; i < 288; ++i) getline(myfile,line);
        /* Actual used code*/
        int nope = 0;
        while ( getline (myfile,line) && nope <50)//&& line != "End of The Project Gutenberg Etext of Shakespeare's Sonnets")
        {
            nope++;
            if (line.length() >= 10)
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
                        wordToInt[temp] = maxIndex;
                        intToWord[maxIndex] = temp;
                        sequence.push_back(maxIndex);
                    }
                    else
                    {
                        sequence.push_back(indexInVector(temp));
                    }
                }
                //sequences.push_back(sequence);
            }
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";
    std::cout << "Reading done" << std::endl;
    HMM model(maxIndex+1,maxIndex+1);
    model.reset();

    std::cerr << "sequence.size(): "<< sequence.size() << "  " << std::endl;
    /*
    std::cerr << "Baum-Welch" << std::endl;
    model.BaumWelch(sequence);
    model.add();
    std::cerr << "Baum-Welch + add done" << std::endl;
    */

    //train hmm
    model.learn(sequence);
    //model.BaumWelch(sequence);
    //model.add();

    int rader = 10;
    for(int j=0; j<rader; j++) //multiple sentence
    {
        std::vector<int> test(rader, 1); //decide size of sentence
        for (int i=0; i<test.size(); i++)
        {
            test[i] = (100*rand())%maxIndex;
        }
        //std::vector<int> ny = model.Viterbi(test);
        std::vector<int> ny = model.Generate(test[j],10);//model.Viterbi(test);
        //std::vector<int> ny = model.Viterbi(ny1);

        for(int i=0;i<(int)ny.size();++i)
            std::cerr << intToWord[ny[i]] << " ";
        std::cerr << "\n" << std::endl;
    }

    //model.print();
	return 0;
}
