#include "performanceUtilizationRateView.h"
#include "common/customWidget/sectorBarChart.h"
#include <QVBoxLayout>
#include <QPainter>

PerformanceUtilizationRateView::PerformanceUtilizationRateView(QWidget *parent) : QWidget(parent)
{
    setMaximumHeight(300);

    SectorBarChart *bar = new SectorBarChart();

    QList<SectorBarChart::BarInfo> optionTypeList;

    SectorBarChart::BarInfo optionA;
    optionA.name = "9/11";
    optionA.value = QString::number(0.82);
    optionTypeList.append(optionA);

    SectorBarChart::BarInfo optionB;
    optionB.name = "9/12";
    optionB.value = QString::number(0.88);
    optionTypeList.append(optionB);

    SectorBarChart::BarInfo optionC;
    optionC.name = "9/13";
    optionC.value = QString::number(0.92);
    optionTypeList.append(optionC);

    SectorBarChart::BarInfo optionD;
    optionD.name = "9/14";
    optionD.value = QString::number(0.95);
    optionTypeList.append(optionD);

    SectorBarChart::BarInfo optionE;
    optionE.name = "9/15";
    optionE.value = QString::number(0.85);
    optionTypeList.append(optionE);

    SectorBarChart::BarInfo optionF;
    optionF.name = "9/16";
    optionF.value = QString::number(0.52);
    optionTypeList.append(optionF);

    SectorBarChart::BarInfo optionG;
    optionG.name = "9/17";
    optionG.value = QString::number(0.60);
    optionTypeList.append(optionG);

    bar->setBarChartDatas(optionTypeList);
    bar->show();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->addSpacing(40);
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(bar);
    setLayout(layout);
}

void PerformanceUtilizationRateView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    painter.setPen(QPen(QColor(113, 149, 183),1));
    //painter.setBrush(Qt::blue);
    painter.drawRect(0,0,geometry().width(),geometry().height());
    painter.restore();

    painter.save();
    painter.setPen(Qt::transparent);
    QRect rect1(1, 1, 10, 40);
    QRect rect2(rect1.topRight().x(), rect1.topRight().y(), geometry().width()-rect1.width() - 1, rect1.bottomRight().y());
    painter.setBrush(QColor(200, 172, 72));
    painter.drawRect(rect1);
    QLinearGradient linear(rect2.topLeft(), rect2.bottomRight());
    linear.setColorAt(0, QColor(69, 86, 107));
    linear.setColorAt(1, QColor(37, 51, 64));
    painter.setBrush(linear);
    painter.drawRect(rect2);
    painter.restore();

    painter.save();
    painter.setPen(QColor(Qt::white));
    painter.setFont(QFont("微软雅黑", 10));
    painter.drawText(QRect(rect2.x() + 10, rect2.y(), rect2.width() - 50, rect2.height()), Qt::AlignVCenter | Qt::AlignLeft, tr("性能稼动率"));
    painter.restore();

    painter.save();
    QRect rect3(rect2.topRight().x() - 30, rect2.topRight().y() + rect2.height() / 2 - 2, 4, 4);
    painter.setBrush(QColor(255, 214, 61));
    painter.setPen(Qt::transparent);
    painter.drawRect(rect3);
    painter.restore();


    //QWidget::paintEvent(event);
}
