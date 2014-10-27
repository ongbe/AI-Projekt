// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>

int main () {
  std::string line;
  std::ifstream myfile ("Petrarca.txt");
  if (myfile.is_open())
  {
    for (int i = 0; i<6638; ++i)
        getline(myfile, line);

    std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    char unallowed[] = "()-,.!?:;";
    while ( getline (myfile,line)  && line != "INDEX.")
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

            std::cout << line << std::endl;
        }
    }
    myfile.close();
  }

  else std::cout << "Unable to open file"<<std::endl;

  return 0;
}
