#include "oesenc/chartreader.h"

bool ChartReader::Open(const std::string &fileName)
{
    m_file = std::ifstream(fileName, std::ios::binary);

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
    const bool eof = (m_file.rdstate() & std::ifstream::eofbit);
    if (eof) {
        return false;
    }
    return result;
}
