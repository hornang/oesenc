#pragma once

#include <fstream>
#include <iostream>

#include "oesenc_export.h"

class ChartReader
{
public:
    bool Open(const std::string &fileName);
    bool Read(void *buffer, size_t size);
    void close();

private:
    std::ifstream m_file;
};
