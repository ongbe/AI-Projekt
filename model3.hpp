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
    wordObj(int i, int j, int k)
    {
        word = i;
        p1 = j;
        p2 = k;
    }
    int word;
    int p1;
    int p2;
};

class model
{
private:
    int N;// = 9;
    std::vector<wordObj>* words;
    std::vector<int> seq;
    double*val;

public:
    /**Contructor - generate matrices*/
    model(int n, int m)
    {
        N=n;
        std::cerr << "constructor called size: " << N << std::endl;
        words = (std::vector<wordObj>*)calloc(N,sizeof(std::vector<wordObj>));
        val = (double*)calloc(N,sizeof(double));
        for(int i=0;i<N;++i)
            val[i] = 10;
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
        seq = std::vector<int>(input.size());
        int j = input.size()-1;
        int i=0;
        while(i<input.size())
        {
            seq[i] = input[j];
            i++;
            j--;

        }

        for(int i=0;i<seq.size()-2;++i)
        {
            words[seq[i]].push_back(wordObj(seq[i],seq[i+1],seq[i+2]));
        }
        //words[seq[i]].push_back(wordObj(seq[i],seq[i+1],seq[i+2])); l-2
        //words[seq[i]].push_back(wordObj(seq[i],seq[i+1],seq[i+2])); l-1
    }

    /**Generate sequence*/
    std::vector<int> Generate(int stopIndex, int length)
    {
        std::vector<int> sequence(length);
        int index = stopIndex;
        sequence[length-1] = index;
        for(int i=length-2;i>=0;--i)
        {
            index = trigram(index);
            sequence[i] = index;
            val[index] = val[index]*0.5;
            std::cout << index << std::endl;
        }
        return sequence;
    }

    /** returns the next word*/
    /*
    int trigram(int index) //version 2 (med "fusk")
    {
        std::cerr << "trigram" << std::endl;

        int arr[N];
        for(int i=0;i<N;++i)
            arr[N]=0;

        int maximum = 0;
        int newIndex = 0;
        for(int i=0; i<words[index].size();++i)
        {
            //std::cout << arr[words[index][i].p1] << std::endl;
            arr[words[index][i].p1] += 1;
            if(arr[words[index][i].p1] > maximum)
            {
                maximum = arr[words[index][i].p1];
                newIndex = i;
            }
        }
        std::cerr << maximum << "  " << newIndex << std::endl;

        return newIndex;
    }
    */
    int trigram(int index)
    {
        std::cerr << "trigram" << std::endl;
        int tempIndex = 0;
        float maximum = 0;

        for(int i=0;i<N;++i)
        {
            for(int j=0;j<N;++j)
            {
                float n1 = 1;
                float n2 = 0;
                //std:: cerr << words[index].size() << std::endl;
                for(int k=0;k<words[index].size();++k)
                {
                    if(words[index][k].p1 == i)
                    {
                        n1++;
                        if(words[index][k].p2 == j)
                        {
                            n2++;
                        }
                    }
                }
                float t = (n2/n1)*val[i]*val[j];
                if(n2>1)
                    std::cerr << "n1 " << n1 << " n2 " << n2 << " t: " << t << " i: " << i << std::endl;
                if(t>maximum && i!= index)
                {
                    maximum = t;
                    tempIndex = i;
                }
            }
        }
        return tempIndex;
    }

    void print(double ** AA)
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
