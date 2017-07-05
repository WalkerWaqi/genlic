//
// Created by waqi on 9/18/16.
//

#include "CLicTool.h"
#include <fstream>

int CLicTool::read_file(std::string &file, std::string &data)
{
    int nRet = -1;

    std::ifstream rfile(file);
    if (!rfile.fail())
    {
        std::istreambuf_iterator<char> beg(rfile), end;
        data.append(beg, end);

        rfile.close();

        nRet = 0;
    }

    return nRet;
}

int CLicTool::write_file(std::string &file, std::string &data)
{
    int nRet = -1;

    std::ofstream wfile(file);
    if (!wfile.fail())
    {
        wfile << data;
        wfile.flush();
        wfile.close();

        nRet = 0;
    }

    return nRet;
}