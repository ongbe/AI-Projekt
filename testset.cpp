#include <iostream>
#include <unordered_set>
#include <fstream>
#include <sstream>
//#include <string>
#include <vector>

std::unordered_set<std::string> wordset;
std::vector<std::string> words;
std::vector<int> index;
//std::vector< std::vector<int> > sequences;
int maxIndex = -1;

bool inVector(std::string in);
int indexInVector(std::string in);
std::string getWord(int i);


bool inVector(std::string in)
{
    if(wordset.find(in) != wordset.end()) return true;
    else return false;
}

int main ()
{
    std::string line;
    //std::ifstream myfile ("poems.txt");
    std::ifstream myfile("ShakespeareSonnets.txt");

    std::vector<int>sequence;

    if (myfile.is_open())
    {
        /* Skip copyright notices etcetera */
        //for(int i = 0; i < 288; ++i) getline(myfile,line);
        /* Actual used code*/
        while ( getline (myfile,line) && line != "End of The Project Gutenberg Etext of Shakespeare's Sonnets")
        {
            if (line.length() >= 10)
            {
                std::cout << line << std::endl;
                //spara ord + Baumwelch + add
                //std::vector<int>sequence;
                std::istringstream iss;
                iss.str(line);
                while (!iss.eof())
                {
                    std::string temp;
                    iss >> temp;
                    //std::cerr << temp << std::endl;
                    if(!inVector(temp))
                    {
                        maxIndex++;
                        wordset.insert(temp);
                        words.push_back(temp);
                        index.push_back(maxIndex);
                        sequence.push_back(maxIndex);
                    }
                    /*else
                    {
                        sequence.push_back(indexInVector(temp));
                    }*/
                }
                //sequences.push_back(sequence);
            }
        }
        myfile.close();
    }
    else std::cout << "Unable to open file";
    std::cout << "Reading done" << std::endl;
}
