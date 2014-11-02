#include <iostream>
#include "modelFinal.hpp"
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
    bool debug = false;
    bool randomWords = true;

    if(debug)
        std::cout << "Debug mode\n[n], n=number of rhyming words\n(n), n=number of syllables in the line" << std::endl;

    std::string choice;
    std::cout << "This is a sonnet generator\nDo you want to write the rhyme words yourself? ";
    std::getline(std::cin,choice);
    std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
    if(choice == "yes")
        randomWords = false;

    /* Generate maps */
    std::vector<std::string> v = {"Shakespeare.txt", "Petrarca.txt", "FernandoAntonio.txt", "HelenHayWhitney.txt", "ElizabethBarrettBrowning.txt"};

    //WordMap(text, debug, random words)
    WordMap temp = WordMap(v,debug, randomWords);
    ourMap = temp;

    if(!ourMap.ok)
    {
        std::cerr << "Program interrupted." << std::endl;
        return 1;
    }

    /* create model */
    model model(ourMap);

    std::cerr << "Number of word in total: "<< ourMap.sequence.size() << "\n  " << std::endl;
    model.learn(ourMap.sequence);
    bool newRhyme = true;
    std::string rhymeA, rhymeB;
    srand (time(NULL));

    int mod = 2;
    if(ourMap.debug)
        mod = 1; //changed the order of lines, to make it easier to see if the lines rhyme

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
            if(j%mod == 0)
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
    }
	return 0;
}

std::string makeRhyme(bool newRhyme, std::string in)
{
    if (newRhyme)
    {
        //New rhyme
        if(!ourMap.rWords) //user input
        {
            input = -1000;
            while(ourMap.wordToInt.find(input) == ourMap.wordToInt.end())
            {
                //New rhyme
                std::cout<< "Word: ";
                std::getline(std::cin,input);
            }
        }
        else //random words
        {
            int random = rand()%ourMap.maxIndex;
            input = ourMap.intToWord[random];
        }
    }
    else
        input = Rhyme(in);
    return input;
}

/**returns a rhyming word to "word"*/
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
        rhymeWord = it->first;
        rhymePhon = ourMap.wordToPhon[rhymeWord];

        int index = std::min(rhymePhon.length(), wordPhon.length());
        if (index > 5) {index = 5;}

        while (index>0 && word.compare(rhymeWord) != 0)
        {
            if(index >= maxI && (rhymePhon.substr(rhymePhon.size()-index,rhymePhon.size()) == wordPhon.substr(wordPhon.size()-index,wordPhon.size())))
            {
                if (index > maxI)
                    rhymingWords.clear();
                rhymingWords.push_back(rhymeWord);
                phon = rhymePhon;
                maxI = index;
                index = -1; //break while loop
            }
            index--;
        }
    }
    if(ourMap.debug)
        std::cout << "[" << rhymingWords.size() << "] ";
    return rhymingWords[rand()%rhymingWords.size()];
}
