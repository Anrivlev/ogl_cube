#include <fstream>
#include "../include/util.hpp"

bool ReadFile(const char *pFileName, std::string &outFile)
{
    std::ifstream f(pFileName);

    bool ret = false;

    if (f.is_open())
    {
        std::string line;
        while (getline(f, line))
        {
            outFile.append(line);
            outFile.append("\n");
        }

        f.close();

        ret = true;
    }

    return ret;
}
