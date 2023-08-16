#pragma once

#include <istream>
#include <memory>
#include <string>

namespace oesenc {

#include "oesenc_export.h"

class OESENC_EXPORT ServerReader
{
public:
    ServerReader() = delete;

    enum class Command {
        ReadEsenc = 0,
        TestAvailability = 1,
        Exit = 2,
        ReadEsencHeader = 3,
        ReadOesu = 8,
    };

    [[nodiscard]] static std::unique_ptr<std::istream> request(std::string_view pipeName,
                                                               Command command,
                                                               std::string_view chartFile,
                                                               std::string_view userKey);

    [[nodiscard]] static std::unique_ptr<std::istream> openOesu(std::string_view pipeName,
                                                                std::string_view chartFile,
                                                                std::string_view key);

    [[nodiscard]] static std::unique_ptr<std::istream> openOesenc(std::string_view pipeName,
                                                                  std::string_view chartFile,
                                                                  std::string_view key);
};
}
