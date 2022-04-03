#pragma once

#include <string>
#include <unordered_map>

#include "oesenc_export.h"

namespace oesenc {

class KeyListReader
{
public:
    enum class Status {
        Unknown,
        Success,
        Partial,
        Failed,
    };

    OESENC_EXPORT KeyListReader() = default;
    OESENC_EXPORT Status load(const std::string &filename);
    OESENC_EXPORT std::string key(const std::string &chartName) const;

private:
    std::unordered_map<std::string, std::string> m_chartKeys;
};
}
