#include "widgetallinfo.h"
#include "ui_widgetallinfo.h"
#include <QScrollBar>
#include <QListView>
#include <QStyledItemDelegate>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QAbstractItemView>
#include <QDateTime>
#include "customData.h"
#include "moduleData/configModule.h"
#include "common/global_config.h"

static WidgetAllInfo* widget_allinfo_ = nullptr;

WidgetAllInfo::WidgetAllInfo(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetAllInfo)
  , body_state_()
  , agv_info_()
  , config_info_()
  , device_info_()
{
  ui->setupUi(this);

  Initialize();
  TranslateLanguage();

  widget_allinfo_ = this;
  ui->stackedWidget->setCurrentIndex(0);
  ui->widget_3->hide();
  connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(CurrentIndexChanged(int)));

  QStyledItemDelegate* delegate = new QStyledItemDelegate(this);
  ui->comboBox->setItemDelegate(delegate);
  ui->comboBox->setStyleSheet(styleSheet() +
                              QString(
                                "\
     QComboBox{ color: rgb(0, 0, 0); background-color: rgb(255, 226, 96); border:none; combobox-popup: 0;border-radius:4px;} \
     QComboBox::hover{color: rgb(0, 0, 0); background-color: rgb(255, 226, 96); } \
     QComboBox::drop-down{ width:10; subcontrol-origin:padding; subcontrol-position:top right; border-left-width:0px; border-left-color:darkgray; border-left-style:solid; padding-right:10px; } \
     QComboBox::down-arrow{ border-image:url(:/image/home/xiala_icon.png); width:0px; height:0px; }\
     QListView::item{padding-left:5px;color: rgb(0,0,0); background-color: rgb(255, 226, 96);}\
     QListView::item:hover{ color: rgb(0,0,0); background-color: rgb(29, 142, 192);}\
     QComboBox QAbstractItemView {border:0px solid #dddddd;outline:0px;height:30px;} \
     QComboBox QAbstractItemView::item {min-height: 30px;background-color: rgb(255, 226, 96);color:rgb(0,0,0);padding-left:11px;padding-right:11px;outline:1px solid rgb(216,215,211);} \
     QComboBox QAbstractItemView::item:hover {background-color: rgb(232, 205, 83);color:rgb(0,0,0);}"));
}

WidgetAllInfo::~WidgetAllInfo()
{
  delete ui;
}

WidgetAllInfo * WidgetAllInfo::GetInstance()
{
  return widget_allinfo_;
}

void WidgetAllInfo::InitData(QVariantMap dataMap)
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

  // 车体
  body_state_.speed             = content.value("speed").toInt();
  body_state_.isEdged           = content.value("isEdged").toBool();
  body_state_.isScram           = content.value("isScram").toBool();
  body_state_.walkStatus        = content.value("walkStatus").toInt();
  body_state_.workStatus        = content.value("workStatus").toInt();
  body_state_.executeStatus     = content.value("executeStatus").toInt();
  body_state_.mapConfigStatus   = content.value("mapConfigStatus").toInt();
  body_state_.paramConfigStatus = content.value("paramConfigStatus").toInt();
  body_state_.isAvoid           = content.value("isAvoid").toBool();
  // 车体
  agv_info_.latestCmdId  = content.value("latestCmdId").toInt();
  agv_info_.currentCmdId = content.value("currentCmdId").toInt();
  agv_info_.taskState    = content.value("taskState").toInt();
  agv_info_.isNormal     = content.value("isNormal").toBool();
  agv_info_.isAuto       = content.value("isAuto").toBool();
  agv_info_.angle        = content.value("angle").toInt() / 100;
  agv_info_.confidence   = content.value("confidence").toInt();

  // 车体配置
  config_info_.mapConfigStatus   = content.value("mapConfigStatus").toInt();
  config_info_.latestMapUuid     = content.value("latestMapUuid").toString();
  config_info_.latestMapSize     = content.value("latestMapSize").toInt();
  config_info_.updatedMapUuid    = content.value("updatedMapUuid").toString();
  config_info_.updatedMapSize    = content.value("updatedMapSize").toInt();
  config_info_.currentMapUuid    = content.value("currentMapUuid").toString();
  config_info_.currentMapSize    = content.value("currentMapSize").toInt();
  config_info_.paramConfigStatus = content.value("paramConfigStatus").toInt();
  config_info_.latestParamUuid   = content.value("latestParamUuid").toString();
  config_info_.updatedParamUuid  = content.value("updatedParamUuid").toString();
  config_info_.currentParamUuid  = content.value("currentParamUuid").toString();

  // 车体设备
  device_info_.lastestExecuteStatus = content.value("lastestExecuteStatus").toInt();
  device_info_.expectMaterialNum    = content.value("expectMaterialNum").toInt();
  device_info_.currentExecuteStatus = content.value("currentExecuteStatus").toInt();

  if(content.find("currentTransData") != content.end())
  {
    QVariantMap currentTransData = content.value("currentTransData").toMap();
    device_info_.cmdId_system        = currentTransData.value("cmdId").toInt();
    device_info_.action_system       = currentTransData.value("action").toInt();
    device_info_.step_system         = currentTransData.value("step").toInt();
    device_info_.materialNum_system  = currentTransData.value("materialNum").toInt();
    device_info_.materialType_system = currentTransData.value("materialType").toInt();
  }

  if(content.find("issueTransData") != content.end())
  {
    QVariantMap issueTransData = content.value("issueTransData").toMap();
    device_info_.cmdId_device        = issueTransData.value("cmdId").toInt();
    device_info_.action_device       = issueTransData.value("action").toInt();
    device_info_.step_device         = issueTransData.value("step").toInt();
    device_info_.materialNum_device  = issueTransData.value("materialNum").toInt();
    device_info_.materialType_device = issueTransData.value("materialType").toInt();
  }

  ShowAgvBodyStateInfo();
  ShowAgvInfo();
  ShowAgvConfigInfo();
  ShowAgvDeviceInfo();
}

void WidgetAllInfo::CurrentIndexChanged(int index)
{
  ui->stackedWidget->setCurrentIndex(index);
}

void WidgetAllInfo::ShowAgvBodyStateInfo()
{
  StatusAndColorInfo info;
  info = GetWorkStatusInfo(body_state_.workStatus);
  ui->label_3->setText(info.status);
  ui->label_5->setText(GetWalkStatus(body_state_.walkStatus));
  info = GetConfigStatus(body_state_.mapConfigStatus);
  ui->label_6->setText(info.status);
  info = GetConfigStatus(body_state_.paramConfigStatus);
  ui->label_9->setText(info.status);
  info = GetExecuteStatusInfo(body_state_.executeStatus);
  ui->label_11->setText(info.status);
// ui->label_13->setText(body_state_.isAvoid ? "避障" : "正常");
  ui->label_13->setText(body_state_.isScram ? "触发" : "正常");
  ui->label_15->setText(body_state_.isEdged ? "触发" : "正常");
  ui->label_17->setText(QString::number(body_state_.speed));
}

void WidgetAllInfo::ShowAgvInfo()
{
  StatusAndColorInfo info;
  info = GetWorkStatusInfo(body_state_.workStatus);
  ui->label_38->setText(info.status);
  ui->label_24->setText(QString::number(agv_info_.latestCmdId));
  ui->label_32->setText(QString::number(agv_info_.currentCmdId));
  ui->label_40->setText(GetAgvHoverState(agv_info_.taskState));
  ui->label_26->setText(agv_info_.isNormal ? "正常" : "异常");
  ui->label_34->setText(agv_info_.isAuto ? "自动" : "手动");
  ui->label_36->setText(QString::number(agv_info_.angle));
  ui->label_30->setText(QString::number(agv_info_.confidence));
  ui->label_42->setText(QString::number(body_state_.speed));
}

void WidgetAllInfo::ShowAgvConfigInfo()
{
  StatusAndColorInfo info;
  info = GetConfigStatus(config_info_.mapConfigStatus);
  ui->label_58->setText(info.status);
  ui->label_44->setText(config_info_.latestMapUuid.isEmpty() ? "无" : config_info_.latestMapUuid);
  ui->label_52->setText(QString::number(config_info_.latestMapSize));
  ui->label_60->setText(config_info_.updatedMapUuid.isEmpty() ? "无" : config_info_.updatedMapUuid);
  ui->label_46->setText(QString::number(config_info_.updatedMapSize));
  ui->label_54->setText(config_info_.currentMapUuid.isEmpty() ? "无" : config_info_.currentMapUuid);
  ui->label_56->setText(QString::number(config_info_.currentMapSize));
  info = GetConfigStatus(config_info_.paramConfigStatus);
  ui->label_50->setText(info.status);
  ui->label_62->setText(config_info_.latestParamUuid.isEmpty() ? "无" : config_info_.latestParamUuid);
  ui->label_48->setText(config_info_.updatedParamUuid.isEmpty() ? "无" : config_info_.updatedParamUuid);
  ui->label_83->setText(config_info_.currentParamUuid.isEmpty() ? "无" : config_info_.currentParamUuid);

  ui->label_58->setToolTip(ui->label_58->text());
  ui->label_44->setToolTip(ui->label_44->text());
  ui->label_52->setToolTip(ui->label_52->text());
  ui->label_60->setToolTip(ui->label_60->text());
  ui->label_46->setToolTip(ui->label_46->text());
  ui->label_54->setToolTip(ui->label_54->text());
  ui->label_56->setToolTip(ui->label_56->text());
  ui->label_50->setToolTip(ui->label_50->text());
  ui->label_62->setToolTip(ui->label_62->text());
  ui->label_48->setToolTip(ui->label_48->text());
  ui->label_83->setToolTip(ui->label_83->text());
}

void WidgetAllInfo::ShowAgvDeviceInfo()
{
  StatusAndColorInfo info;
  info = GetWorkStatusInfo(device_info_.lastestExecuteStatus);
  ui->label_78->setText(info.status.isEmpty() ? "无" : info.status);
  ui->label_64->setText(QString::number(device_info_.expectMaterialNum));
  info = GetWorkStatusInfo(device_info_.currentExecuteStatus);
  ui->label_72->setText(info.status.isEmpty() ? "无" : info.status);
  ui->label_80->setText(QString::number(device_info_.cmdId_system));
  NameAndLabelInfo info_label;
  info_label = GetActionType(device_info_.action_system);
  ui->label_66->setText(info_label.name);
  ui->label_74->setText(QString::number(device_info_.step_system));
  ui->label_76->setText(QString::number(device_info_.materialNum_system));
  ui->label_70->setText(QString::number(device_info_.materialType_system));
  ui->label_82->setText(QString::number(device_info_.cmdId_device));
  info_label = GetActionType(device_info_.action_device);
  ui->label_68->setText(info_label.name);
  ui->label_27->setText(QString::number(device_info_.step_device));
  ui->label_85->setText(QString::number(device_info_.materialNum_device));
  ui->label_87->setText(QString::number(device_info_.materialType_device));
}

void WidgetAllInfo::Initialize()
{
  ui->comboBox->setFixedSize(QSize(184, 30));

  QFont font = ui->label->font();
  font.setPixelSize(12);
  ui->label->setFont(font);
  ui->label_2->setFont(font);

  font.setPixelSize(14);
  ui->comboBox->setFont(font);
  // 车体状态
  font.setPixelSize(10);
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

  // 车体
  ui->label_23->setFont(font);
  ui->label_24->setFont(font);
  ui->label_25->setFont(font);
  ui->label_26->setFont(font);
  ui->label_29->setFont(font);
  ui->label_30->setFont(font);
  ui->label_31->setFont(font);
  ui->label_32->setFont(font);
  ui->label_33->setFont(font);
  ui->label_34->setFont(font);
  ui->label_35->setFont(font);
  ui->label_36->setFont(font);
  ui->label_37->setFont(font);
  ui->label_38->setFont(font);
  ui->label_39->setFont(font);
  ui->label_40->setFont(font);
  ui->label_41->setFont(font);
  ui->label_42->setFont(font);

  // 车体配置
  ui->label_43->setFont(font);
  ui->label_44->setFont(font);
  ui->label_45->setFont(font);
  ui->label_46->setFont(font);
  ui->label_47->setFont(font);
  ui->label_48->setFont(font);
  ui->label_49->setFont(font);
  ui->label_50->setFont(font);
  ui->label_51->setFont(font);
  ui->label_52->setFont(font);
  ui->label_53->setFont(font);
  ui->label_54->setFont(font);
  ui->label_55->setFont(font);
  ui->label_56->setFont(font);
  ui->label_57->setFont(font);
  ui->label_58->setFont(font);
  ui->label_59->setFont(font);
  ui->label_60->setFont(font);
  ui->label_61->setFont(font);
  ui->label_62->setFont(font);
  ui->label_83->setFont(font);
  ui->label_84->setFont(font);

  // 车体设备
  ui->label_63->setFont(font);
  ui->label_64->setFont(font);
  ui->label_65->setFont(font);
  ui->label_66->setFont(font);
  ui->label_67->setFont(font);
  ui->label_68->setFont(font);
  ui->label_69->setFont(font);
  ui->label_70->setFont(font);
  ui->label_71->setFont(font);
  ui->label_72->setFont(font);
  ui->label_73->setFont(font);
  ui->label_74->setFont(font);
  ui->label_75->setFont(font);
  ui->label_76->setFont(font);
  ui->label_77->setFont(font);
  ui->label_78->setFont(font);
  ui->label_79->setFont(font);
  ui->label_80->setFont(font);
  ui->label_81->setFont(font);
  ui->label_82->setFont(font);
  ui->label_87->setFont(font);
  ui->label_88->setFont(font);
  ui->label_86->setFont(font);
  ui->label_85->setFont(font);
  ui->label_27->setFont(font);
  ui->label_28->setFont(font);
}

void WidgetAllInfo::TranslateLanguage()
{
  ui->comboBox->addItem(tr("车体状态"));
  ui->comboBox->addItem(tr("车体"));
  ui->comboBox->addItem(tr("车体配置"));
// ui->comboBox->addItem(tr("车体设备(叉齿)"));

  ui->label->setText(tr("属性"));
  ui->label_2->setText(tr("值"));

  // 车体状态
  ui->label_4->setText(tr("AGV工作状态"));
  ui->label_7->setText(tr("AGV行走状态"));
  ui->label_8->setText(tr("地图当前状态"));
  ui->label_10->setText(tr("参数当前状态"));
  ui->label_12->setText(tr("命令执行状态"));
// ui->label_14->setText(tr("激光当前状态"));
  ui->label_14->setText(tr("急停状态"));
  ui->label_16->setText(tr("触边状态"));
  ui->label_18->setText(tr("AGV速度"));

  // 车体
  ui->label_37->setText(tr("命令执行状态"));
  ui->label_23->setText(tr("最新命令号"));
  ui->label_31->setText(tr("当前命令号"));
  ui->label_39->setText(tr("任务状态(车体)"));
  ui->label_25->setText(tr("异常状态"));
  ui->label_33->setText(tr("手自动状态"));
  ui->label_35->setText(tr("角度"));
  ui->label_29->setText(tr("置信度"));
  ui->label_41->setText(tr("AGV速度"));

  // 车体配置
  ui->label_57->setText(tr("地图当前状态"));
  ui->label_43->setText(tr("最新地图版本(系统)"));
  ui->label_51->setText(tr("最新地图大小(系统)"));
  ui->label_59->setText(tr("最新地图版本(车体)"));
  ui->label_45->setText(tr("最新地图大小(车体)"));
  ui->label_53->setText(tr("当前地图版本(车体)"));
  ui->label_55->setText(tr("当前地图大小(车体)"));
  ui->label_49->setText(tr("参数当前状态"));
  ui->label_61->setText(tr("最新参数版本(系统)"));
  ui->label_47->setText(tr("最新参数版本(车体)"));
  ui->label_84->setText(tr("当前参数版本(车体)"));

  // 车体设备(叉齿)
  ui->label_77->setText(tr("最终执行状态"));
  ui->label_63->setText(tr("期望物料数量"));
  ui->label_71->setText(tr("执行状态(设备上传)"));
  ui->label_79->setText(tr("命令号(设备上传)"));
  ui->label_65->setText(tr("动作(设备上传)"));
  ui->label_73->setText(tr("步骤(设备上传)"));
  ui->label_75->setText(tr("物料数量(设备上传)"));
  ui->label_69->setText(tr("物料类型(设备上传)"));
  ui->label_81->setText(tr("命令号(系统下发)"));
  ui->label_67->setText(tr("动作(系统下发)"));
  ui->label_28->setText(tr("步骤(系统下发)"));
  ui->label_86->setText(tr("物料数量(系统下发)"));
  ui->label_88->setText(tr("物料类型(系统下发)"));
}

void WidgetAllInfo::GetDeviceName(QVariantMap dataMap)
{
  if(dataMap.isEmpty())
  {
    return;
  }

  QVariantMap list = dataMap.value(QString::number(int(ConfigType::Agv))).toMap();
  if(list.isEmpty())
  {
    return;
  }
  QVariantMap config = list.value(QString("%1").arg(UserConfigs::AgvId)).toMap();

  if(config.empty())
  {
    return;
  }
  int agvTemplateId = config.value("agvTemplateId").toInt();

  QVariantMap device     = ConfigModule::getInstance()->getcurrentAgvTemplateLoadDevice(agvTemplateId);

  QVariantMap dataDevice = device.value(QString("%1").arg(agvTemplateId)).toMap();
  int         id         = dataDevice.value("id").toInt();
  QString     name       = dataDevice.value("name").toString();
  UserConfigs::AgvAxisId = id;
  UserConfigs::AgvAxisName = name;

  ui->comboBox->addItem(tr("车体设备(%1)").arg(name));
}

