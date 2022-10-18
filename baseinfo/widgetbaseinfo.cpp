#include "widgetbaseinfo.h"
#include "ui_widgetbaseinfo.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"
#include <QDebug>
#include "common/global_config.h"
#include "common/widgetmessagebox.h"
#include "common/tiplabel.h"

static WidgetBaseInfo* widget_baseinfo_ = nullptr;

WidgetBaseInfo::WidgetBaseInfo(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetBaseInfo)
{
  ui->setupUi(this);

  Initialize();
  TranslateLanguage();

  widget_baseinfo_ = this;

  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LogoutButtonClicked()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(PauseButtonClicekd()));
}

WidgetBaseInfo::~WidgetBaseInfo()
{
  delete ui;
}

WidgetBaseInfo * WidgetBaseInfo::GetInstance()
{
  return widget_baseinfo_;
}

void WidgetBaseInfo::InitData(QVariantMap data_map)
{
  if(data_map.isEmpty())
  {
    return;
  }

  QVariantMap module = data_map.value("ModuleData").toMap();
  if(module.isEmpty())
  {
    return;
  }

  int operation = module.value("OperationType").toInt();
  if(static_cast<AgvOperationType>(operation) != AgvOperationType::AGV_STATUS_UPDATED)
  {
    return;
  }

  QVariantMap content = module.value("Content").toMap();
  if(content.isEmpty())
  {
    return;
  }

  if(UserConfigs::AgvId != content.value("id").toInt())
  {
    return;
  }
  int futureNodeId    = content.value("futureNodeId").toInt();
  int targetStationId = content.value("targetStationId").toInt();
  int id              = content.value("id").toInt();
  int walkStatus      = content.value("walkStatus").toInt();
  int workStatus      = content.value("workStatus").toInt();

  QString agvName = ConfigModule::getInstance()->getAgvName(id);
  ui->label_7->setText(agvName);
  StatusAndColorInfo info = GetWorkStatusInfo(workStatus);
  ui->label_8->setText(info.status);
  ui->label_8->setStyleSheet(QString("QLabel{background-color: rgb(255, 255, 255, 0);color: %1;border:none;}").arg(info.color));
  ui->label_26->setText(info.status);
  ui->label_26->setStyleSheet(QString("QLabel{background-color: rgb(255, 255, 255, 0);color: %1;border:none;}").arg(info.color));
  ui->label_12->setText(GetWalkStatus(walkStatus));
  ui->label_28->setText(GetWalkStatus(walkStatus));
  ui->label->setText(QString::number(futureNodeId));
  QString stationName = StationModule::getInstance()->getStationName(targetStationId);
  ui->label_3->setText(stationName.isEmpty() ? "无" : stationName);

  QVariantMap batteryState = content.value("batteryState").toMap();
  if(batteryState.isEmpty())
  {
    return;
  }
  int    energy     = batteryState.value("energy").toInt();
  double current    = batteryState.value("current").toDouble() / 10.0;
  double voltage    = batteryState.value("voltage").toDouble() / 10.0;
  bool   isCharging = batteryState.value("isCharging").toBool();

  ui->label_24->setText(isCharging ? tr("Yes") : tr("No"));
  ui->label_17->setText(QString::number(voltage));
  ui->label_19->setText(QString::number(current));
  ui->label_13->setText(tr("%1%").arg(energy));

  if(energy < 5)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_0.png);color:rgb(255, 58, 114);}");
  }
  else if(energy >= 5 && energy < 10)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_5.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 10 && energy < 15)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_10.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 15 && energy < 20)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_15.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 20 && energy < 25)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_20.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 25 && energy < 30)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/dianliang_25.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 30 && energy < 35)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_30.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 35 && energy < 40)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_35.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 40 && energy < 45)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_40.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 45 && energy < 50)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_45.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 50 && energy < 55)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_50.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 55 && energy < 60)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_55.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 60 && energy < 65)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_60.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 65 && energy < 70)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_65.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 70 && energy < 75)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_70.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 75 && energy < 80)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_75.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 80 && energy < 85)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_80.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 85 && energy < 90)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_85.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 90 && energy < 95)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_90.png);color:rgb(43, 235, 132);}");
  }
  else if(energy >= 95 && energy < 100)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_95.png);color:rgb(43, 235, 132);}");
  }
  else if(energy == 100)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home_all/dianliang_100.png);color:rgb(43, 235, 132);}");
  }


  bool isAuto   = content.value("isAuto").toBool();
  int  speed    = content.value("speed").toInt();
  bool isEdged  = content.value("isEdged").toBool();
  bool isScram  = content.value("isScram").toBool();
  int  isAvoid  = content.value("isAvoid").toBool();
  bool isNormal = content.value("isNormal").toBool();

  ui->label_21->setText(isAuto ? tr("Automatic") : tr("Manual"));
  ui->label_23->setText(isScram ? tr("Trigger") : tr("Normal"));
  ui->label_30->setText(QString::number(speed));
  ui->label_32->setText(isNormal ? tr("Normal") : tr("Abnormal"));
  ui->label_34->setText(isAvoid ? tr("Obstacle avoidance") : tr("Normal"));
  ui->label_36->setText(isEdged ? tr("Trigger") : tr("Normal"));
}

void WidgetBaseInfo::VisitorModel(bool model)
{
  ui->pushButton->setHidden(model);
  ui->pushButton_2->setHidden(model);
}

void WidgetBaseInfo::LogoutButtonClicked()
{
  if(WidgetMessageBox().MessageHint(tr("Sign out confirmation"), tr("Are you sure you want to log out of the robot")))
  {
     TipLabel::Popup(tr("Login succeeded"));
  }
}

void WidgetBaseInfo::PauseButtonClicekd()
{

}

void WidgetBaseInfo::Initialize()
{
  QFont font = ui->label_7->font();
  font.setPixelSize(16);
  ui->label_7->setFont(font);

  font.setPixelSize(14);
  ui->label_24->setFont(font);
  ui->label_17->setFont(font);
  ui->label_19->setFont(font);
  ui->label_13->setFont(font);

  font.setPixelSize(12);
  ui->label_3->setFont(font);
  ui->label_16->setFont(font);
  ui->label_18->setFont(font);
  ui->label->setFont(font);
  ui->label_8->setFont(font);
  ui->label_12->setFont(font);
  ui->label_20->setFont(font);
  ui->label_21->setFont(font);
  ui->label_22->setFont(font);
  ui->label_23->setFont(font);
  ui->label_25->setFont(font);
  ui->label_26->setFont(font);
  ui->label_27->setFont(font);
  ui->label_28->setFont(font);
  ui->label_29->setFont(font);
  ui->label_30->setFont(font);
  ui->label_31->setFont(font);
  ui->label_32->setFont(font);
  ui->label_33->setFont(font);
  ui->label_34->setFont(font);
  ui->label_35->setFont(font);
  ui->label_36->setFont(font);

  ui->pushButton->setFont(font);
  ui->pushButton_2->setFont(font);

  font.setPixelSize(10);
  ui->label_11->setFont(font);
  ui->label_9->setFont(font);
  ui->label_16->setFont(font);
  ui->label_15->setFont(font);
  ui->label_18->setFont(font);

  ui->label_6->setFixedSize(QSize(14, 14));
  ui->label_13->setFixedSize(QSize(85, 78));
  ui->pushButton->setFixedSize(QSize(93, 30));
  ui->pushButton_2->setFixedSize(QSize(93, 30));
  ui->label_2->setFixedSize(QSize(11, 11));
  ui->label_4->setFixedSize(QSize(11, 11));
  ui->label_14->setFixedSize(QSize(11, 11));
}

void WidgetBaseInfo::TranslateLanguage()
{
  ui->label_10->setText(tr("Basic information"));
  ui->label_11->setText(tr("Running state"));
  ui->label_9->setText(tr("Machine position"));
  ui->pushButton->setText(tr("Logout"));
  ui->pushButton_2->setText(tr("Pause"));
  ui->label_16->setText(tr("Charging state"));
  ui->label_15->setText(tr("Battery voltage(V)"));
  ui->label_18->setText(tr("Battery current(A)"));
  ui->label_8->setText(tr("Offline"));
  ui->label_20->setText(tr("AGV mode"));
  ui->label_22->setText(tr("Emergency stop status"));
  ui->label_25->setText(tr("AGV working state"));
  ui->label_27->setText(tr("AGV travel status"));
  ui->label_29->setText(tr("AGV speed"));
  ui->label_31->setText(tr("Current state of laser"));
  ui->label_33->setText(tr("Obstacle avoidance state"));
  ui->label_35->setText(tr("Contact state"));
}
