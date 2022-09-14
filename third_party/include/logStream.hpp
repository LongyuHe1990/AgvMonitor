#pragma once

#include <sstream>
#include <functional>
#include <Utility/noncopy/noncopyable.h>
#include <chrono>

namespace logger
{
    enum class LogLevel
    {
        Debug = 1,
        Info,
        Warn,
        Error,
        Fatal
    };

    struct  LogInfo
    {
    public:
        std::string moduleName;
        std::string objectName;
        std::string threadId;
        std::string timeStamp;
        logger::LogLevel logLevel;
        std::string message;
    };


    template <typename Logger>
    class  LogStream
    {
        NONCOPYABLE(LogStream)
    public:
        LogStream(const Logger* log,LogLevel level) noexcept
            : m_level(level)
            , m_log(log)
            , m_stream(new std::ostringstream)
        {
        }
        LogStream(LogStream&& log_stream) noexcept
            : m_level(log_stream.m_level)
            , m_log(log_stream.m_log)
            , m_stream(log_stream.m_stream)
        {
            log_stream.m_log = nullptr;
            log_stream.m_stream = nullptr;
        }

        ~LogStream()
        {
            if (m_log)
            {
                auto log_info = m_stream->str();
                m_log->log(m_level, log_info);
                delete m_stream;
            }
        }

    public:
        std::ostringstream& stream() noexcept { return *m_stream; }

    private:
        const LogLevel m_level;
        const Logger* m_log;
        std::ostringstream* m_stream;
    };

}


#define LOGGER_FATAL(log_ptr) (log_ptr)->logStream(logger::LogLevel::Fatal).stream()
#define LOGGER_ERROR(log_ptr) (log_ptr)->logStream(logger::LogLevel::Error).stream()
#define LOGGER_WARN(log_ptr)  (log_ptr)->logStream(logger::LogLevel::Warn).stream()
#define LOGGER_INFO(log_ptr)  (log_ptr)->logStream(logger::LogLevel::Info).stream()
#define LOGGER_DEBUG(log_ptr) (log_ptr)->logStream(logger::LogLevel::Debug).stream()


