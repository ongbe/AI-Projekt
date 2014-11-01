#include <iostream>
#include "model6.hpp"
#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <ctime>

/** Globals */
std::string input;
WordMap ourMap;

/** Functions */
std::string makeRhyme(bool, std::string);
std::string Rhyme(std::string);


int main()
{
    /* Generate maps */
    std::vector<std::string> v = {"Shakespeare.txt", "Petrarca.txt", "FernandoAntonio.txt", "HelenHayWhitney.txt", "ElizabethBarrettBrowning.txt"};

    WordMap temp = WordMap(v);
    ourMap = temp;

    if(!ourMap.ok)
    {
        std::cerr << "Program interrupted." << std::endl;
        return 1;
    }

    /* Generate model */
    model model(ourMap.maxIndex+1,ourMap.maxIndex+1, ourMap);

    std::cerr << "sequence.size(): "<< ourMap.sequence.size() << "  " << std::endl;

    //train model
    model.learn(ourMap.sequence);
    bool newRhyme = true;
    std::string rhymeA, rhymeB;
    srand (time(NULL)); //<--------------------------------------------Den är ny
    for(int j=0; j<14; ++j) //multiple sentences
    {
        if(j>=12)
        {
            rhymeA = makeRhyme(newRhyme, rhymeA);
            newRhyme = (newRhyme==false);
            std::cout << "  ";
        }
        else
        {
            if(j%1 == 0)
            {
                rhymeA = makeRhyme(newRhyme, rhymeA);
                newRhyme = (newRhyme==false);
            }
            else
            {
                rhymeB = makeRhyme(!newRhyme, rhymeB);
            }
        }

        int in = ourMap.wordToInt[input];
        std::vector<int> ny = model.Generate(in,10);

        std::stringstream ss;
        for(int i=0;i<(int)ny.size();++i)
            ss << ourMap.intToWord[ny[i]] << " ";
        std::cout << ss.str() << std::endl;

        //return 0; /** OBS!!!! TEMPORARY!*/

    }

    //model.print();
	return 0;
}

std::string makeRhyme(bool newRhyme, std::string in)
{
    if (newRhyme)
    {
        /*New rhyme
        input = -1000;
        while(ourMap.wordToInt.find(input) == ourMap.wordToInt.end())
        {
            //New rhyme
            std::cout<< "Word: ";
            std::cin >> input;
        }*/

        int random = rand()%ourMap.maxIndex;   //detta är också nytt
        //std::cout << random << std::endl;
        input = ourMap.intToWord[random];
    }
    else
        input = Rhyme(in);
    return input;
}

std::string Rhyme(std::string word)
{
    int maxI = -1;
    std::vector<std::string>rhymingWords;

    std::string wordPhon = ourMap.wordToPhon[word];
    std::string rhymeWord;
    std::string rhymePhon;
    std::string rhyme = " ";
    std::string phon = " ";
    for(auto it = ourMap.wordToInt.begin(); it != ourMap.wordToInt.end(); ++it)
    {
        //std::cerr << wordPhon << " " << ourMap.phonetics(it->first) << std::endl;
        rhymeWord = it->first;
        //std::cerr << ourMap.wordToPhon[word] << " " << ourMap.wordToPhon[rhymeWord] << std::endl << std::endl;
        rhymePhon = ourMap.wordToPhon[rhymeWord];

        int index = std::min(rhymePhon.length(), wordPhon.length());
        if (index > 5) {index = 5;}

        while (index>0 && word.compare(rhymeWord) != 0)
        {
            if(index > maxI)
            {
                //std::cout << "index > maxI: " << index << std::endl;
                if(rhymePhon.substr(rhymePhon.size()-index,rhymePhon.size()) == wordPhon.substr(wordPhon.size()-index,wordPhon.size()))
                {
                    //rhyme = rhymeWord;
                    rhymingWords.push_back(rhymeWord);
                    phon = rhymePhon;
                    maxI = index;
                    //index = -1; //break while loop
                }
            }
            index--;
        }
    }
    //std::cout << std::endl << "Rhyme: " << word << "  (phonetic: " << wordPhon << ") --> " << rhyme << "  (phonetic: " << phon << ")" << std::endl << std::endl;
    std::cout << "[" << rhymingWords.size() << "] ";
    return rhymingWords[rand()%rhymingWords.size()];
    return rhyme;
}
