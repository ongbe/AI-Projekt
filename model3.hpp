#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <limits>
#include <fstream>
#include <queue>
#include "wordmap.hpp"

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

struct Tree
{
    Tree(int word1, Tree parent1)
    {
        word = word1;
        //seq = parent -> seq;
        //seq.push_back(word1);
        parent = &parent1;
        syllables = parent1.syllables;
        depth = parent1.depth+1;
    }

    Tree(int word1)
    {
        word = word1;
        //seq.push_back(word);
    }

    Tree(bool leaf1, Tree parent1)
    {
        isLeaf = leaf1;
        parent = &parent1;
        syllables = parent1.syllables;
        depth = parent1.depth+1;
        //seq = parent -> seq;
    }

    //std::vector<int>seq;
    int depth = 1;
    int syllables = 0;
    int word;
    Tree* high; //best
    Tree* low; //second best
    Tree* parent;
    bool isLeaf = false;
};

class model
{
private:
    int N;// = 9;
    std::vector<wordObj>* words;
    double*val;
    WordMap ourMap;

public:
    /**Contructor - generate matrices*/
    model(int n, int m, WordMap &ourMap1)
    {
        N=n;
        std::cerr << "constructor called size: " << N << std::endl;
        words = (std::vector<wordObj>*)calloc(N,sizeof(std::vector<wordObj>));
        val = (double*)calloc(N,sizeof(double));
        for(int i=0;i<N;++i)
            val[i] = 10;
        ourMap = ourMap1;
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
        std::vector<int> seq = std::vector<int>(input.size());
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
    std::vector<int> Generate(int stopIndex, int syl)
    {
        Tree tree(stopIndex); //rot
        tree.syllables = ourMap.syllables(ourMap.intToWord[tree.word]);

        std::queue<Tree> Q;
        Q.push(tree);
        bool go = true;
        Tree temp = Q.front();
        while(!Q.empty() && go)
        {
            Tree temp = Q.front();
            Q.pop();

            std::cout << "Syllables " << temp.syllables << " Word: " << ourMap.intToWord[temp.word];
            std::cout << " parentWord: " << ourMap.intToWord[temp.parent -> word];
            std::cout << " parentParentWord: " << ourMap.intToWord[temp.parent -> parent -> word] << std::endl;

            if(temp.syllables>= syl)
            {
                go = false;
                std::cout << "break!" << std::endl;
                break;
            }

            //generate new nodes;
            int* a = trigram(temp.word);

            Tree right = Tree(a[0],temp);
            right.syllables = temp.syllables + ourMap.syllables(ourMap.intToWord[right.word]);

            //Tree left = Tree(a[1],temp);
            //left.syllables = temp.syllables + ourMap.syllables(ourMap.intToWord[left.word]);

            Q.push(right);
            //Q.push(left);
        }

        //find path back to tree
        std::cout << "Leta mening, length " << temp.depth << std::endl;

        int length = temp.depth;
        std::vector<int>sequence(length);

        for(int i=0;i<length;++i)
        {
            //std::cout << "word: " << ourMap.intToWord[goodNode -> word];
            //std::cout << " parentWord: " << ourMap.intToWord[goodNode -> parent -> word] << std::endl;

            sequence[i] = temp.word;//goodNode -> word;
            temp = *temp.parent; //goodNode -> parent;
        }

        return sequence;
    }

    /*
    void generateTree(Tree &parent,int depth)
    {
        if(parent.syllables > 10)
            return;

        parent.syllables += ourMap.syllables(ourMap.intToWord[parent.word]);
        std::cout << ourMap.intToWord[parent.word] << " " << parent.syllables << " Depth: "  << depth << std::endl;
        if(parent.syllables > 10)
            return;

        int* a = trigram(parent.word);
        generateTree(Tree(a[0], parent),depth+1);
        generateTree(Tree(a[1], parent),depth+1);
    }
    */

    /** Generate leaf (recursive)*/
    /*
    Tree leaf(Tree &parent)
    {
        std::cout << "leaf called " << std::endl;
        //if (parent.isLeaf)
        //{
            //return parent;
        //}

        std::cout << "efter if" << std::endl;

        Tree t1 = leaf(*parent.high);
        Tree t2 = leaf(*parent.low);

        int m0 = abs(10-parent.syllables);
        int m1 = abs(10-t1.syllables);
        int m2 = abs(10-t2.syllables);

        std::cout << "m0 " << m0 << " m1 " << m1 << " m2 " << m2 << std::endl;

        if(m0 >= m1 && m0 >= m2)
            return parent;
        if(m1 >= m0 >= m2)
            return t1;
        if(m2 >= m0 && m2 >= m1)
            return t2;




        //EJ FÄRDIGT!!
    }
    */
    /** returns the next word*/

    int* trigram(int index) //version 2 (med "fusk")
    {
        std::cerr << "trigram" << std::endl;

        int arr[N];
        for(int i=0;i<N;++i)
            arr[N]=0;

        int n = 2;
        int best[n][2];
        for(int i=0;i<n;++i)
        {
            best[i][0]=0;
            best[i][1]=0;
        }

        //int maximum = 0;
        int minimum = 0;
        int minIndex = 0;
        for(int i=0; i<words[index].size();++i)
        {
            //std::cout << arr[words[index][i].p1] << std::endl;
            arr[words[index][i].p1] += 1;
            if(arr[words[index][i].p1] > minimum)
            {
                //replace the smallest value in best
                best[minIndex][0] = words[index][i].p1;
                best[minIndex][1] = arr[words[index][i].p1];

                //find the smallest value in best, and update minimum and minIndex
                int m = 0;
                int in = 0;
                for(int j=0;j<n;++j)
                {
                    if(best[j][1]<=m)
                    {
                        in = j;
                        m = best[j][1];
                    }
                }
                minimum = m;
                minIndex = in;
            }
        }

        //Check trigrams
        /*
        double maximum1 = 0;
        double maximum2 = 0;
        int newIndex1 = 0;
        int newIndex2 = 0;
        for(int i=0;i<N;++i)
        {
            for(int j=0;j<N;++j)
            {
                float n1 = 1;
                float n2 = 0;
                //std:: cerr << words[index].size() << std::endl;
                for(int k=0;k<n;++k)
                {
                    if(words[index][best[k][0]].p1 == i)
                    {
                        n1++;
                        if(words[index][best[k][0]].p2 == j)
                        {
                            n2++;
                        }
                    }
                }
                float t = (n2/n1)*val[i]*val[j];
                //if(n2>1)
                    //std::cerr << "n1 " << n1 << " n2 " << n2 << " t: " << t << " i: " << i << std::endl;
                if(t>maximum1 && i!= index)
                {
                    if(maximum1>maximum2)
                    {
                        maximum2 = maximum1;
                        newIndex2 = newIndex1;
                    }
                    maximum1 = t;
                    newIndex1 = i;
                }
                else if(t>maximum2 && i!= index)
                {
                    maximum2 = t;
                    newIndex2 = i;
                }

            }
        }
        //std::cerr << maximum << "  " << newIndex << std::endl;
        //write best
        //for(int i=0;i<n;++i)
            //std::cerr << "best: " << best[i][0] << " " << best[i][1] << std::endl;;
        */
        int* r = (int*)calloc(2,sizeof(int));
        r[0] = best[0][0];//newIndex1;
        r[1] = best[1][0];//newIndex2;
        return r;
    }
    /*
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
    }*/

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
