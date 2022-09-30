#ifndef SECTORBARCHART_H
#define SECTORBARCHART_H

#include <QWidget>

class SectorBarChart : public QWidget
{
    Q_OBJECT
public:
    struct BarInfo
    {
        BarInfo()
            :name("")
            ,value("0.00")
            ,rect(QRect())
        {

        }

        QString name;
        QString value;
        QRect rect;
    };

public:
    explicit SectorBarChart(QWidget *parent = nullptr);
    void setBarChartDatas(QList<BarInfo> option);
    void initBarChart();

private:
    void resizeEvent(QResizeEvent *event) override;
signals:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawAbscissa(QPainter& painter);
    void drawOrdinate(QPainter& painter);
    void drawBar(QPainter& painter);

private:
    const int m_barWidth;
    QColor m_axisColor;
    int m_abscissaSpacing;
    int m_ordinateSpacing;
    QRect m_axisRect;
    int m_barOffset;
    QList<BarInfo> m_barList;
    QList<QString> m_ordinateValues;
    QList<QRect>   m_ordinateScaleRectList;
};

#endif // SECTORBARCHART_H
