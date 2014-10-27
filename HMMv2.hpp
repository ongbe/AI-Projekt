#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
class HMM
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    double** A;

public:
    /**Contructor - generate matrices*/
    HMM(int n, int m)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;
        A = initialize(N,N);
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
        std::vector<double> p(N,1.);
        for(int i=0;i<N;++i)
        for(int j=0;j<N;++j)
        A[i][j] = 0;

        // Bigram
        for(int i=0; i<input.size()-1; i++)
        {
            A[input[i]][input[i+1]] += 1;
            p[input[i]] += 1;
        }
        for(int i=0; i<input.size()-1; i++)
        {
            A[input[i]][input[i+1]] = A[input[i]][input[i+1]]/p[input[i]];
        }
    }

    /**Generate sequence*/
    std::vector<int> Generate(int startIndex, int length)
    {
        std::vector<int> seq(length);
        int index = startIndex;
        seq[0] = index;
        for(int i=1;i<length;++i)
        {
            int tempIndex = 0;
            double maximum = 0;
        for(int j=0;j<N;++j)
        {
            for(int k=0; k<N; k++)
            {
                double temp = A[index][j]*A[j][k];
                //std::cerr << temp << std::endl;
                if(temp>maximum)
                {
                    tempIndex = j;
                    maximum = temp;
                }
            }
        }
        index = tempIndex;
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
