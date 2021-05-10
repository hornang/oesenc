#include "oesenc/chartreader.h"

bool ChartReader::Open(const std::string &fileName)
{
    m_file = std::ifstream(fileName);

    if (!m_file.is_open()) {
        std::cout << "Unable to open file" << std::endl;
        return false;
    }

    return true;
}

bool ChartReader::Read(void *buffer, size_t size)
{
    if (!m_file.is_open()) {
        std::cerr << "File not open" << std::endl;
    }
    m_file.read(reinterpret_cast<char *>(buffer), size);
    bool result = (m_file.rdstate() & std::ifstream::goodbit) == 0;
    return result;
}
