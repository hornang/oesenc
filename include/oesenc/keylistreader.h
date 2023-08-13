#pragma once

#include <string>
#include <unordered_map>

#include "oesenc_export.h"

namespace oesenc {

class OESENC_EXPORT KeyListReader
{
public:
    static std::unordered_map<std::string, std::string> readOesuKeys(std::string_view chartDir);
};
}
