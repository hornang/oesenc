#pragma once

#include <fstream>
#include <iostream>

class ChartReader
{
public:
    bool Open(const std::string &fileName);
    bool Read(void *buffer, size_t size);

private:
    std::ifstream m_file;
};
