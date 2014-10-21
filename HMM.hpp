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
    int N;// = 9;
    int M;// = 9;
    bool converged;// = true;
    double**A;
    double**B;
    double*q;
    double*qT;

public:
	/**Contructor*/
	HMM(int n, int m)
	{
		N=n;
		M=m;
		std::cerr << "contructor called size: " << N << "x" << M << std::endl;

		//A
		A = (double**)calloc(N,sizeof(double*));
		for(int i=0;i<N;++i)
			A[i] = (double*)calloc(N,sizeof(double));
		//B
		B = (double**)calloc(N,sizeof(double*));
		for(int i=0;i<M;++i)
			B[i] = (double*)calloc(M,sizeof(double));
		//q
		q = (double*)calloc(N,sizeof(double));
		//qT q at last timestep
		qT = (double*)calloc(N,sizeof(double));

		converged = false;
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

	/** sets A,b,q to random values */
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

        //q
        double sum = 0;
        for(int j=0;j<N;++j)
        {
            double r = (rand()%100)+1;
            sum+=r;
            q[j] = r;
        }
        for(int j=0;j<N;++j)
        {
            q[j] = q[j]/sum;
        }
    }

	/** Adds a small number to A,B,q */
	void add()
    {
        double extra = 1./N;
        //A
        for(int i=0;i<N;++i)
        {
            double sum = 0;
            for(int j=0;j<N;++j)
            {
                A[i][j] += extra;
                sum += A[i][j];
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
                B[i][j] += extra;
                sum += B[i][j];
            }
            for(int j=0;j<N;++j)
            {
                B[i][j] = B[i][j]/sum;
            }
        }

        //q
        double sum = 0;
        for(int j=0;j<N;++j)
        {
            q[j] += extra;
            sum += q[j];
        }
        for(int j=0;j<N;++j)
        {
            q[j] = q[j]/sum;
        }
    }

	/**viterbi*/
    std::vector<int> Viterbi(std::vector<int> seq)
    {
        int state;

        int T = (int)seq.size();

        double maximum = -std::numeric_limits<double>::max();
        double* tempArray = (double*)calloc(N,sizeof(double));
        double* prob = (double*)calloc(N,sizeof(double));
        int sequences[T][N];

        //t=0;
        for(int i=0;i<N;++i)
        {
            prob[i] = q[i]*B[i][seq[0]];
            int index = 0;
            if(prob[i]> maximum)
            {
                maximum = prob[i];
                index = i;
            }
            sequences[0][i] = index;
        }

        for(int t=1;t<T;++t)
        {
            state = seq[t];
            for(int i=0;i<N;++i)
            {
                maximum = prob[0] * A[0][i]* B[i][state];
                int index = 0;
                for(int k=1;k<N;++k)
                {
                    double te = prob[k] * A[k][i] * B[i][state];
                    if(te > maximum)
                    {
                        maximum = te;
                        index = k;
                    }
                }

                tempArray[i] = maximum;
                sequences[t][i] = index;
            }

            for(int i=0;i<N;++i)
                prob[i] = tempArray[i];
        }

        int current = 0;
        maximum = prob[0];
        for(int i=1;i<N;++i)
        {
            if(prob[i]>maximum)
            {
                maximum = prob[i];
                current = i;
            }
        }

        std::vector<int> e(T);
        for(int t = (T-1);t>=0 ;t--)
        {
            e[t] = current;
            current = sequences[t][current];
        }
        return e;
    }

	/**BaumWelch*/
    void BaumWelch(std::vector<int> seq)
    {
        //std::cerr << "BAUM" << std::endl;
        int T = (int)seq.size();

        int MaxItter = 200;
        int itter = 0;
        double OldLogProb = -std::numeric_limits<double>::max();

        //Massa matriser
        double* c = (double*) calloc(T,sizeof(double)); //c[time]
        double** alpha = initialize(T, N);  //alpha[time][index]
        double** beta = initialize(T, N);  //beta[time][index]
        double** Gamma = initialize(T,N);
        double*** diGamma = (double***)calloc(T,sizeof(double**)); //diGamma[time][index1][index2]
        for(int t=0;t<T;++t)
            diGamma[t] = initialize(N,N);

        bool GO = true;
        while((itter < MaxItter) && GO)
        {

            /**----------Alpha-pass------------------------*/
            //std::cerr << "alpha-pass" << std::endl;
            //alpha-0
            c[0] = 0;

            for(int i=0;i<N;++i)
            {
                alpha[0][i] = q[i]*B[i][seq[0]];
                c[0] += alpha[0][i];
            }
            //scale
            c[0] = 1.0/c[0];
            for(int i=0;i<N;++i)
            {
                alpha[0][i] *= c[0];
            }

            //alpha-t
            for(int t=1;t<T;++t)
            {
                c[t] = 0;
                for(int i=0;i<N;++i)
                {
                    alpha[t][i] = 0;
                    for(int j=0;j<N;++j)
                    {
                        alpha[t][i] += alpha[t-1][j]*A[j][i];
                    }
                    alpha[t][i] *= B[i][seq[t]];
                    c[t] += alpha[t][i];
                }

                //scale
                c[t] = 1.0/c[t];
                for(int i=0;i<N;++i)
                {
                    alpha[t][i] *= c[t];
                }
            }

            /**----------Beta-pass------------------------*/
            //std::cerr << "Beta-pass" << std::endl;
            //beta T-1
            for(int i=0;i<N;++i)
                beta[T-1][i] = c[T-1];

            for(int t=T-2; t>=0;--t)
            {
                for(int i=0;i<N;++i)
                {
                    beta[t][i] = 0;
                    for(int j=0;j<N;++j)
                    {
                        beta[t][i] += A[i][j] * B[j][seq[t+1]] * beta[t+1][j];
                    }
                    //scale
                    beta[t][i] *= c[t];
                }
            }


            /**----------diGamma & Gamma----------------*/
            //std::cerr << "diGamma&Gamma" << std::endl;
            for(int t=0;t<(T-1);++t)
            {
                double denom = 0;
                for(int i=0;i<N;++i)
                    for(int j=0;j<N;++j)
                        denom += alpha[t][i] * A[i][j] * B[j][seq[t+1]] * beta[t+1][j];

                for(int i=0;i<N;++i)
                {
                    Gamma[t][i] = 0;
                    for(int j=0;j<N;++j)
                    {
                        diGamma[t][i][j] = (alpha[t][i] * A[i][j] * B[j][seq[t+1]] * beta[t+1][j]) / denom;
                        Gamma[t][i] += diGamma[t][i][j];
                    }
                }
            }

            /**----------Estimate A,b,q------------------*/

            /*q*/
            for(int i=0;i<N;++i)
            {
                q[i] = Gamma[0][i];
                qT[i] = alpha[T-1][i];
            }

            /*A*/
            for(int i=0;i<N;++i)
            {
                for(int j=0;j<N;++j)
                {
                    double numer = 0;
                    double denom = 0;
                    for(int t=0;t<(T-1);++t)
                    {
                        numer += diGamma[t][i][j];
                        denom += Gamma[t][i];
                    }
                A[i][j] = numer/denom;
                //std::cerr << "A : numer=" << numer <<", denom=" << denom << std::endl;
                }
            }

            /*B*/
            for(int i=0;i<N;++i)
            {
                for(int j=0;j<M;++j)
                {
                    double numer = 0;
                    double denom = 0;
                    for(int t=0;t<(T-1);++t)
                    {
                        if(seq[t] == j)
                            numer += Gamma[t][i];
                        denom += Gamma[t][i];
                    }
                B[i][j] = numer/denom;
                //std::cerr << "B : numer=" << numer <<", denom=" << denom << std::endl;
                }
            }

            double logProb = 0;
            for(int t=0;t<T;++t)
                logProb += log(c[t]);
            logProb *= -1;

            if(logProb <= OldLogProb)
            {
                GO = false;
            }

            OldLogProb = logProb;
            itter++;
        }

        bool batman = false;
        for(int i=0;i<N && !batman;++i)
            for(int j=0;j<N && !batman;++j)
                if(std::isnan(A[i][j]))
                {
                    batman = true;
                    A[i][j] = 0;
                }
        for(int i=0;i<N && !batman;++i)
            for(int j=0;j<M && !batman;++j)
                if(std::isnan(B[i][j]))
                {
                    batman = true;
                    B[i][j] = 0;
                }
        if(batman)
            std::cerr << "NaN" << std::endl;

        converged = true;
        if((itter == MaxItter) || (batman))
            converged = false;

        std::cerr << "Itterations " << itter << std::endl;
    }

    bool isConverged()
    {
        return converged;
    }

};
