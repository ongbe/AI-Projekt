#include <iostream>
#include "model.hpp"
#include "wordmap.hpp"
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>

/** Globals */
std::string input;
WordMap ourMap;

/** Functions */
std::string makeRhyme(bool, std::string);

int main()
{
    /* Generate maps */
    std::vector<std::string> v = {"ShakespeareSonnets.txt", "Petrarca.txt"};

    WordMap temp = WordMap(v);
    ourMap = temp;

    if(!ourMap.ok)
    {
        std::cerr << "Program interrupted." << std::endl;
        return 1;
    }

    /* Generate model */
    model model(ourMap.maxIndex+1,ourMap.maxIndex+1);

    std::cerr << "sequence.size(): "<< ourMap.sequence.size() << "  " << std::endl;
    /*
    std::cerr << "Baum-Welch" << std::endl;
    model.BaumWelch(ourMap.sequence);
    model.add();
    std::cerr << "Baum-Welch + add done" << std::endl;
    */

    //train hmm
    model.learn(ourMap.sequence);
    bool newRhyme = true;
    std::string rhymeA, rhymeB;
    for(int j=0; j<14; ++j) //multiple sentences
    {
        if(j>=12)
        {
            rhymeA = makeRhyme(newRhyme, rhymeA);
            newRhyme = (newRhyme==false);
        }
        else
        {
            if(j%2 == 0)
            {
                rhymeA = makeRhyme(newRhyme, rhymeA);
                newRhyme = (newRhyme==false);
            }
            else
            {
                rhymeB = makeRhyme(!newRhyme, rhymeB);
            }
        }

//        std::vector<int> test(rader, 1); //decide size of sentence
//        for (int i=0; i<test.size(); i++)
//        {
//            test[i] = (100*rand())%ourMap.maxIndex;
//        }
//        std::vector<int> ny = model.Generate(test[j],10);

        int in = ourMap.wordToInt[input];

        std::vector<int> ny = model.Generate(in,10);

        for(int i=0;i<(int)ny.size();++i)
            std::cerr << ourMap.intToWord[ny[i]] << " ";
        std::cerr << std::endl;
        //in = model.rhyme(wordToInt(ny[(int)ny.size()-1]));

    }

    //model.print();
	return 0;
}

std::string makeRhyme(bool newRhyme, std::string in)
{
    if (newRhyme)
    {
        //New rhyme
        std::cout<< "Word: ";
        std::cin >> input;
        return input;
    }
    else
        input = in;
        return in;
}
