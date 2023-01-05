#include <cstring>
#include <iostream>

#include "vectorreader.h"

VectorReader::VectorReader(const std::vector<std::byte> &data)
    : m_data(data)
{
}

bool VectorReader::open()
{
    m_position = 0;
    return true;
}

bool VectorReader::read(void *buffer, size_t size)
{
    std::byte *bytes = static_cast<std::byte *>(m_data.data()) + m_position;

    if (m_position >= m_data.size()) {
        return false;
    } else if (m_position + size > m_data.size()) {
        memcpy(buffer, bytes, m_data.size() - m_position);
        return false;
    }

    memcpy(buffer, bytes, size);
    m_position += size;
    return true;
}
