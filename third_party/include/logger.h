#pragma once
#include <string>
#include <thread>
#include "logStream.hpp"
#include "logSubscribe.h"
#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#if defined(LOGGER_LIBRARY)
#  define LOGGER_EXPORT __declspec(dllexport)
#else
#  define LOGGER_EXPORT __declspec(dllimport)
#  pragma comment(lib,"Logger.lib")
#endif
#else
#define LOGGER_EXPORT
#endif


namespace logger
{

    class LOGGER_EXPORT Logger
    {
    public:
        Logger() noexcept;  //Logger("Root")
        Logger(const std::string& module_name) noexcept;
        ~Logger();

    public:
        LogStream<Logger> logStream(LogLevel level)  const noexcept { return LogStream<Logger>(this,level); }
        void log(LogLevel level, const std::string& msg) const noexcept;
        const std::string& getModuleName() const noexcept { return m_moduleName; }
        void setModuleName(const std::string& module_name) noexcept;

        void setObjectName(const std::string& object_name) noexcept;
        const std::string& getObjectName() const noexcept { return m_objectName; }

        void setDirName(const std::string& file_name) noexcept;
        const std::string& getDirName() const noexcept { return m_dirName; }

    private:
        std::string m_moduleName;
        std::string m_dirName;
        std::string m_objectName;
    };

    using log_stream = std::basic_ostream<char, std::char_traits<char>>;
    LOGGER_EXPORT log_stream& operator<<(log_stream& stream, const LogInfo& log_info) noexcept;
}





