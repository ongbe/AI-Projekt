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

public:
    /**Contructor - generate matrices*/
    model(int n, int m)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;
    }

    /**uses the sequence "input" to train the hmm-model
    * input = the emission sequence
    */
    void learn(std::vector<int> input)
    {
        for (int i = input.size()-1; i > 1 ; i--)
        {
            std::vector<int> temp1 = {input[i],input[i-1]}, temp2 = {input[i],input[i-1],input[i-2]};
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
        int bestWrd;
        double maxFreq = 0, tempVal1, tempVal2;
        for(int i = 0; i < mapBigrams.size(); i++)
        {
            std::vector<int> temp1 = {lastWrd,i};
            int* tempBi =  temp1.data();
            if (mapBigrams.find(tempBi) != mapBigrams.end())
            {
                //std::cout << "ok\t" << i << "\t" << mapBigrams[tempBi] << std::endl;
                tempVal2 = (double)(1.+mapBigrams[tempBi]);
            }

            for(int j = 0; j < trigrams.size(); j++)
            {
                //Count occurrences
                std::vector<int> temp2 = {lastWrd, i, j};
                int* tempTri =  temp2.data();
                if (trigrams.find(tempTri) != trigrams.end())
                {
                    //std::cout << "YES\t" << j << "\t" << mapTrigrams[tempTri] << std::endl;
                    tempVal1 = (double)(1.+mapTrigrams[tempTri]--);
                }

                //Check if max
                //std::cout << tempVal1 << "\t" << tempVal2 << std::endl;
                //std::cout << tempVal1/tempVal2 << std::endl;
                if (tempVal1/tempVal2 > maxFreq)
                {
                    bestWrd = i;
                    maxFreq = tempVal1/tempVal2;
                }
            }
        }
        //Best word found! Generate next word
        out = Generate(bestWrd, length-1);
        out.push_back(lastWrd);
        return out;
    }
};
