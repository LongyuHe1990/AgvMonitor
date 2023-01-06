#include "widgetmanualcontrol.h"
#include "ui_widgetmanualcontrol.h"
#include "superstratumControl/superstratumControlView.h"

WidgetManualControl::WidgetManualControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetManualControl)
{
    ui->setupUi(this);

    agv_status = new AgvStatus(this);
    ui->verticalLayout->addWidget(agv_status);

    SuperstratumControlView* superstratumControlVie = new SuperstratumControlView(this);
    ui->verticalLayout_2->addWidget(superstratumControlVie);

}

WidgetManualControl::~WidgetManualControl()
{
    delete ui;
}
