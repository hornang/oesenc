#pragma once

#include <string>
#include <unordered_map>

#include "oesenc_export.h"

namespace oesenc {

class OESENC_EXPORT KeyListReader
{
public:
    enum class Status {
        Unknown,
        Success,
        Partial,
        Failed,
    };

    KeyListReader() = default;
    Status load(const std::string &filename);
    std::string key(const std::string &chartName) const;

private:
    std::unordered_map<std::string, std::string> m_chartKeys;
};
}
