#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>

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

    int* p; //number of times each word occurs

public:
    /**Contructor - generate matrices*/
    model(int n, int m)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;

        p = (int*)calloc(N,sizeof(int));
        for(int i=0;i<N;++i)
            p[i] = 1;
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
        // Bigram
        for(int i=input.size()-2; i>=0; i--)
        {
            A[input[i+1]][input[i]] += 1;
            p[input[i]] += 1;
        }
        for(int i=input.size()-2; i>=0; i--)
        {
            A[input[i+1]][input[i]] = A[input[i+1]][input[i]]/p[input[i]];
        }
    }

    /**Generate sequence*/
    std::vector<int> Generate(int stopIndex, int length)
    {
        double val[N];
        for(int i=0;i<N;++i)
            val[i] = 1.;

        std::vector<int> seq(length);
        int index = stopIndex;
        seq[length-1] = index;
        for(int i=length-2;i>=0;--i)
        {


            seq[i] = index;
        }
        return seq;
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
