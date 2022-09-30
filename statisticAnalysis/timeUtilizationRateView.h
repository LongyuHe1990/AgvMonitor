#ifndef UTILIZATIONRATEVIEW_H
#define UTILIZATIONRATEVIEW_H

#include <QWidget>

class TimeUtilizationRateView : public QWidget
{
    Q_OBJECT
public:
    explicit TimeUtilizationRateView(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // UTILIZATIONRATEVIEW_H
