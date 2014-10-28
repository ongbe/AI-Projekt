#include <iostream>
#include "modelv2.hpp"
#include "wordmap2.hpp"
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
std::string Rhyme(std::string word);

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
//        input = in;
//        return in;
        std::string input = Rhyme(in);
        return input;
}

std::string Rhyme(std::string word)
{
    std::string phon = ourMap.phonetics(word);
    //std::cerr << "Word to rhyme on: " << phon << std::endl;

    //Iterate through all the words
    int maxMatch = 0;
    std::string rhyme = "no rhyme :(";
    std::string phonRhyme;
    for(auto it = ourMap.wordToInt.begin(); it != ourMap.wordToInt.end(); ++it)
    {
        //std::cout << " " << it->first << std::endl;
        std::string currentWord = it->first;
        std::string phon2 = ourMap.phonetics(currentWord);

        bool b = true;

        int I;
        if(phon2.size()-1>=5 && phon.size()-1>=5)
        {
            I = 5;
        }
        else
        {
            I = std::min(phon2.size()-1,phon.size()-1);
        }

        while(b && I>0)
        {
            if (phon2.size()>=I && phon.size()>=I && b)
            {
                if(phon.compare(phon2) != 0 && phon2.substr(phon2.size()-I,phon2.size()-1) == phon.substr(phon.size()-I,phon.size()-1))
                {
                    //std::cout << word << " " << word.size() << " " << word2 << " " << word2.size() << std::endl;
                    rhyme = currentWord;
                    phonRhyme = phon2;
                    b = false;
                }
            }
            I--;
        }

    }

    std::cout << "Rhyme: " << word << "  (phonetic: " << phon << ") --> " << rhyme << "  (phonetic: " << phonRhyme << ")" <<  std::endl;

    return " ";
}
