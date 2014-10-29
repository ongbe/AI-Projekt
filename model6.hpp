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
    WordMap oneMap;

public:
    /**Contructor - generate matrices*/
    model(int n, int m, WordMap inMap)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;
        oneMap = inMap;
    }

    /**uses the sequence "input" to train the hmm-model
    * input = the emission sequence
    */
    void learn(std::vector<int> in)
    {
        for (int i = in.size()-1; i > 1 ; i--)
        {
            std::vector<int> temp1 = {in[i],in[i-1]}, temp2 = {in[i],in[i-1],in[i-2]};
            int* tempBi =  temp1.data();

            mapBigrams[tempBi]++; //If not previously existing (new bigram), automatically creates a node with value 0+1=1. Else, add 1 to value (count).
            int* tempTri = temp2.data();
            mapTrigrams[tempTri]++;
            trigrams.insert(tempTri);
        }
    }

    /**Generate sequence*/
    std::vector<int> Generate(int lastWrd, int length)
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
        int bestWrd1;
        double maxFreq = 0, tempVal1, tempVal2;
        tempVal2 = count(oneMap.sequence.begin(), oneMap.sequence.end(), lastWrd);
        for(int i = 0; i < mapBigrams.size(); i++)
        {
            std::vector<int> temp1 = {lastWrd,i};
            int* tempBi =  temp1.data();
            if (mapBigrams.find(tempBi) != mapBigrams.end())
            {
                std::cout << "ok\t" << i << "\t" << mapBigrams[tempBi] << std::endl;
                tempVal1 = (double)(1.+mapBigrams[tempBi]);
            }
            if (tempVal1/tempVal2 > maxFreq)
            {
                bestWrd1 = i;
                maxFreq = tempVal1/tempVal2;
            }
        }

        //Initializer condition
        if(length == 10)
        {
            out = Generate(bestWrd1, length-1);
            out.push_back(lastWrd);
            return out;
        }

    /*std::vector<int> Generate(int bestWrd; int lastWrd, int length)
    {
        //std::cout << "Generation: " << lastWrd << std::endl;
        std::vector<int> out;*/

        //Consider
        int bestWrd2;
        maxFreq = 0;
        std::vector<int> temp1 = {lastWrd, bestWrd1};
        int* tempBi =  temp1.data();
        for(int j = 0; j < N; j++)
        {
            int bestWrd;
            double maxFreq = 0, tempVal1, tempVal2;
            //Count occurrences
            std::vector<int> temp2 = {lastWrd, bestWrd1, j};
            int* tempTri =  temp2.data();
            if (trigrams.find(tempTri) != trigrams.end())
            {
                //std::cout << "YES\t" << j << "\t" << mapTrigrams[tempTri] << std::endl;
                tempVal1 = (double)(1.+mapTrigrams[tempTri]);
            }

            //Check if max
            //std::cout << tempVal1 << "\t" << tempVal2 << std::endl;
            //std::cout << tempVal1/tempVal2 << std::endl;

            if (tempVal1/tempVal2 > maxFreq)
            {
                bestWrd2 = j;
                maxFreq = tempVal1/tempVal2;
            }
        }
        //Best word found! Generate next word
        out = Generate(bestWrd2, length-1);
        out.push_back(lastWrd);
        return out;
    }
};
