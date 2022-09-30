#include "barchart.h"
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

BarChart::BarChart(QWidget *parent) : QWidget(parent),
    m_barWidth(18),
    m_axisColor(qRgb(233, 238, 250)),
    m_abscissaSpacing(30),
    m_ordinateSpacing(30)
{
    setStyleSheet("background-color: rgb(19, 39, 58)");
    m_ordinateValues.push_back("0");
    m_ordinateValues.push_back("60");
    m_ordinateValues.push_back("65");
    m_ordinateValues.push_back("70");
    m_ordinateValues.push_back("75");
    m_ordinateValues.push_back("80");
    m_ordinateValues.push_back("85");
    m_ordinateValues.push_back("90");
    m_ordinateValues.push_back("95");
    m_ordinateValues.push_back("100");
}

void BarChart::setBarChartDatas(QList<BarInfo> option)
{
    m_barList.clear();
    m_barList = option;

    update();
}

void BarChart::initBarChart()
{
    m_barList.clear();

    update();
}

void BarChart::resizeEvent(QResizeEvent *event)
{
    double v = (height() - m_ordinateSpacing * 2) % (m_ordinateValues.size()-1);
    double h = (width() - m_abscissaSpacing) % (m_barList.size());
    m_axisRect.setRect(m_abscissaSpacing,m_ordinateSpacing,width()-m_abscissaSpacing-h,height()-m_ordinateSpacing*2-v);
    QWidget::resizeEvent(event);
}

void BarChart::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    QPainter p(this);
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //绘制横坐标
    drawAbscissa(painter);
    drawOrdinate(painter);
    drawBar(painter);
}

void BarChart::drawAbscissa(QPainter &painter)
{
    painter.save();
    painter.setPen(QPen(m_axisColor,1));
    painter.setBrush(m_axisColor);
    painter.drawRect(m_axisRect.bottomLeft().x()+20,m_axisRect.bottomLeft().y(),m_axisRect.width()-40,2);

    int spacing = (m_axisRect.width() - 20) / (m_barList.size());
    for(int i = 0; i < m_barList.size(); i++)
    {
        painter.setFont(QFont("微软雅黑", 8, 10));
        QRect rect((m_axisRect.bottomLeft().x() + 10) + (spacing * i), m_axisRect.bottomLeft().y() + 5, spacing, m_abscissaSpacing-10);
        m_barList[i].rect = rect;
        //painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter, m_barList.at(i).name);
    }

    painter.restore();
}

void BarChart::drawOrdinate(QPainter &painter)
{
    m_ordinateScaleRectList.clear();
    //painter.drawRect(m_axisRect);
    painter.save();
    painter.setPen(QPen(m_axisColor,1));
    painter.setFont(QFont("微软雅黑", 9, 10));
    painter.drawText(QRect(m_axisRect.x() - m_ordinateSpacing, m_axisRect.y() - m_ordinateSpacing, m_ordinateSpacing*2, m_ordinateSpacing), Qt::AlignCenter, "单位/%");
    painter.restore();
    painter.save();
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    pen.setBrush(m_axisColor);
    painter.setPen(pen);
    painter.drawLine(m_axisRect.x(),m_axisRect.y(),m_axisRect.x()+1,m_axisRect.bottomLeft().y());
    painter.restore();
    int spacing = m_axisRect.height() / (m_ordinateValues.size()-1);
    int startY = m_axisRect.bottomLeft().y()-spacing + painter.pen().width();
    for(int i = 0; i < m_ordinateValues.size(); ++i)
    {
        painter.save();
        QRect rect(0,startY - (spacing*i),m_ordinateSpacing,spacing);
        m_ordinateScaleRectList.push_back(rect);
        painter.setPen(QPen(m_axisColor,1));
        //painter.drawRect(rect);
        QRect textRect(rect.bottomLeft().x(), rect.bottomLeft().y()-10 + painter.pen().width(), m_abscissaSpacing - 5,20);
        //painter.drawRect(textRect);
        painter.setFont(QFont("微软雅黑", 8, 10));
        painter.drawText(textRect,Qt::AlignCenter | Qt::AlignRight,m_ordinateValues.at(i));
        painter.restore();
        painter.save();
        painter.setPen(QPen(m_axisColor,1));
        painter.setBrush(m_axisColor);
        painter.drawRect(m_axisRect.x(),textRect.bottomRight().y() - textRect.height()/2,10,2);
        painter.restore();
    }
}

void BarChart::drawBar(QPainter& painter)
{
    for(int i = 0; i < m_barList.length(); ++i)
    {
        BarInfo barInfo = m_barList.at(i);
        int startX = barInfo.rect.x() + barInfo.rect.width() / 2 - m_barWidth / 2;
        int value = barInfo.value.isEmpty() ? 0 : (barInfo.value.toFloat() * 100);

        int index = -1;
        for(int i = 0; i < (m_ordinateValues.size() - 1); i++)
        {
            if(value >= m_ordinateValues.at(i).toFloat() && value < m_ordinateValues.at(i+1).toFloat())
            {
                index = i;
                break;
            }
        }
        if(index == -1)
        {
            index = m_ordinateValues.size() - 1;
        }

        QRect histogramRect;
        if(value > 60)
        {
            QRect rect = m_ordinateScaleRectList.at(index);
            int minVaiue = m_ordinateValues.at(index).toFloat();
            int surplus = value-minVaiue;
            double ratio = (double)rect.height() / 5.00;
            int startY = rect.height() * index + (ratio * surplus);
            histogramRect = QRect(QPoint(startX, m_axisRect.bottomLeft().y() - startY), QPoint(startX + m_barWidth, m_axisRect.bottomLeft().y()-2));

        }
        else
        {
            QRect rect = m_ordinateScaleRectList.at(index);
            double ratio = (double)rect.height() / 60.00;
            int startY = ratio * value;
            histogramRect = QRect(QPoint(startX, m_axisRect.bottomLeft().y() - startY), QPoint(startX + m_barWidth, m_axisRect.bottomLeft().y()-2));
        }

        //bar
        painter.save();
        painter.setPen(QColor(Qt::transparent));
        QLinearGradient Linear(histogramRect.x(),histogramRect.y(),histogramRect.bottomRight().x(),histogramRect.bottomRight().y());        //垂直渐变
        if(i==m_barList.size()-1)
        {
            //使用qRgba无法透明
            Linear.setColorAt(0,QColor(242, 199, 58,220));
            Linear.setColorAt(1,QColor(48, 56, 50,100));
        }
        else
        {
            Linear.setColorAt(0,QColor(233, 238, 250,220));
            Linear.setColorAt(1,QColor(19, 39, 58,100));
        }
        painter.setBrush(Linear);
        painter.drawRect(histogramRect);

        QRect transverseRect(startX - 5, histogramRect.y(), histogramRect.width() + 10, 2);
        painter.drawRect(transverseRect);
        painter.restore();

        //百分比
        painter.save();
        QFont font;
        font.setPixelSize(12);
        painter.setFont(font);
        if(i==m_barList.size()-1)
        {
            painter.setPen(QColor(qRgb(242, 199, 58)));
        }
        else
        {
            painter.setPen(QColor(qRgb(233, 238, 250)));
        }
        painter.setRenderHint(QPainter::Antialiasing);
        QRect percentRect(startX - 5, histogramRect.y() - 15, histogramRect.width() + 10, 15);
        //painter.drawRect(percentRect);
        painter.drawText(percentRect, Qt::AlignCenter, QString("%1%").arg(value));
        painter.restore();


    }
}
