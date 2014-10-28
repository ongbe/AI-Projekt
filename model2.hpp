#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
#include <unordered_map>

struct wordObj
{
    int word;
    int p1;
    int p2;

    wordObj(int index, int plus1, int plus2)
    {
        word = index;
        p1 = plus1;
        p2 = plus2;
    }
};

class model
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    //double** A;
    //int* p; //number of times each word occurs
    std::unordered_map<int64_t, double> bigrams;
    std::unordered_map<int64_t, double> trigrams;

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
            int tempBi[2] = {input[i],input[i-1]}, tempTri[3] = {input[i],input[i-1],input[i-2]};
            int64_t t1 = (int64_t)tempBi, t2 = (int64_t)tempTri;
            bigrams[t1] = (double)bigrams[t1]+1.; //If not previously existing (new bigram), automatically creates a node with value 0+1=1. Else, add 1 to value (count).
            trigrams[t2] = (double)trigrams[t2]+1.;
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
            for(int j = 0; j < N; j++)
            {
                //Count occurrences
                int tempTri[3] = {lastWrd, i, j}, tempBi[2] = {lastWrd,i};
                int64_t t1 = (int64_t)tempBi, t2 = (int64_t)tempTri;
                if (trigrams.find(t2) == trigrams.end()) tempVal1 = 1.;
                else tempVal1 = (trigrams[t2]+1.);
                if (bigrams.find(t1) == bigrams.end()) tempVal2 = 1.;
                else tempVal2 = (bigrams[t1]+1.);

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

    void print (double ** AA)
    {
        std::ofstream myfile("printout.txt");
        if (myfile.is_open())
        {
            for(int i = 0; i < N; i++)
            {
                for(int j = 0; j<N; j++)
                {
                    myfile << AA[i][j] << ", ";
                }
                myfile << "\n";
            }
            myfile.close();
        }
        else std::cout << "Unable to open file";
    }
};
