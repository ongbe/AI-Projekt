#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "wordmap.hpp"
#include <sstream>
#include <time.h>

class model
{
private:
    int N;// = 9; States
    int M;// = 9; Observations
    //double** A;
    //int* p; //number of times each word occurs
    std::unordered_map<std::string, double> mapBigrams;
    std::unordered_map<std::string, double> mapTrigrams;
    std::unordered_set<std::string> trigrams;
    std::vector<int> allOk;
    WordMap oneMap;
    double* weight;

public:
    /**Contructor - generate matrices*/
    model(int n, int m, WordMap inMap)
    {
        N=n;
        M=m;
        std::cerr << "constructor called size: " << N << "x" << M << std::endl;
        oneMap = inMap;
        weight = (double*)calloc(N,sizeof(double));
        for(int i=0;i<N;++i)
            weight[i] = 1.0;
    }

    std::string toString1(int one, int two)
    {
        std::stringstream ss;
        ss.str(""); // empty the stringstream
        ss.clear();
        ss << one;
        ss << " ";
        ss << two;
        std::string str2 = ss.str();
        return str2;
    }

    std::string toString2(int one, int two, int three)
    {
        std::stringstream ss;
        ss.str(""); // empty the stringstream
        ss.clear();
        ss << one;
        ss << " ";
        ss << two;
        ss << " ";
        ss << three;
        std::string str2 = ss.str();
        return str2;
    }

    /**uses the sequence "input" to train the hmm-model
    * input = the emission sequence
    */
    void learn(std::vector<int> in)
    {
        for (int i = in.size()-1; i > 1 ; i--)
        {
            //std::vector<int> temp1 = {in[i],in[i-1]}, temp2 = {in[i],in[i-1],in[i-2]};
            //std::string temp1 = ""; temp1.append(in[i]); temp1.append(" "); temp1.append(in[i-1]);
            //std::string temp1 = ""; temp1.append(in[i]); temp1.append(" "); temp1.append(in[i-1]);

            std::string temp1 = toString1(in[i],in[i-1]);
            std::string temp2 = toString2(in[i],in[i-1], in[i-2]);

            //std::string temp2 = ""+in[i]+" "+in[i-1]+" "+in[i-2];
            //int* tempBi =  temp1.data();
            mapBigrams[temp1]++; //If not previously existing (new bigram), automatically creates a node with value 0+1=1. Else, add 1 to value (count).
            //int* tempTri = temp2.data();
            mapTrigrams[temp2]++;
            trigrams.insert(temp2);
        }
    }

    /**Generate sequence*/
    std::vector<int> Generate(int lastWrd, int length)
    {
        //std::cout << "1Generation: " << lastWrd << std::endl;
        std::vector<int> out;

        //If "superlong" word LastWord (just in case)
        //if (length - 1 < 0)
        if (length - oneMap.syllables(oneMap.intToWord[lastWrd]) <= 0)
        {
            out.push_back(lastWrd);
            return out;
        }

        //Consider
        int bestWrd1;
        double maxFreq = 0, tempVal1, tempVal2;
        tempVal2 = count(oneMap.sequence.begin(), oneMap.sequence.end(), lastWrd);
        for(int i = 0; i < mapBigrams.size(); i++)
        {
            //std::vector<int> temp1 = {lastWrd,i};
            std::string temp1 = toString1(lastWrd,i);
            //int* tempBi =  temp1.data();
            if (mapBigrams.find(temp1) != mapBigrams.end())
            {
                //std::cout << "ok\t" << temp1 << "\t" << mapBigrams[temp1] << std::endl;
                tempVal1 = mapBigrams[temp1]* weight[i]/tempVal2;;
            }
            else
            {
                tempVal1 = weight[lastWrd]*tempVal2/(double)oneMap.sequence.size();
                //std::cout << tempVal1 << " " << weight[i] << " " << tempVal2 <<  " " << oneMap.sequence.size() << std::endl;
            }
            /*
            if((fabs(tempVal1 - maxFreq) < 0.5))
            {
                allOk.push_back(i);
            }
            else*/ if (tempVal1 > maxFreq)
            {
                //allOk.clear();
                bestWrd1 = i;
                maxFreq = tempVal1;
            }
        }
        /*if(!allOk.empty())
        {
            srand (time(NULL));
            bestWrd1 = allOk[rand() % allOk.size()];
            std::cout << "Allok best: "  << bestWrd1<< std::endl;
        }*/

        weight[bestWrd1]*=0.5;
        //Best word found! Generate the rest recursively.
        out = GenerateReq(lastWrd, bestWrd1, length-oneMap.syllables(oneMap.intToWord[lastWrd]));
        //out = GenerateReq(lastWrd, bestWrd1, length-1);
        out.push_back(lastWrd);

        int same = 0;
        bool c = false;
        //kolla om en rad existerar
        for(int i=0;i<oneMap.sequence.size();++i)
        {
            if(oneMap.sequence[i] == out[0])
            {
                same+=1;
                for(int j=1;j<out.size();++j)
                {
                    if(oneMap.sequence[i+j] == out[j])
                    {
                        same++;
                    }
                }
                if(same == out.size()-1)
                {
                    c=true;
                }
                else
                {
                    same = 0;
                }

            }
        }
        if(c)
            std::cout << "\tit's a Copy ";
        else
            std::cout << "\tnot a Copy  ";
        //

        int summa = 0;
        for(int i = 0; i < out.size(); i++)
            summa += oneMap.syllables(oneMap.intToWord[out[i]]);
        std::cout << "(" << summa << ")-";
        return out;
    }

    std::vector<int> GenerateReq(int lastWrd, int bestWrd, int length)
    {
        //std::cout << "Generation: " << lastWrd << std::endl;
        std::vector<int> out;

        //Break condition - out of syllables
        if (length - oneMap.syllables(oneMap.intToWord[bestWrd]) <= 0)
        {
            out.push_back(bestWrd);
            return out;
        }

        //Consider
        int bestWrd2 = 0;
        double maxFreq = 0, tempVal1, tempVal2;
        //std::cout << lastWrd << " " << bestWrd << std::endl;
        std::string temp1 = toString1(lastWrd, bestWrd);
        tempVal2 = mapBigrams[temp1];
        double w = count(oneMap.sequence.begin(), oneMap.sequence.end(), lastWrd);
        for(int j = 0; j < N; j++)
        {
            //Count occurrences
            //std::vector<int> temp2 = {lastWrd, bestWrd1, j};
            std::string temp2 = toString2(lastWrd,bestWrd,j);
            //int* tempTri =  temp2.data();
            if ((trigrams.find(temp2) != trigrams.end()) && length - oneMap.syllables(oneMap.intToWord[j]))
            {
                //std::cout << "YES\t" << j << "\t" << mapTrigrams[temp2] << std::endl;
                tempVal1 = mapTrigrams[temp2]*weight[j]/tempVal2;
            }
            else
            {
                tempVal1 = 0;//weight[bestWrd]*tempVal2/w;
                //std::cout << "else " << tempVal1 << " " << weight[j] << " " << tempVal2 << " " << w << std::endl;
            }

            //Check if max
            //std::cout << tempVal1 << "\t" << tempVal2 << std::endl;
            //std::cout << tempVal1/tempVal2 << std::endl;
            /*
            if((fabs(tempVal1 - maxFreq) < 1e-5) && tempVal1 - 1e-5 < 0)
            {
                allOk.push_back(j);
            }
            else*/ if (tempVal1 > maxFreq)
            {
                //allOk.clear();
                bestWrd2 = j;
                maxFreq = tempVal1;
            }
        }
        /*if(!allOk.empty())
        {
            bestWrd2 = allOk[rand() % allOk.size()];
        }*/
        //Best word found! Generate next word
        weight[bestWrd]*=0.5;
        out = GenerateReq(bestWrd, bestWrd2, length-oneMap.syllables(oneMap.intToWord[lastWrd]));
        //out = GenerateReq(bestWrd, bestWrd2, length-1);
        out.push_back(bestWrd);
        return out;
    }
};
