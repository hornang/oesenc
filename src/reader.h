#pragma once

#include <string>

namespace oesenc {
class IReader
{
public:
    virtual bool open() = 0;
    virtual bool read(void *buffer, size_t size) = 0;
    virtual void close() = 0;
};
}
