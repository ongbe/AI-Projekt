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
        std::vector<double> p(N,1);
        for(int i=0;i<N;++i)
            for(int j=0;j<N;++j)
                A[i][j] = 0;

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
                /*
                for(int k=0; k<N; k++)
                {
                    double temp = A[index][j]*A[j][k]*val[j]*val[k];
                    //std::cerr << temp << std::endl;
                    if(temp>maximum)
                    {
                        tempIndex = j;
                        maximum = temp;
                    }
                }*/

                //quadgram
                /*
                std::cout << "ord: " << i << std::endl;
                for(int k=0; k<N; k++)
                {
                    for(int l=0;l<N;++l)
                    {§1§§§
                        double temp = A[index][j]*A[j][k]*A[k][l]*val[j]*val[k]*val[l];
                        if(temp>maximum)
                        {
                            tempIndex = j;
                            maximum = temp;
                        }
                    }
                }*/


            }
            index = tempIndex;
            val[index]*=0.5;
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
