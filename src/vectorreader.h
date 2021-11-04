#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "reader.h"

class VectorReader : public oesenc::IReader
{
public:
    VectorReader(const std::vector<std::byte> &data);
    bool open() override;
    bool read(void *buffer, size_t size) override;
    void close() override { }

private:
    std::vector<std::byte> m_data;
    size_t m_position = 0;
};
