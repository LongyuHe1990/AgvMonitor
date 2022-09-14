#pragma once
#include <chrono>
#include <iostream>
#include <ctime>
#include <time.h>
#include <iomanip>

namespace utility
{
	class Time
	{
	public:
        Time() noexcept
            :m_lastTime(std::chrono::system_clock::now())
        {}
        ~Time() = default;

	public:
		void restart() noexcept { m_lastTime = std::chrono::system_clock::now(); }
		int elapsed()  const noexcept
		{
			auto now_time = std::chrono::system_clock::now();
			return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(now_time - m_lastTime).count());
		}

	private:
		decltype(std::chrono::system_clock::now()) m_lastTime;
	};


    class TimePoint
    {
    public:
        TimePoint(const std::chrono::system_clock::time_point& time_point) noexcept
        {
            auto time_t = std::chrono::system_clock::to_time_t(time_point);
            tm * cm = localtime(&time_t);
            m_year = cm->tm_year + 1900;
            m_month = cm->tm_mon + 1;
            m_day = cm->tm_mday;
            m_hour = cm->tm_hour;
            m_minite = cm->tm_min;
            m_second = cm->tm_sec;
            m_msec = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count() % 1000; 
        }
        TimePoint(time_t time) noexcept
            :TimePoint(std::chrono::system_clock::time_point(std::chrono::system_clock::duration(time)))
        {
            
        }
        ~TimePoint() = default;

        template<class _Ch,class _Tr>
        friend std::basic_ostream<_Ch, _Tr>& operator<<(std::basic_ostream<_Ch, _Tr>& stream,const TimePoint& time_point)
        {	// insert representation into stream
            return (stream << time_point.m_year << "-" << std::setw(2) << std::setfill('0') << time_point.m_month << "-" << std::setw(2) << std::setfill('0') << time_point.m_day << " " << std::setw(2) << std::setfill('0') << time_point.m_hour << ":" << std::setw(2) << std::setfill('0') << time_point.m_minite << ":"<<std::setw(2) << std::setfill('0') << time_point.m_second << "."<< std::setw(3) << std::setfill('0') << time_point.m_msec);
        }

        static TimePoint getCurrentTime() noexcept { return TimePoint(std::chrono::system_clock::now()); }

    private:
        int m_year;
        int m_month;
        int m_day;
        int m_hour;
        int m_minite;
        int m_second;
        int m_msec;
    };
}
