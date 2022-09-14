#pragma once
#include <Utility/noncopy/noncopyable.h>
#include <map>
#include <mutex>
#include "logSubscribe.h"
#include <list>
#include "logger.h"

namespace logger
{
    class LogProvider;

    class LOGGER_EXPORT LogModule
    {
        NONCOPYABLE(LogModule)
    private:
        LogModule() noexcept;
        ~LogModule();

    public:
        static LogModule* getInstance() noexcept;

    public:
        void startSubscribe(LogSubscribe* suber) noexcept;
        void stopSubscribe(LogSubscribe* suber) noexcept;
        const std::string& getProgramName() const noexcept { return m_programName; }

    public:
        void newlog(const Logger& logger,LogLevel level, const std::string& msg) noexcept;

    private:
        LogProvider* getLogProvider(const std::string& name) noexcept;

    private:
        std::map<std::string, LogProvider*> m_logProviderMap;
        std::mutex m_providerMutex;
        std::list<LogSubscribe*> m_subscribeList;
        std::string m_programName;
    };



}