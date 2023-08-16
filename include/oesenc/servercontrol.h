#pragma once

#include <chrono>
#include <string>

#ifdef OESENC_WINDOWS
struct _PROCESS_INFORMATION;
#endif

namespace oesenc {

#include "oesenc_export.h"

class OESENC_EXPORT ServerControl
{
public:
    struct Flags
    {
        Flags() = default;

        constexpr Flags(int value)
            : value(value)
        {
        }

        constexpr Flags operator|(const Flags &flags)
        {
            return Flags(value | flags.value);
        }

        constexpr Flags operator&(const Flags &flags)
        {
            return Flags(value & flags.value);
        }

        constexpr operator bool() const
        {
            return value != NoFlags;
        }

        int value = 0;

        static constexpr int NoFlags = 0;
        static constexpr int DontStartServer = 1;
        static constexpr int LeaveServerRunning = 2;
    };

    ServerControl(Flags flags = Flags::NoFlags, std::string_view pipeName = std::string());
    ServerControl(const ServerControl &other) = delete;
    ServerControl(ServerControl &&other) = delete;
    ServerControl &operator=(const ServerControl &other) = delete;
    ServerControl &operator=(ServerControl &&other) = delete;
    bool waitUntilReady(std::chrono::seconds timeout = std::chrono::seconds(10));
    bool isReady();
    std::string_view pipeName() const;
    ~ServerControl();

private:
    bool m_leaveServerRunning = true;
    std::string m_pipeName;

#ifdef OESENC_LINUX
    pid_t m_oexserverdPid = 0;
#endif

#ifdef OESENC_WINDOWS
    std::unique_ptr<_PROCESS_INFORMATION> m_processInfo;
#endif
};
}
