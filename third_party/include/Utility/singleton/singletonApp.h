#pragma once
#include <Windows.h>
#include <atomic>
#include <filesystem>

#pragma  data_seg("SINGLETON")
std::atomic_int g_lAppInstance = 0;
#pragma  data_seg()
#pragma comment(linker,"/SECTION:SINGLETON,RWS")


namespace utility
{

class SingleAppliction
{
public:
    SingleAppliction()
       :m_isRunning(false)
    {
        if(++g_lAppInstance > 1)
        {
            m_isRunning = true;
        }
    }
    ~SingleAppliction() = default;

public:
    bool isRunning() const noexcept { return m_isRunning;}

private:
    bool m_isRunning;
};

}
