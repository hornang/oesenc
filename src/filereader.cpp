#include <iostream>

#include "filereader.h"

FileReader::FileReader(const std::string &fileName)
    : m_fileName(fileName)
{
}

bool FileReader::open()
{
    m_file = std::ifstream(m_fileName, std::ios::binary);

    if (!m_file.is_open()) {
        std::cout << "Unable to open file" << std::endl;
        return false;
    }

    return true;
}

bool FileReader::read(void *buffer, size_t size)
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

void FileReader::close()
{
    m_file.close();
}
