#include "agvstatus.h"
#include "ui_agvstatus.h"
#include "customData.h"
#include "moduleData/configModule.h"
#include "common/global_config.h"
#include "common/global_helper.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

static AgvStatus* widget_agvStatus_ = nullptr;

AgvStatus::AgvStatus(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::AgvStatus)
{
  ui->setupUi(this);

  Initialize();
  TranslateLanguage();

  widget_agvStatus_ = this;
}

AgvStatus::~AgvStatus()
{
  delete ui;
}

AgvStatus * AgvStatus::GetInstance()
{
  return widget_agvStatus_;
}

void AgvStatus::InitData(QVariantMap dataMap)
{
  if(dataMap.isEmpty())
  {
    return;
  }

  QVariantMap module = dataMap.value("ModuleData").toMap();
  if(module.isEmpty())
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

  if(content.find("agvState") != content.end())
  {
    QVariantMap agvState = content.value("agvState").toMap();

    int                lastestExecuteStatus = agvState.value("lastestExecuteStatus").toInt();
    StatusAndColorInfo info;
    info = GetLastestExecuteStatus(lastestExecuteStatus);
    ui->label_33->setText(info.status.isEmpty() ? "无" : info.status);
    int expectMaterialNum = agvState.value("expectMaterialNum").toInt();
    ui->label_5->setText(QString::number(expectMaterialNum));
    int currentExecuteStatus = agvState.value("currentExecuteStatus").toInt();
    info = GetLastestExecuteStatus(currentExecuteStatus);
    ui->label_21->setText(info.status.isEmpty() ? "无" : info.status);
    // 系统下发
    if(agvState.find("issueTransData") != agvState.end())
    {
      QVariantMap currentTransData    = agvState.value("issueTransData").toMap();
      int         cmdId_system        = currentTransData.value("cmdId").toInt();
      int         action_system       = currentTransData.value("action").toInt();
      int         step_system         = currentTransData.value("step").toInt();
      int         materialNum_system  = currentTransData.value("materialNum").toInt();
      int         materialType_system = currentTransData.value("materialType").toInt();
      QString     materialId          = currentTransData.value("materialId").toString();
      ui->label_7->setText(QString::number(cmdId_system));
      NameAndLabelInfo info_label;
      info_label = GetActionType(action_system);
      ui->label_9->setText(info_label.name);
      ui->label_11->setText(QString::number(step_system));
      ui->label_13->setText(materialId.isEmpty() ? "(无)" : materialId);
      ui->label_15->setText(QString::number(materialNum_system));
      QVariantMap   type = ConfigModule::getInstance()->getConfig(ConfigType::MaterialType, materialType_system);
      ui->label_17->setText(type.isEmpty() ? "无" : type.value("name").toString());
    }
    // 设备上传
    if(agvState.find("currentTransData") != agvState.end())
    {
      QVariantMap issueTransData      = agvState.value("currentTransData").toMap();
      int         cmdId_device        = issueTransData.value("cmdId").toInt();
      int         action_device       = issueTransData.value("action").toInt();
      int         step_device         = issueTransData.value("step").toInt();
      int         materialNum_device  = issueTransData.value("materialNum").toInt();
      int         materialType_device = issueTransData.value("materialType").toInt();
      QString     materialId          = issueTransData.value("materialId").toString();
      ui->label_19->setText(QString::number(cmdId_device));
      NameAndLabelInfo info_label;
      info_label = GetActionType(action_device);
      ui->label_23->setText(info_label.name);
      ui->label_25->setText(QString::number(step_device));
      ui->label_27->setText(materialId.isEmpty() ? "(无)" : materialId);
      ui->label_29->setText(QString::number(materialNum_device));
      QVariantMap   type = ConfigModule::getInstance()->getConfig(ConfigType::MaterialType, materialType_device);
      ui->label_31->setText(type.isEmpty() ? "无" : type.value("name").toString());
    }
  }
}

void AgvStatus::Initialize()
{
  QFont font = ui->label->font();
  font.setPixelSize(24);
  ui->label->setFont(font);
  ui->label_2->setFont(font);
  ui->label_3->setFont(font);
  ui->label_4->setFont(font);
  ui->label_5->setFont(font);
  ui->label_6->setFont(font);
  ui->label_7->setFont(font);
  ui->label_8->setFont(font);
  ui->label_9->setFont(font);
  ui->label_10->setFont(font);
  ui->label_11->setFont(font);
  ui->label_12->setFont(font);
  ui->label_13->setFont(font);
  ui->label_14->setFont(font);
  ui->label_15->setFont(font);
  ui->label_16->setFont(font);
  ui->label_17->setFont(font);
  ui->label_18->setFont(font);
  ui->label_19->setFont(font);
  ui->label_20->setFont(font);
  ui->label_21->setFont(font);
  ui->label_22->setFont(font);
  ui->label_23->setFont(font);
  ui->label_24->setFont(font);
  ui->label_25->setFont(font);
  ui->label_26->setFont(font);
  ui->label_27->setFont(font);
  ui->label_28->setFont(font);
  ui->label_29->setFont(font);
  ui->label_30->setFont(font);
  ui->label_31->setFont(font);
  ui->label_32->setFont(font);
  ui->label_33->setFont(font);
}

void AgvStatus::TranslateLanguage()
{
  ui->label->setText(tr("Agv attribute"));                // agv属性
  ui->label_32->setText(tr("Final execution status"));    // 最终执行状态
  ui->label_2->setText(tr("Issued by the system"));       // 系统下发
  ui->label_4->setText(tr("Expected material quantity")); // 期望物料数量
  ui->label_6->setText(tr("Command number"));             // 命令号
  ui->label_8->setText(tr("Action"));                     // 动作
  ui->label_10->setText(tr("Step"));                      // 步骤
  ui->label_12->setText(tr("Item ID"));                   // 物料标识
  ui->label_14->setText(tr("Item quantity"));             // 物料数量
  ui->label_16->setText(tr("Item Type"));                 // 物料类型
  ui->label_3->setText(tr("Device upload"));              // 设备上传
  ui->label_18->setText(tr("Command number"));            // 命令号
  ui->label_20->setText(tr("Execution status"));          // 执行状态
  ui->label_22->setText(tr("Action"));                    // 动作
  ui->label_24->setText(tr("Step"));                      // 步骤
  ui->label_26->setText(tr("Item ID"));                   // 物料标识
  ui->label_28->setText(tr("Item quantity"));             // 物料数量
  ui->label_30->setText(tr("Item Type"));                 // 物料类型
}

void AgvStatus::changeEvent(QEvent* e)
{
  QWidget::changeEvent(e);
  switch(e->type())
  {
  case QEvent::LanguageChange:
    TranslateLanguage();
    break;
  default:
    break;
  }
}
