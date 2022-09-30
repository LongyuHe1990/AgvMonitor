#ifndef PERFORMANCEUTILIZATIONRATEVIEW_H
#define PERFORMANCEUTILIZATIONRATEVIEW_H

#include <QWidget>

class PerformanceUtilizationRateView : public QWidget
{
    Q_OBJECT
public:
    explicit PerformanceUtilizationRateView(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
signals:

};

#endif // PERFORMANCEUTILIZATIONRATEVIEW_H
