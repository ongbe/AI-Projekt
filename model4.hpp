#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "wordmap.hpp"

class model
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    //double** A;
    //int* p; //number of times each word occurs
    std::unordered_map<int*, double> mapBigrams;
    std::unordered_map<int*, double> mapTrigrams;
    std::unordered_set<int*> trigrams;
    WordMap ourMap;

public:
    /**Contructor - generate matrices*/
    model(int n, int m, WordMap mapIn)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;
        ourMap = mapIn;
    }

    /**uses the sequence "input" to train the hmm-model
    * input = the emission sequence
    */
    void learn(std::vector<int> input)
    {
        for (int i = 0; i > input.size()-1 ; i++)
        {
            std::vector<int> temp1 = {input[i+1],input[i]}, temp2 = {input[i+2],input[i+1],input[i]};
            int tempBi[2] =  {temp1[0], temp1[1]};
            mapBigrams[tempBi]++; //If not previously existing (new bigram), automatically creates a node with value 0+1=1. Else, add 1 to value (count).
            if (i != input.size()-2)
            {
                int* tempTri = temp2.data();
                mapTrigrams[tempTri]++;
                trigrams.insert(tempTri);
            }
        }
    }

    /**Generate sequence*/
    std::vector<int> Generate(int lastWrd, int length)
    {
        //Generate second last word as bigram;
        std::vector<int> out;
        int bestWrd;
        double maxFreq = 0, tempVal1, tempVal2 = count(ourMap.sequence.begin(), ourMap.sequence.end(), lastWrd);
        //std::cout << tempVal2 <<"\t" << lastWrd << std::endl;
        for (int i = 0; i <= N; i++)
        {
            std::vector<int> temp = {lastWrd, i};
            int tempBi[2] =  {temp[0], temp[1]};
            //std::cout << tempBi[0] << "\t" << tempBi[1] << std::endl;
            std::cout << (mapBigrams.find(*{2,1}) != mapBigrams.end()) << std::endl;
            if (mapBigrams.find(tempBi) != mapBigrams.end())
            {
                std::cout << "YES\t" << i << "\t" << mapBigrams[tempBi] << std::endl;
                tempVal1 = (double)(1.+mapBigrams[tempBi]);
            }
            else
                tempVal1 = 0;
            if (tempVal1/tempVal2> maxFreq)
            {
                bestWrd = i;
                maxFreq = tempVal1/tempVal2;
            }
        }
        std::cout << "bigram done" << std::endl;
        out = GenerateReq(bestWrd, lastWrd, length-1);
        out.push_back(lastWrd);
        return out;
    }

    std::vector<int> GenerateReq(int lastWrd, int wrdBeforeThat, int length)
    {
        //std::cout << "Generation: " << lastWrd << std::endl;
        std::vector<int> out;

        //Break condition
        if (length == 1)
        {
            out.push_back(lastWrd);
            return out;
        }

        //Consider
        int bestWrd;
        std::vector<int> temp = {wrdBeforeThat, lastWrd};
        int* tempBi = temp.data();
        double maxFreq = 0, tempVal1, tempVal2 = mapBigrams[tempBi];
        for(int j = 0; j <= trigrams.size(); j++)
        {
            //Count occurrences
            std::vector<int> temp2 = {wrdBeforeThat, lastWrd, j};
            int* tempTri =  temp2.data();
            if (trigrams.find(tempTri) != trigrams.end())
            {
                //std::cout << "YES\t" << j << "\t" << mapTrigrams[tempTri] << std::endl;
                tempVal1 = (double)(1.+mapTrigrams[tempTri]--);
            }
            else
                tempVal1 = 0;
            if (tempVal1/tempVal2> maxFreq)
            {
                bestWrd = j;
                maxFreq = tempVal1/tempVal2;
            }

            //Check if max
            //std::cout << tempVal1 << "\t" << tempVal2 << std::endl;
            //std::cout << tempVal1/tempVal2 << std::endl;

        }

        //Best word found! Generate next word
        out = GenerateReq(bestWrd, lastWrd, length-1);
        out.push_back(lastWrd);
        return out;
    }
};
