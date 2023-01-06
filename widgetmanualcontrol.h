#ifndef WIDGETMANUALCONTROL_H
#define WIDGETMANUALCONTROL_H

#include <QWidget>
#include "superstratumControl/agvstatus.h"

namespace Ui {
class WidgetManualControl;
}

class WidgetManualControl : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetManualControl(QWidget *parent = nullptr);
    ~WidgetManualControl();

private:
    Ui::WidgetManualControl *ui;

    AgvStatus* agv_status;
};

#endif // WIDGETMANUALCONTROL_H
