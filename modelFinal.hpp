#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "wordmapFinal.hpp"
#include <sstream>
#include <time.h>

class model
{
private:
    int N;
    std::unordered_map<std::string, double> mapBigrams; //key=the bigram value = number of times it occurred
    std::unordered_map<std::string, double> mapTrigrams; //key=the trigram value = number of times it occurred
    std::unordered_set<std::string> trigrams; //stored trigrams

    WordMap oneMap;
    double* weight; //each word has a weight, the weight is reduced if the word has been used, this
                    // prevents the program from writing the same word all the time.

public:
    /**Contructor */
    model(WordMap inMap)
    {
        N=inMap.maxIndex+1;
        std::cerr << "Number of unique words: " << N << std::endl;
        oneMap = inMap;
        weight = (double*)calloc(N,sizeof(double));
        for(int i=0;i<N;++i)
            weight[i] = 1.0;
    }

    //creates a hashable string from two integers
    std::string toString1(int one, int two)
    {
        std::stringstream ss;
        ss.str(""); // empty the stringstream
        ss.clear();
        ss << one;
        ss << " ";
        ss << two;
        std::string str2 = ss.str();
        return str2;
    }

    //creates a hashable string from three integers
    std::string toString2(int one, int two, int three)
    {
        std::stringstream ss;
        ss.str(""); // empty the stringstream
        ss.clear();
        ss << one;
        ss << " ";
        ss << two;
        ss << " ";
        ss << three;
        std::string str2 = ss.str();
        return str2;
    }

    /**uses the sequence "in" to generate bigrams and trigrams.
    * in = the emission sequence
    */
    void learn(std::vector<int> in)
    {
        for (int i = in.size()-1; i > 1 ; i--)
        {
            std::string temp1 = toString1(in[i],in[i-1]);
            std::string temp2 = toString2(in[i],in[i-1], in[i-2]);

            mapBigrams[temp1]++;
            mapTrigrams[temp2]++;

            trigrams.insert(temp2);
        }
    }

    /**Generate sequence*/
    std::vector<int> Generate(int lastWrd, int length)
    {
        //std::cout << "1Generation: " << lastWrd << std::endl;
        std::vector<int> out;
        std::vector<int> goodWords;

        if (length - oneMap.syllables(oneMap.intToWord[lastWrd]) <= 0)
        {
            out.push_back(lastWrd);
            return out;
        }

        int bestWrd1;
        double maxFreq = 0, tempVal1, tempVal2;
        tempVal2 = count(oneMap.sequence.begin(), oneMap.sequence.end(), lastWrd);
        for(int i = 0; i < mapBigrams.size(); i++)
        {
            std::string temp1 = toString1(lastWrd,i);
            if (mapBigrams.find(temp1) != mapBigrams.end())
            {
                tempVal1 = mapBigrams[temp1]* weight[i]/tempVal2;;
            }
            else
            {
                tempVal1 = weight[lastWrd]*tempVal2/(double)oneMap.sequence.size();
            }

            if(tempVal1 == maxFreq)
            {
                goodWords.push_back(i);
            }

            if (tempVal1 > maxFreq)
            {
                goodWords.clear();
                goodWords.push_back(i);
                maxFreq = tempVal1;
            }
        }

        bestWrd1 = goodWords[rand()%goodWords.size()]; //chooses a word from a list with words, all words in
                                                       //this list has the same probability.
        weight[bestWrd1]*=0.5; //reduces the weight of the word.

        //Generate the rest recursively.
        out = GenerateReq(lastWrd, bestWrd1, length-oneMap.syllables(oneMap.intToWord[lastWrd]));
        out.push_back(lastWrd);

        if(oneMap.debug)
        {
            int same = 0;
            bool c = false;
            //checks if a line exits in the corpus
            for(int i=0;i<oneMap.sequence.size();++i)
            {
                if(oneMap.sequence[i] == out[0])
                {
                    same+=1;
                    for(int j=1;j<out.size();++j)
                    {
                        if(oneMap.sequence[i+j] == out[j])
                        {
                            same++;
                        }
                    }
                    if(same == out.size()-1)
                    {
                        c=true;
                    }
                    else
                    {
                        same = 0;
                    }
                }
            }
            if(c)
                std::cout << "\tit's a Copy ";
            else
                std::cout << "\tnot a Copy  ";

            //counts the number of syllables in the line
            int summa = 0;
            for(int i = 0; i < out.size(); i++)
                summa += oneMap.syllables(oneMap.intToWord[out[i]]);
            std::cout << "(" << summa << ")-";
        }

        return out;
    }

    //generates the line recursively.
    std::vector<int> GenerateReq(int lastWrd, int bestWrd, int length)
    {
        std::vector<int> out;
        std::vector<int> goodWords;

        //Break condition - out of syllables
        if (length - oneMap.syllables(oneMap.intToWord[bestWrd]) <= 0)
        {
            out.push_back(bestWrd);
            return out;
        }

        int bestWrd2 = 0;
        double maxFreq = 0, tempVal1, tempVal2;
        std::string temp1 = toString1(lastWrd, bestWrd);
        tempVal2 = mapBigrams[temp1];
        double w = count(oneMap.sequence.begin(), oneMap.sequence.end(), lastWrd);
        for(int j = 0; j < N; j++)
        {
            std::string temp2 = toString2(lastWrd,bestWrd,j);
            if ((trigrams.find(temp2) != trigrams.end()) && length - oneMap.syllables(oneMap.intToWord[j]))
            {
                tempVal1 = mapTrigrams[temp2]*weight[j]/tempVal2;
            }
            else
            {
                tempVal1 = 0;
                //tempVal1 = weight[bestWrd]*tempVal2/w; -add one smoothing
            }

            if (tempVal1 == maxFreq)
            {
                goodWords.push_back(j);
            }

            if (tempVal1 > maxFreq)
            {
                goodWords.clear();
                goodWords.push_back(j);
                maxFreq = tempVal1;
            }
        }

        bestWrd2 = goodWords[rand()%goodWords.size()];
        weight[bestWrd]*=0.5;
        out = GenerateReq(bestWrd, bestWrd2, length-oneMap.syllables(oneMap.intToWord[lastWrd]));
        out.push_back(bestWrd);
        return out;
    }
};
