#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
class model
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    double** A;
    int* p; //number of times each word occurs

public:
    /**Contructor - generate matrices*/
    model(int n, int m)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;
        A = initialize(N,N);
        for(int i=0;i<N;++i)
            for(int j=0;j<N;++j)
                A[i][j] = 0;

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
            int tempIndex = 0;
            double maximum = 0;
            for(int j=0;j<N;++j)
            {
                //bigram
                /*
                if(A[index][j]>maximum)
                {
                    tempIndex = j;
                    maximum = A[index][j]*val[j];
                }*/


                //trigram

                for(int k=0; k<N; k++)
                {
                    double temp = A[index][j]*A[j][k];//*val[j];//*val[k];
                    //std::cerr << temp << std::endl;
                    if(temp>maximum)
                    {
                        tempIndex = j;
                        maximum = temp;
                    }
                }
            }
            index = tempIndex;
            //val[index] = ((val[index] *p[index])-1) / p[index];
            double t = ((A[seq[i+1]][index] *p[seq[i+1]])-1) / p[seq[i+1]];
            if(t>=0)
                A[seq[i+1]][index] = t;
            //val[index]*=0.5;
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
