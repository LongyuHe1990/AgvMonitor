#pragma once
#include <Utility/noncopy/noncopyable.h>
#include "logStream.hpp"

namespace logger
{
    class LogProvider
    {
        NONCOPYABLE(LogProvider)
    private:
        LogProvider(const std::string& dir_name) noexcept;
        ~LogProvider();
        friend class LogModule;

    public:
        void log(LogLevel level, const std::string& msg) const noexcept;

    private:
        void init() noexcept;
        void clear() noexcept;

    private:
        std::string m_dirName;
    };

}