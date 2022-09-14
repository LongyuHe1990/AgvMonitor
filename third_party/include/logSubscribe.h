#pragma once
#include "logStream.hpp"

namespace logger
{
    class LogSubscribe
    {
    public:
        LogSubscribe() = default;
        ~LogSubscribe() = default;

    public:
        virtual void newlog(const LogInfo& log_info) noexcept = 0;
    };

}
