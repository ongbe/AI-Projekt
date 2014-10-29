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
//#include <hash>

class myHash{
public:
    size_t operator()(const std::vector<int> &k) const{
        int arr[k.size()];
        std::copy(k.begin(), k.end(), arr);
        std::hash<int*> hasher;
        return hasher(arr);
    }
};

struct myEquals : std::binary_function<const std::vector<int>&, const std::vector<int>&, bool> {
    result_type operator()( first_argument_type first, second_argument_type second ) const
    {
        int lhs[first.size()];
        std::copy(first.begin(), first.end(), lhs);
        int rhs[second.size()];
        std::copy(second.begin(), second.end(), rhs);
        return (lhs == rhs);
    }
};

class model
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    //double** A;
    //int* p; //number of times each word occurs
    std::unordered_map<std::vector<int>, double, myHash, myEquals> mapBigrams;
    std::unordered_map<std::vector<int>, double, myHash, myEquals> mapTrigrams;
    std::unordered_set<std::vector<int>, myHash, myEquals> trigrams;

public:
    /**Contructor - generate matrices*/
    model(int n, int m)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;
        /*A = initialize(M,N);
        p = (int*)calloc(N,sizeof(int));
        for(int i=0;i<N;++i)
            p[i] = 1;*/
    }

    /**Creates a nxm matrix*/
    double** initialize(int rows, int cols)
    {
        double** temp;
        temp = (double**)calloc(rows , sizeof(double *));
        for(int i=0 ; i< rows ; ++i)
        temp[i] = (double*)calloc(cols , sizeof(double));
        return temp;
    }

    /**uses the sequence "input" to train the hmm-model
    * input = the emission sequence
    */
    void learn(std::vector<int> input)
    {
        for (int i = input.size()-1; i > 1 ; i--)
        {
            std::vector<int> tempBi = {input[i],input[i-1]}, tempTri = {input[i],input[i-1],input[i-2]};
            mapBigrams[tempBi]++; //If not previously existing (new bigram), automatically creates a node with value 0+1=1. Else, add 1 to value (count).
            mapTrigrams[tempTri]++;
            trigrams.insert(tempTri);
        }
    }

    /**Generate sequence*/
    std::vector<int> Generate(int lastWrd, int length)
    {
        std::cout << "Generation: " << lastWrd << std::endl;
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
            std::vector<int> tempBi = {lastWrd,i};
            if (mapBigrams.find(tempBi) == mapBigrams.end())
            {
                tempVal2 = 1.;
            }
            else
            {
                std::cout << "ok\t" << i << "\t" << mapBigrams[tempBi] << std::endl;
                tempVal2 = (double)(1.+mapBigrams[tempBi]);
            }

            for(int j = 0; j < trigrams.size(); j++)
            {
                //Count occurrences
                std::vector<int> tempTri = {lastWrd, i, j};
                if (trigrams.find(tempTri) == trigrams.end())
                {
                    tempVal1 = 1.;
                }
                else
                {
                    std::cout << "YES\t" << j << "\t" << mapTrigrams[tempTri] << std::endl;
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
