#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "reader.h"

class FileReader : public oesenc::IReader
{
public:
    FileReader(const std::string &fileName);
    bool open() override;
    bool read(void *buffer, size_t size) override;
    void close() override;

private:
    std::string m_fileName;
    std::ifstream m_file;
};
