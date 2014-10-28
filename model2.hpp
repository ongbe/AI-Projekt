#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
#include <unordered_map>
//#import <utility>

class myHash{
public:
    size_t operator()(const std::vector<int> &k) const{
        if (k.size() == 2)
            return k[0] * 100 + k[1]; //CAUTION: Do NOT use if vector.size()
        if (k.size() == 3)
            return k[0] * 50 + k[1] + k[2] * 13;
    }
};

struct myEquals : std::binary_function<const std::vector<int>&, const std::vector<int>&, bool> {
    result_type operator()( first_argument_type lhs, second_argument_type rhs ) const
    {
        if (lhs.size() == 2 && rhs.size() == 2)
            return (lhs[0] == rhs[0] && lhs[1] == rhs[1]);
        if (lhs.size() == 3 && rhs.size() == 3)
            return (lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2]);
    }
};

class model
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    //double** A;
    //int* p; //number of times each word occurs
    std::unordered_map<std::vector<int>, double, myHash, myEquals> bigrams;
    std::unordered_map<std::vector<int>, double, myHash, myEquals> trigrams;

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
            bigrams[tempBi]++; //If not previously existing (new bigram), automatically creates a node with value 0+1=1. Else, add 1 to value (count).
            trigrams[tempTri]++;
        }
    }

    /**Generate sequence*/
    /*std::vector<int> Generate(int lastWrd, int length)
    {
        std::cout << "Generation" << std::endl;
        std::vector<int> out;
        int bestWrd;
        double maxFreq = 0, tempVal1, tempVal2;
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                //Count occurrences
                int tempTri[3] = {lastWrd, i, j}, tempBi[2] = {i,j};
                if (trigrams.find(tempTri) == trigrams.end()) tempVal1 = 1;
                else tempVal1 = trigrams[tempTri]+1;
                if (bigrams.find(tempBi) == bigrams.end()) tempVal2 = 1;
                else tempVal2 = bigrams[tempBi]+1;

                //Check if max
                if (tempVal1/tempVal2 > maxFreq)
                {
                    bestWrd = i;
                    maxFreq = tempVal1/tempVal2;
                }
            }
        }
        //Best word found! Generate next word
        if (length >= 1)
            out = GenerateReq(bestWrd, length-1);
        out.push_back(lastWrd);
        return out;
    }*/
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
        for(int i = 0; i < N; i++)
        {
            std::vector<int> tempBi = {lastWrd,i};
            if (bigrams.find(tempBi) == bigrams.end())
            {
                tempVal2 = 1.;
            }
            else
            {
                std::cout << "ok\t" << i << "\t" << bigrams[tempBi] << std::endl;
                tempVal2 = (double)(1.+bigrams[tempBi]);
            }

            for(int j = 0; j < N; j++)
            {
                //Count occurrences
                std::vector<int> tempTri = {lastWrd, i, j};
                if (trigrams.find(tempTri) == trigrams.end())
                {
                    tempVal1 = 1.;
                }
                else
                {
                    std::cout << "YES\t" << j << "\t" << trigrams[tempTri] << std::endl;
                    tempVal1 = (double)(1.+trigrams[tempTri]--);
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
