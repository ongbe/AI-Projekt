#include <iostream>
#include "HMMv2.hpp"
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>

//std::unordered_set<std::string> wordset;
std::unordered_map<std::string,int> wordToInt;
std::unordered_map<int, std::string> intToWord;
int maxIndex = -1;

bool inVector(std::string);
int indexInVector(std::string);
bool readFile(std::string);
std::string line;
std::vector<int>sequence;

int main()
{
    bool one = readFile("ShakespeareSonnets.txt"), two = readFile("Petrarca.txt");

    if(!one && !two)
    {
        std::cerr << "Program interrupted." << std::endl;
        return 1;
    }
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

bool inVector(std::string in)
{
    return wordToInt.find(in) != wordToInt.end();
}

int indexInVector(std::string in)
{
    return wordToInt[in];
}

bool readFile(std::string in)
{
    /* Set variables */
    std::string line, endLine;
    std::ifstream myfile (in);
    int start;
    if (in == "Petrarca.txt")
    {
        start = 6638;
        endLine = "INDEX.";
    }
    else
    {
        start = 288;
        endLine = "End of The Project Gutenberg Etext of Shakespeare's Sonnets";
    }

    /* import file */
    if (myfile.is_open())
    {
        for (int i = 0; i<6638; ++i)
            getline(myfile, line);

        std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
        char unallowed[] = "()-,.!?:;";
        while ( getline (myfile,line)  && line != endLine)
        {
            /* Skip non-poem lines */
            if (line.find_last_of (lowercase) != std::string::npos && line.find("_"))
            {
                /* Trim special characters */
                for (unsigned int i = 0; i < strlen(unallowed); ++i)
                {
                  // you need include <algorithm> to use general algorithms like std::remove()
                  line.erase (std::remove(line.begin(), line.end(), unallowed[i]), line.end());
                }
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
        std::cout << "Reading " << in << " done" << std::endl;
        return true;
    }

    else std::cout << "Unable to open "<< in << std::endl;
    return false;

}
