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

    /**uses the sequence "input" to train the hmm-model
    * input = the emission sequence
    */
    void learn(std::vector<int> input)
    {
        double** C = initialize(N,N);

        //Initialize C
        for(int i=0; i<input.size()-1; i++)
        {
            C[input[i]][input[i+1]] += 1;
        }

        double** R = initialize(N,N);
        double** RB = initialize(N,N);
        double** Add = initialize(N,N);
        int it = 0;
        //Iterate
        while (it < 5)
        {
            R = ElementDiv(C, Mult(Mult(B,A, false, false), B, false, true));
            RB = Mult(R, B, false, false);
            A = ElementMult(A, Mult(B, RB, true, false), false, false);
            Add = ElementAdd(Mult(RB, A, false, true), Mult(R, Mult(B, A, false, false), true, false));
            B = ElementMult(B, Add, false, false);

            //Normalize A (all entries) & B (columns)
            int sumA = 0;
            int sumB;
            for(int i = 0; i<N; i++)
            {
                sumB = 0;
                for(int j=0; j<N; j++)
                {
                    sumA += A[i][j];
                    sumB += B[j][i];
                }
                for(int j=0; j<N; j++)
                {
                    B[j][i] = B[j][i]/sumB;
                }
            }
            for(int i = 0; i<N; i++)
            {
                for(int j=0; j<N; j++)
                {
                    A[i][j] = A[i][j]/sumA;
                }
            }

            it++;
        }

        //Normalize rows of A
        int sumA;
        for(int i = 0; i<N; i++)
        {
            sumA = 0;
            for(int j=0; j<N; j++)
            {
                sumA += A[i][j];
            }
            for(int j=0; j<N; j++)
            {
                A[i][j] = A[i][j]/sumA;
            }
        }

    }


    /**regular matrix multiplication
     * T1=true -> left matrix is transposed
     * T2=true -> right matrix is transposed
    */
    double** Mult(double** matrix1,double** matrix2, bool t1, bool t2)
    {
        bool T1 = t1;
        bool T2 = t2;
        double** temp = initialize(N,N);
        double t;
		int i,j,k;


		if(T1 && T2)
        {
            double**t = matrix1;
            matrix1 = matrix2;
            matrix2 = t;
            T1 = false;
            T2 = false;
        }

        if(!T1 && T2)
        {
            double**t = matrix1;
            matrix1 = matrix2;
            matrix2 = t;
            T1 = true;
            t2 = false;
        }



		//RÄKNA! (vanligt)
		if(!T1 && !T2)
        {
            for (i=0;i<N;++i)
            {
                for(j=0;j<N;++j)
                {
                    t = 0;
                    for(k=0;k<N;++k)
                        t+= matrix1[i][k] * matrix2[k][j];
                    temp[i][j] = t;
                }
            }
        }

        //RÄKNA! (matrix 1 transpose) (T1 true, T2 false)
		if(T1 && !T2)
        {
            for (i=0;i<N;++i)
            {
                for(j=0;j<N;++j)
                {
                    t = 0;
                    for(k=0;k<N;++k)
                        t+= matrix1[k][i] * matrix2[k][j];
                    temp[i][j] = t;
                }
            }
        }


		//std::cerr << "Mult klar" << std::endl;
        return temp;//initialize(N,N);
    }

    /**Element multiplication
     * T1=true -> left matrix is transposed
     * T2=true -> right matrix is transposed
    */
    double** ElementMult(double** matrix1,double** matrix2, bool T1, bool T2)
    {
        double** matrix = initialize(N,N);
        for(int i=0; i<N; i++)
        {
            for(int j=0; j<N; j++)
            {
                if (T1 == false && T2 == false)
                {
                    matrix[i][j] = matrix1[i][j]*matrix2[i][j];
                }
                else if (T1 == true && T2 == false)
                {
                    matrix[i][j] = matrix1[j][i]*matrix2[i][j];
                }
                else if (T1 == false && T2 == true)
                {
                    matrix[i][j] = matrix1[i][j]*matrix2[j][i];
                }
                else if (T1 == true && T2 == true)
                {
                    matrix[i][j] = matrix1[j][i]*matrix2[j][i];
                }
            }
        }
        return matrix;
    }

    /**Element division */
    double** ElementDiv(double** matrix1,double** matrix2)
    {
        double** matrix = initialize(N,N);
        for(int i=0; i<N; i++)
        {
            for(int j=0; j<N; j++)
            {
                matrix[i][j] = matrix1[i][j]/matrix2[i][j];
            }
        }
        return matrix;
    }

    double** ElementAdd(double** matrix1,double** matrix2)
    {
        double** matrix = initialize(N,N);
        for(int i=0; i<N; i++)
        {
            for(int j=0; j<N; j++)
            {
                matrix[i][j] = matrix1[i][j] + matrix2[i][j];
            }
        }
        return matrix;
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
                if(A[index][j]>maximum)
                {
                    tempIndex = j;
                    maximum = A[index][j];
                }
            }
            index = tempIndex;
            seq[i] = index;
        }
        return seq;
    }
};
