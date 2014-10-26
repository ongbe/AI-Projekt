#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>

class HMM
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    double** A;
    double** B;

public:
	/**Contructor - generate matrices*/
	HMM(int n, int m)
	{
		N=n;
		M=m;
		std::cerr << "constructor called size: " << N << "x" << M << std::endl;
		A = initialize(N,N);
		B = initialize(N,M);

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

	/** sets A,b,q to random values (starting guess)*/
	void reset()
    {
        //A
        for(int i=0;i<N;++i)
        {
            double sum = 0;
            for(int j=0;j<N;++j)
            {
                double r = (rand()%100)+1;
                sum+=r;
                A[i][j] = r;
            }
            for(int j=0;j<N;++j)
            {
                A[i][j] = A[i][j]/sum;
            }
        }

        //B
        for(int i=0;i<M;++i)
        {
            double sum = 0;
            for(int j=0;j<N;++j)
            {
                double r = (rand()%100)+1;
                sum+=r;
                B[i][j] = r;
            }
            for(int j=0;j<N;++j)
            {
                B[i][j] = B[i][j]/sum;
            }
        }
    }

    /**uses the sequence "input" to train the hmm-model*/
    void learn(std::vector<int> input)
    {
        double** C = initialize(N,N);

        //fixa C!

        //Iterate


    }

    /**regular matrix multiplication
     * T1=true -> left matrix is transposed
     * T2=true -> right matrix is transposed
    */
    double** Mult(double** matrix1,double** matrix2, bool T1, bool T2)
    {

        return initialize(N,N);
    }

    /**Element multiplication
     * T1=true -> left matrix is transposed
     * T2=true -> right matrix is transposed
    */
    double** ElementMult(double** matrix1,double** matrix2, bool T1, bool T2)
    {

        return initialize(N,N);
    }

    /**Element division */
    double** ElementDiv(double** matrix1,double** matrix2)
    {

        return initialize(N,N);
    }

    /**Generate sequence*/
    /*std::vector<int> Generate(int startIndex, int length)
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
                if(A.A[index][j]>maximum)
                {
                    tempIndex = j;
                    maximum = A.A[index][j];
                }
            }
            index = tempIndex;
            seq[i] = index;
        }
        return seq;
    }*/
};
