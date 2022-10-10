#include "widgetaddtask.h"
#include "ui_widgetaddtask.h"
#include <QListView>
#include <QStyledItemDelegate>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QAbstractItemView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"
#include "common/global_helper.h"
#include "common/global_config.h"

static WidgetAddTask* add_task_ = nullptr;

WidgetAddTask::WidgetAddTask(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetAddTask)
{
  ui->setupUi(this);

  add_task_ = this;

  Initialize();
  TranslateLanguage();

  ui->verticalLayout_3->setAlignment(Qt::AlignTop);
  ui->widget_4->setHidden(true);

  GetTargetListInfo();

  QString style =
    "\
          QComboBox{ color: rgb(255, 255, 255); background-color: rgb(64, 67, 76); border:none; combobox-popup: 0;border-radius:4px;} \
          QComboBox::hover{color: rgb(255, 255, 255); background-color: rgb(64, 67, 76); } \
          QComboBox::drop-down{ width:10; subcontrol-origin:padding; subcontrol-position:top right; border-left-width:0px; border-left-color:darkgray; border-left-style:solid; padding-right:10px; } \
          QComboBox::down-arrow{ border-image:url(:/image/home/xiala_icon.png); width:0px; height:0px; }\
          QListView::item{padding-left:5px;color: rgb(255,255,255); background-color: rgb(64, 67, 76);}\
          QListView::item:hover{ color: rgb(255,255,255); background-color: rgb(64, 67, 76);}\
          QComboBox QAbstractItemView {border:0px solid #dddddd;outline:0px;height:30px;} \
          QComboBox QAbstractItemView::item {min-height: 30px;background-color: rgb(64, 67, 76);color:rgb(255,255,255);padding-left:11px;padding-right:11px;outline:1px solid rgb(216,215,211);} \
          QComboBox QAbstractItemView::item:hover {background-color: rgb(64, 67, 76);color:rgb(255,255,255);}";
  QStyledItemDelegate* delegate = new QStyledItemDelegate(this);
  ui->comboBox_action->setItemDelegate(delegate);
  ui->comboBox_action->setStyleSheet(styleSheet() + style);
  ui->comboBox_agvFloor->setItemDelegate(delegate);
  ui->comboBox_agvFloor->setStyleSheet(styleSheet() + style);
  ui->comboBox_floor->setItemDelegate(delegate);
  ui->comboBox_floor->setStyleSheet(styleSheet() + style);
  ui->comboBox_target->setItemDelegate(delegate);
  ui->comboBox_target->setStyleSheet(styleSheet() + style);
  ui->comboBox_type->setItemDelegate(delegate);
  ui->comboBox_type->setStyleSheet(styleSheet() + style);
  ui->comboBox_device->setItemDelegate(delegate);
  ui->comboBox_device->setStyleSheet(styleSheet() + style);

  connect(ui->pushButton_close, SIGNAL(clicked()), this, SIGNAL(HideAddTaskListWidget()));
  connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(AddButtonClicked()));
  connect(ui->pushButton_create, SIGNAL(clicked()), this, SLOT(CreateButtonClicked()));

  connect(ui->comboBox_type, SIGNAL(currentIndexChanged(int)), this, SLOT(StationTypeChanged(int)));
  connect(ui->comboBox_agvFloor, SIGNAL(currentIndexChanged(int)), this, SLOT(StationRowChanged(int)));
}

WidgetAddTask::~WidgetAddTask()
{
  delete ui;
}

WidgetAddTask * WidgetAddTask::GetIntance()
{
  return add_task_;
}

void WidgetAddTask::InitTargetList()
{
  for(int i = 0; i < items_.count(); ++i)
  {
    WidgetTargetListItem* item = items_[i];
    ui->verticalLayout_3->addWidget(item);
  }
}

void WidgetAddTask::GetTargetListInfo()
{
  while(!items_.isEmpty())
  {
    WidgetTargetListItem* item = items_.takeLast();
    ui->verticalLayout_3->removeWidget(item);
    delete item;
  }
}

void WidgetAddTask::SetInitData()
{
  ui->comboBox_action->clear();
  ui->comboBox_agvFloor->clear();
  ui->comboBox_floor->clear();
  ui->comboBox_target->clear();
  ui->comboBox_type->clear();
  ui->comboBox_device->clear();
  ui->comboBox_type->addItem("All", 0);
  // station type
  QVariantMap stationType = ConfigModule::getInstance()->getStationTypeParams();

  if(stationType.isEmpty())
  {
    return;
  }

  QList<QString> keys = stationType.keys();
  for(int i = 0; i < keys.size(); ++i)
  {
    QVariantMap item = stationType.value(keys[i]).toMap();
    int         id   = item.value("id").toInt();
    QString     name = item.value("name").toString();

    ui->comboBox_type->addItem(name, id);
  }
  ui->comboBox_type->setCurrentIndex(0);
  // station row
  int         typeId = ui->comboBox_type->itemData(ui->comboBox_type->currentIndex()).toInt();
  QVariantMap row    = StationModule::getInstance()->getCraftStationType(typeId);

  if(row.isEmpty())
  {
    ui->widget_4->setHidden(true);
  }
  else
  {
    ui->widget_4->setHidden(false);
    ui->comboBox_agvFloor->clear();
    int count = row.value("rowCount").toInt();
    for(int i = 1; i <= count; ++i)
    {
      ui->comboBox_agvFloor->addItem(tr("第%1排").arg(i), i);
    }
  }
  // target station
  int          rowId       = ui->comboBox_agvFloor->itemData(ui->comboBox_agvFloor->currentIndex()).toInt();
  QVariantList stationList = StationModule::getInstance()->getStationFromStationType(typeId, rowId);

  for(int i = 0; i < stationList.size(); ++i)
  {
    QVariantMap map = stationList.at(i).toMap();
// QJsonObject   obj = QJsonObject::fromVariantMap(map);
// QJsonDocument doc(obj);
// QString       strRet = QString(doc.toJson(QJsonDocument::Indented));

// qDebug() << strRet;
    int         stationTypeId = map.value("stationTypeId").toInt();
    QString     id            = map.value("id").toString();
    QVariantMap temp;
    temp.insert("stationTypeId", stationTypeId);
    temp.insert("id", id);
    QString name = map.value("name").toString();

    ui->comboBox_target->addItem(name, temp);
  }

  // station level
  QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
  int stationTypeId = 0;
  if(!map.empty())
  {
    stationTypeId = map.value("stationTypeId").toInt();
  }

  QVariantMap   stationLevel = ConfigModule::getInstance()->getMaterialBufferParams(stationTypeId);

  if(stationLevel.isEmpty())
  {
    ui->widget_floor->setHidden(true);
  }
  else
  {
    ui->widget_floor->setHidden(false);

    QVariantMap::iterator iter = stationLevel.begin();
    for(; iter != stationLevel.end(); ++iter)
    {
      QVariantMap map = iter.value().toMap();
      int id = map.value("id").toInt();
      QString name = map.value("name").toString();

      ui->comboBox_floor->addItem(name, id);
    }
  }

  // 机构动作
  QVariantList actionList = ConfigModule::getInstance()->getActionTypeOfStationType(stationTypeId);
  for(int i = 0; i < actionList.size(); ++i)
  {
    int              type = actionList.at(i).toInt();
    NameAndLabelInfo info = GetActionType(type);
    ui->comboBox_action->addItem(info.name, type);
  }
  // 车体设备
  if(!UserConfigs::AgvAxisName.isEmpty())
  {
    ui->comboBox_device->addItem(UserConfigs::AgvAxisName, UserConfigs::AgvAxisId);
  }
}

void WidgetAddTask::AddButtonClicked()
{
  WidgetTargetListItem* item = new WidgetTargetListItem();
  TargetListInfo        info;
  info.actionName    = ui->comboBox_action->currentText();
  info.stationName   = ui->comboBox_target->currentText();
  info.action        = ui->comboBox_action->itemData(ui->comboBox_action->currentIndex()).toInt();
  info.agvAxisId     = ui->comboBox_device->itemData(ui->comboBox_device->currentIndex()).toInt();
  info.stationAxisId = ui->comboBox_floor->itemData(ui->comboBox_floor->currentIndex()).toInt();
  QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
  info.stationId     = map.value("id").toString();

  item->SetInput(info);
  ui->verticalLayout_3->addWidget(item);
  items_ << item;
}

void WidgetAddTask::CreateButtonClicked()
{
  QVariantMap dataMap;
  dataMap.insert("agvId", UserConfigs::AgvId);
  int count = ui->checkBox->isChecked() ? 10000000 : 0;
  dataMap.insert("maxLoopTimes", count);
  dataMap.insert("mapId", ConfigModule::getInstance()->getAgvBelongMapId(UserConfigs::AgvId));
  dataMap.insert("taskNum", UserConfigs::Uuid);

  QVariantList stationList;

  for(int i = 0; i < items_.count(); ++i)
  {
    QVariantList   actionList;
    QVariantMap    actionListMap;
    TargetListInfo info = items_[i]->GetInput();
    actionListMap.insert("action", info.action);
    actionListMap.insert("agvAxisId", info.agvAxisId);
    actionListMap.insert("stationAxisId", info.stationAxisId);
    actionList.insert(i, actionListMap);
    QVariantMap stationListMap;

    stationListMap.insert("actionList", actionList);
    stationListMap.insert("stationId", info.stationId);
    stationListMap.insert("stationName", info.stationName);

    stationList.insert(i, stationListMap);
  }

  dataMap.insert("stationList", stationList);
  //send request
}

void WidgetAddTask::StationTypeChanged(int index)
{
  ui->comboBox_agvFloor->clear();
  ui->comboBox_target->clear();
  ui->comboBox_action->clear();
  ui->comboBox_floor->clear();
  int typeId = ui->comboBox_type->itemData(index).toInt();

  QVariantMap row = StationModule::getInstance()->getCraftStationType(typeId);

  if(row.isEmpty())
  {
    ui->widget_4->setHidden(true);
  }
  else
  {
    ui->widget_4->setHidden(false);

    int count = row.value("rowCount").toInt();
    for(int i = 1; i <= count; ++i)
    {
      ui->comboBox_agvFloor->addItem(tr("第%1排").arg(i), i);
    }
  }

  // target station

  int          rowId       = ui->comboBox_agvFloor->itemData(ui->comboBox_agvFloor->currentIndex()).toInt();
  QVariantList stationList = StationModule::getInstance()->getStationFromStationType(typeId, rowId);

  for(int i = 0; i < stationList.size(); ++i)
  {
    QVariantMap map = stationList.at(i).toMap();

    int         stationTypeId = map.value("stationTypeId").toInt();
    QString     id            = map.value("id").toString();
    QVariantMap temp;
    temp.insert("stationTypeId", stationTypeId);
    temp.insert("id", id);
    QString name = map.value("name").toString();

    ui->comboBox_target->addItem(name, temp);
  }

  if(index == 0)
  {
    QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
    if(!map.empty())
    {
      typeId = map.value("stationTypeId").toInt();
    }
  }
  // station level
  QVariantMap stationLevel = ConfigModule::getInstance()->getMaterialBufferParams(typeId);
  if(stationLevel.isEmpty())
  {
    ui->widget_floor->setHidden(true);
  }
  else
  {
    ui->widget_floor->setHidden(false);
    QVariantMap::iterator iter = stationLevel.begin();
    for(; iter != stationLevel.end(); ++iter)
    {
      QVariantMap map = iter.value().toMap();
      int id = map.value("id").toInt();
      QString name = map.value("name").toString();
      ui->widget_floor->setHidden(name.isEmpty());
      ui->comboBox_floor->addItem(name, id);
    }
  }
  // 机构动作
  QVariantList actionList = ConfigModule::getInstance()->getActionTypeOfStationType(typeId);
  for(int i = 0; i < actionList.size(); ++i)
  {
    int              type = actionList.at(i).toInt();
    NameAndLabelInfo info = GetActionType(type);
    ui->comboBox_action->addItem(info.name, type);
  }
}

void WidgetAddTask::StationRowChanged(int index)
{
  ui->comboBox_target->clear();
  int typeId = ui->comboBox_type->itemData(ui->comboBox_type->currentIndex()).toInt();
  if(ui->comboBox_type->currentIndex() == 0)
  {
    QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
    if(!map.empty())
    {
      typeId = map.value("stationTypeId").toInt();
    }
  }

  int          rowId       = ui->comboBox_agvFloor->itemData(index).toInt();
  QVariantList stationList = StationModule::getInstance()->getStationFromStationType(typeId, rowId);
  if(stationList.isEmpty())
  {
    return;
  }

  for(int i = 0; i < stationList.size(); ++i)
  {
    QVariantMap map = stationList.at(i).toMap();

    int         stationTypeId = map.value("stationTypeId").toInt();
    QString     id            = map.value("id").toString();
    QVariantMap temp;
    temp.insert("stationTypeId", stationTypeId);
    temp.insert("id", id);
    QString name = map.value("name").toString();

    ui->comboBox_target->addItem(name, temp);
  }
}

void WidgetAddTask::Initialize()
{
  QFont font = ui->label_title->font();
  font.setPixelSize(16);
  ui->label_title->setFont(font);
  ui->label_tragetList->setFont(font);
  ui->pushButton_add->setFont(font);
  ui->pushButton_create->setFont(font);

  font.setPixelSize(12);
  ui->label_type->setFont(font);
  ui->label_target->setFont(font);
  ui->label_loop->setFont(font);
  ui->label_floor->setFont(font);
  ui->label_action->setFont(font);
  ui->label_action1->setFont(font);
  ui->label_target1->setFont(font);
  ui->label_agvFloor->setFont(font);
  ui->label_device->setFont(font);
  ui->comboBox_type->setFont(font);
  ui->comboBox_floor->setFont(font);
  ui->comboBox_action->setFont(font);
  ui->comboBox_target->setFont(font);
  ui->comboBox_agvFloor->setFont(font);
  ui->comboBox_device->setFont(font);

  ui->pushButton_close->setFixedSize(QSize(34, 34));
  ui->label->setFixedSize(QSize(32, 11));
  ui->label_2->setFixedSize(QSize(32, 11));
}

void WidgetAddTask::TranslateLanguage()
{
  ui->label_title->setText(tr("添加任务"));
  ui->label_type->setText(tr("站台类型"));
  ui->label_target->setText(tr("目标站台"));
  ui->label_floor->setText(tr("站台层级"));
  ui->label_agvFloor->setText(tr("所在排数"));
  ui->label_action->setText(tr("机构动作"));
  ui->label_device->setText(tr("车体设备"));
  ui->label_loop->setText(tr("循环任务"));
  ui->label_tragetList->setText(tr("目标列表"));
  ui->label_target1->setText(tr("目标站台"));
  ui->label_action1->setText(tr("机构动作"));
  ui->pushButton_add->setText(tr("添加目标列表"));
  ui->pushButton_create->setText(tr("创建任务"));
}

WidgetTargetListItem::WidgetTargetListItem(QWidget* parent)
  : QWidget(parent)
  , info_()
{
  SetupUi();
}

WidgetTargetListItem::~WidgetTargetListItem()
{
}

void WidgetTargetListItem::SetInput(TargetListInfo info)
{
  station_label_->setText(info.stationName);
  action_label_->setText(info.actionName);

  info_ = info;
}

TargetListInfo WidgetTargetListItem::GetInput()
{
  return info_;
}

void WidgetTargetListItem::SetupUi()
{
  setStyleSheet("QWidget{background:transprent;}");
  QVBoxLayout* main_layout = new QVBoxLayout(this);

  station_label_ = new QLabel(this);
  action_label_  = new QLabel(this);

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(station_label_);
  layout->addWidget(action_label_);

  QFont font = station_label_->font();
  font.setPixelSize(12);
  station_label_->setFont(font);
  action_label_->setFont(font);
  QString style = "QLabel{color:rgb(255,255,255);border:none;}";
  station_label_->setStyleSheet(style);
  action_label_->setStyleSheet(style);

  layout->setMargin(11);
  frame_ = new QFrame(this);
  frame_->setFixedHeight(1);
  frame_->setStyleSheet("background:rgb(184,184,184)");

  main_layout->addLayout(layout);
  main_layout->addWidget(frame_);
  main_layout->setSpacing(11);

  setFixedHeight(35);
}
