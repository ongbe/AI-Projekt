#include <unordered_map>
#include <cstring>
#include <algorithm>

class WordMap
{

public:
    std::unordered_map<std::string,int> wordToInt;
    std::unordered_map<int, std::string> intToWord;
    bool ok;
    int maxIndex = -1;
    std::vector<int>sequence;

    WordMap(){}

    WordMap(std::vector<std::string> in)
    {
        for (auto i : in)
        {
            bool temp = readFile(i);
            ok = (ok && temp);
        }
    }

    bool readFile(std::string in)
    {
        /* Set variables */
        std::string line, endLine;
        std::ifstream myfile (in);
        int start;
        if (in == "Petrarca.txt")
        {
            start = 6638;
            endLine = "INDEX.";
        }
        else
        {
            start = 288;
            endLine = "End of The Project Gutenberg Etext of Shakespeare's Sonnets";
        }

        /* import file */
        if (myfile.is_open())
        {
            for (int i = 0; i<start; ++i)
                getline(myfile, line);

            std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
            char unallowed[] = " -(),.!?:;";
            unallowed[0] = '"';
            while ( getline (myfile,line)  && line != endLine)
            {
                /* Skip non-poem lines */
                if (line.find_last_of (lowercase) != std::string::npos && line.find("_"))
                {
                    /* Trim special characters */
                    for (unsigned int i = 0; i < strlen(unallowed); ++i)
                    {
                      // you need include <algorithm> to use general algorithms like std::remove()
                      line.erase (std::remove(line.begin(), line.end(), unallowed[i]), line.end());
                    }
                    //spara ord + Baumwelch + add
                    //std::vector<int>sequence;
                    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
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
                            wordToInt[temp] = maxIndex;
                            intToWord[maxIndex] = temp;
                            sequence.push_back(maxIndex);
                        }
                        else
                        {
                            sequence.push_back(indexInVector(temp));
                        }
                    }
                    //sequences.push_back(sequence);
                }
            }
            myfile.close();
            std::cout << "Reading " << in << " done" << std::endl;
            return true;
        }

        else std::cout << "Unable to open "<< in << std::endl;
        return false;

    }

    bool inVector(std::string in)
    {
        return wordToInt.find(in) != wordToInt.end();
    }

    int indexInVector(std::string in)
    {
        return wordToInt[in];
    }
};
