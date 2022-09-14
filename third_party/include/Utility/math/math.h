#pragma once
#include <QPoint>
#include <QPointF>

namespace  utility {

template<class T>
constexpr T pi = T(3.1415926535897932385);

template<typename... Args>
auto sum(Args... args) { return (... + args);}


template<typename Point>
double computeDistance(const Point& p1,const Point& p2) noexcept
{
	QPointF distance = p1 - p2;
    return std::sqrt(std::pow(distance.x(), 2) + std::pow(distance.y(), 2));
}

inline uint16_t computeAngleGap(uint16_t angle1, uint16_t angle2) noexcept
{
    auto del = std::abs(angle1 - angle2) % 36000;
    if (del > 18000)
    {
        del = 36000 - del;
    }
    return del;
}


template <typename T>
bool equal( T t1, T t2)
{
    return t1 == t2;
}

template <>
inline bool equal<double>(double t1,double t2)
{
    return std::abs(t1-t2) < 0.000001;
}

/*************************************************
Function:       // computeLinearEquation
Description:    // ax+ by + c =0; 计算直线方程
Input:          // QPoint 点，int 角度
Output:         // int& a,int& b,int& abc系数
Return:         // 无
Others:         // 无
*************************************************/
static void computeLinearEquation(const QPoint& p, int angle, double& a, double& b, double& c)
{
    if (angle == 9000 || angle == 27000)
    {
        a = 1;
        b = 0;
        c = -p.x();
    }
    else
    {
        b = 1;
        a = -std::tan(angle * utility::pi<double> / 18000);
        c = -a * p.x() - p.y();
    }
}

static void computeLinearEquation(const QPoint& p1, const QPoint& p2, double& a, double& b, double& c)
{
    auto del = p2 - p1;
    if (del.x() == 0)
    {
        a = 1;
        b = 0;
        c = -p1.x();
    }
    else
    {
        b = 1;
        a = -del.y() / del.x();
        c = -a * p1.x() - p1.y();
    }
}

static double gapPointToLine(const QPoint& p1, double a, double b, double c)
{
    return std::abs(a* p1.x() + b * p1.y() + c) / (std::sqrt(a*a + b * b));
}

static double gapPointToLine(const QPoint& p,const QPoint& line_p1, const QPoint& line_p2)
{
    double a, b, c;
    computeLinearEquation(line_p1, line_p2, a, b, c);
    return gapPointToLine(p,a,b,c);
}

static uint16_t computeAngleDis(uint16_t angle1, uint16_t angle2)  noexcept
{
    if (angle1 == 0xFFFF)
    {
        return 0;
    }
    uint16_t d_angle = static_cast<uint16_t>(std::abs(angle1 - angle2));
    return d_angle > 18000 ? (36000 - d_angle) : d_angle;
}
}


