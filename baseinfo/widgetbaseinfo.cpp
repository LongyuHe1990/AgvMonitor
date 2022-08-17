#include "widgetbaseinfo.h"
#include "ui_widgetbaseinfo.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"
#include <QDebug>
#include "common/global_config.h"

static WidgetBaseInfo* widget_baseinfo_ = nullptr;

WidgetBaseInfo::WidgetBaseInfo(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetBaseInfo)
  , base_info_()
{
  ui->setupUi(this);

  Initialize();
  TranslateLanguage();

  widget_baseinfo_ = this;
  ui->pushButton->setHidden(true);
  ui->pushButton_2->setHidden(true);
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
  base_info_.futureNodeId    = content.value("futureNodeId").toInt();
  base_info_.walkStatus      = content.value("walkStatus").toInt();
  base_info_.workStatus      = content.value("workStatus").toInt();
  base_info_.targetStationId = content.value("targetStationId").toInt();
  base_info_.id              = content.value("id").toInt();

  QVariantMap batteryState = content.value("batteryState").toMap();
  if(batteryState.isEmpty())
  {
    return;
  }
  base_info_.energy     = batteryState.value("energy").toInt();
  base_info_.current    = batteryState.value("current").toDouble() / 10.0;
  base_info_.voltage    = batteryState.value("voltage").toDouble() / 10.0;
  base_info_.isCharging = batteryState.value("isCharging").toBool();

  ShowBaseInfo();
}

void WidgetBaseInfo::VisitorModel(bool model)
{
  ui->pushButton->setHidden(model);
  ui->pushButton_2->setHidden(model);
}

void WidgetBaseInfo::ShowBaseInfo()
{
  QString agvName = ConfigModule::getInstance()->getAgvName(base_info_.id);
  ui->label_7->setText(agvName);
  StatusAndColorInfo info = GetWorkStatusInfo(base_info_.workStatus);
  ui->label_8->setText(info.status + "-" + GetWalkStatus(base_info_.walkStatus));
  ui->label_8->setStyleSheet(QString("QLabel{background-color: rgb(255, 255, 255, 0);color: %1;border:none;}").arg(info.color));

  ui->label_3->setText(QString::number(base_info_.futureNodeId));
  QString stationName = StationModule::getInstance()->getStationName(base_info_.targetStationId);

  ui->label_5->setText(stationName.isEmpty() ? "无" : stationName);
  ui->label_24->setText(tr(base_info_.isCharging ? "是" : "否"));
  ui->label_17->setText(QString::number(base_info_.voltage));
  ui->label_9->setText(QString::number(base_info_.current));
  ui->label_13->setText(tr("%1%").arg(base_info_.energy));

  if(base_info_.energy <= 10)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/10.png);color:rgb(255, 58, 114);}");
  }
  else if(base_info_.energy > 10 | base_info_.energy <= 20)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/20.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 20 | base_info_.energy <= 30)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/30.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 30 | base_info_.energy <= 40)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/40.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 40 | base_info_.energy <= 50)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/50.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 50 | base_info_.energy <= 60)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/60.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 60 | base_info_.energy <= 70)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/70.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 70 | base_info_.energy <= 80)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/80.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 80 | base_info_.energy <= 90)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/90.png);color:rgb(43, 235, 132);}");
  }
  else if(base_info_.energy > 90 & base_info_.energy <= 100)
  {
    ui->label_13->setStyleSheet("QLabel{border-image:url(:/image/home/100.png);color:rgb(43, 235, 132);}");
  }
}

void WidgetBaseInfo::Initialize()
{
  QFont font = ui->label_7->font();
  font.setPixelSize(16);
  ui->label_7->setFont(font);

  font.setPixelSize(14);
  ui->label_8->setFont(font);
  ui->label_24->setFont(font);
  ui->label_17->setFont(font);
  ui->label_9->setFont(font);
  ui->label_13->setFont(font);

  font.setPixelSize(12);
  ui->label_3->setFont(font);
  ui->label_5->setFont(font);
  ui->label_16->setFont(font);
  ui->label_18->setFont(font);
  ui->label->setFont(font);
  ui->pushButton->setFont(font);
  ui->pushButton_2->setFont(font);

  ui->label_6->setFixedSize(QSize(14, 14));
  ui->label_13->setFixedSize(QSize(80, 84));
  ui->pushButton->setFixedSize(QSize(80, 30));
  ui->pushButton_2->setFixedSize(QSize(80, 30));
  ui->label_2->setFixedSize(QSize(11, 11));
  ui->label_4->setFixedSize(QSize(11, 11));
}

void WidgetBaseInfo::TranslateLanguage()
{
  ui->pushButton->setText(tr("登出"));
  ui->pushButton_2->setText(tr("暂停"));
  ui->label_16->setText(tr("充电状态"));
  ui->label_18->setText(tr("电池电压(V)"));
  ui->label->setText(tr("电池电流(A)"));
  ui->label_8->setText(tr("离线"));
}
