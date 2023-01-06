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
#include <QMenu>
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"
#include "moduleData/taskModule.h"
#include "common/global_helper.h"
#include "common/global_config.h"
#include "webSocket/webSocketClient.h"
#include "customData.h"

static WidgetAddTask* add_task_ = nullptr;

WidgetAddTask::WidgetAddTask(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetAddTask)
  , count_(0)
{
  ui->setupUi(this);

  add_task_ = this;

  Initialize();
  InitTargetListTableView();
  TranslateLanguage();

  ui->widget_4->setHidden(true);

  QString style =
    "\
          QComboBox{ color: rgb(255, 255, 255); background-color: rgb(64, 67, 76); border:none; combobox-popup: 0;border-radius:4px;font-size:24px;} \
          QComboBox::hover{color: rgb(227, 186, 56); background-color: rgb(64, 67, 76); } \
          QComboBox::drop-down{ width:10; subcontrol-origin:padding; subcontrol-position:top right; border-left-width:0px; border-left-color:darkgray; border-left-style:solid; padding-right:10px; } \
          QComboBox::down-arrow{ border-image:url(:/image/home/xiala_icon.png); width:0px; height:0px; }\
          QListView::item{padding-left:5px;color: rgb(255,255,255); background-color: rgb(64, 67, 76);}\
          QListView::item:hover{ color: rgb(227,186,56); background-color: rgb(64, 67, 76);}\
          QComboBox QAbstractItemView {border:0px solid #dddddd;outline:0px;height:30px;} \
          QComboBox QAbstractItemView::item {min-height: 30px;background-color: rgb(64, 67, 76);color:rgb(255,255,255);padding-left:11px;padding-right:11px;outline:1px solid rgb(216,215,211);} \
          QComboBox QAbstractItemView::item:hover {background-color: rgb(64, 67, 76);color:rgb(227,186,56);}";
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
  connect(ui->comboBox_target, SIGNAL(currentIndexChanged(int)), this, SLOT(TargetStationChanged(int)));
}

WidgetAddTask::~WidgetAddTask()
{
  delete ui;
}

WidgetAddTask * WidgetAddTask::GetIntance()
{
  return add_task_;
}

void WidgetAddTask::SetInitData()
{
  model_->removeRows(0, model_->rowCount());
  count_ = 0;
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
  QVariantMap row    = StationModule::getInstance()->getStationType(typeId);

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
  ui->comboBox_agvFloor->setCurrentIndex(0);
  // target station
  int          rowId       = ui->comboBox_agvFloor->itemData(ui->comboBox_agvFloor->currentIndex()).toInt();
  QVariantList stationList = StationModule::getInstance()->getStationFromStationType(typeId, rowId);
  ui->comboBox_target->clear();
  for(int i = 0; i < stationList.size(); ++i)
  {
    QVariantMap map = stationList.at(i).toMap();
// QJsonObject   obj = QJsonObject::fromVariantMap(map);
// QJsonDocument doc(obj);
// QString       strRet = QString(doc.toJson(QJsonDocument::Indented));

// qDebug() << strRet;
    int         stationTypeId = map.value("stationTypeId").toInt();
    int     id            = map.value("id").toInt();
    QVariantMap temp;
    temp.insert("stationTypeId", stationTypeId);
    temp.insert("id", id);
    QString name = map.value("name").toString();

    ui->comboBox_target->addItem(name, temp);
  }

  // station level
  ui->comboBox_floor->clear();
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
      if(name.isEmpty())
      {
          ui->comboBox_floor->addItem(QString::number(id), id);
      }
      else
      {
          ui->comboBox_floor->addItem(name, id);
      }
    }
  }

  // 机构动作
  ui->comboBox_action->clear();
  QVariantList actionList = ConfigModule::getInstance()->getActionTypeOfStationType(stationTypeId);
  for(int i = 0; i < actionList.size(); ++i)
  {
    int              type = actionList.at(i).toInt();
    NameAndLabelInfo info = GetActionType(type);
    ui->comboBox_action->addItem(info.name, type);
  }
  // 车体仓位
  QVariantMap config = ConfigModule::getInstance()->getConfig(ConfigType::Agv, UserConfigs::AgvId);
  int agvTypeId = config.value("agvTypeId").toInt();

  QVariantMap position = ConfigModule::getInstance()->getConfig(ConfigType::AgvType, agvTypeId);

  QVariantList materialBufferParamList = position.value("materialBufferParamList").toList();
  for(int i = 0; i < materialBufferParamList.size(); ++i)
  {
      QVariantMap map = materialBufferParamList.at(i).toMap();
      int id = map.value("id").toInt();
      QString name = map.value("name").toString();
      if(name.isEmpty())
      {
         ui->comboBox_device->addItem(QString::number(id), id);
      }
      else
      {
         ui->comboBox_device->addItem(name, id);
      }
  }
}

void WidgetAddTask::AddButtonClicked()
{
  TargetListInfo        info;
  info.actionName    = ui->comboBox_action->currentText();
  info.stationName   = ui->comboBox_target->currentText();
  info.action        = ui->comboBox_action->itemData(ui->comboBox_action->currentIndex()).toInt();
  info.agvBufferIndex     = ui->comboBox_device->itemData(ui->comboBox_device->currentIndex()).toInt();
  info.stationBufferIndex = ui->comboBox_floor->itemData(ui->comboBox_floor->currentIndex()).toInt();
  QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
  info.stationId     = map.value("id").toInt();

  model_->setItem(count_, 0, new QStandardItem(QString::number(count_)));
  model_->setItem(count_, 1, new QStandardItem(info.stationName));
  model_->setItem(count_, 2, new QStandardItem(info.actionName));
  model_->item(count_, 0)->setTextAlignment(Qt::AlignCenter);
  model_->item(count_, 1)->setTextAlignment(Qt::AlignCenter);
  model_->item(count_, 2)->setTextAlignment(Qt::AlignCenter);
  model_->item(count_, 0)->setToolTip(QString::number(count_));
  model_->item(count_, 1)->setToolTip(info.stationName);
  model_->item(count_, 2)->setToolTip(info.actionName);
  model_->item(count_)->setData(QVariant::fromValue(info));

  count_++;
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

  for(int i = 0; i < model_->rowCount(); ++i)
  {
    QVariantList   actionList;
    QVariantMap    actionListMap;
    TargetListInfo info = model_->item(i)->data().value<TargetListInfo>();
    actionListMap.insert("action", info.action);
    actionListMap.insert("agvBufferIndex", info.agvBufferIndex);
    actionListMap.insert("stationBufferIndex", info.stationBufferIndex);
    actionList.insert(i, actionListMap);
    QVariantMap stationListMap;

    stationListMap.insert("actionList", actionList);
    stationListMap.insert("stationId", info.stationId);
    stationListMap.insert("stationName", info.stationName);

    stationList.insert(i, stationListMap);
  }

  dataMap.insert("stationList", stationList);

  QVariantMap Content;
  Content.insert("Content", dataMap);
  Content.insert("OperationType", int(StationOperationType::Update_Station_Material));
  //
  QVariantMap sendData;
  sendData.insert("ModuleType", int(DataModuleType::Task));
  sendData.insert("Async", false);
  sendData.insert("ModuleData", Content);
  sendData.insert("RequestId", int(RequestIdType::REQUEST_CREATE_TASK));

  QJsonObject   obj = QJsonObject::fromVariantMap(sendData);
  QJsonDocument doc(obj);
  QString       strRet = QString(doc.toJson(QJsonDocument::Indented));

  //send request
  WebSocketClient::getInstance()->sendDataToServer(strRet);

}

void WidgetAddTask::StationTypeChanged(int index)
{
  ui->comboBox_agvFloor->clear();
  ui->comboBox_target->clear();
  ui->comboBox_action->clear();
  ui->comboBox_floor->clear();
  int typeId = ui->comboBox_type->itemData(index).toInt();

  QVariantMap row = StationModule::getInstance()->getStationType(typeId);

  if(row.isEmpty())
  {
    ui->widget_4->setHidden(true);
  }
  else
  {
    int count = row.value("rowCount").toInt();
    if(count < 1)
    {
        ui->widget_4->setHidden(true);
    }
    else
    {
        ui->widget_4->setHidden(false);
        for(int i = 1; i <= count; ++i)
        {
          ui->comboBox_agvFloor->addItem(tr("第%1排").arg(i), i);
        }
    }
  }
   ui->comboBox_agvFloor->setCurrentIndex(0);
  // target station

  int          rowId       = ui->comboBox_agvFloor->itemData(ui->comboBox_agvFloor->currentIndex()).toInt();
  QVariantList stationList = StationModule::getInstance()->getStationFromStationType(typeId, rowId);

  for(int i = 0; i < stationList.size(); ++i)
  {
    QVariantMap map = stationList.at(i).toMap();

    int         stationTypeId = map.value("stationTypeId").toInt();
    int     id            = map.value("id").toInt();
    QVariantMap temp;
    temp.insert("stationTypeId", stationTypeId);
    temp.insert("id", id);
    QString name = map.value("name").toString();

    ui->comboBox_target->addItem(name, temp);
  }

  int stationTypeId = 0;

  QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
  if(!map.empty())
  {
    stationTypeId = map.value("stationTypeId").toInt();
  }

  // station level
  ui->comboBox_floor->clear();
  QVariantMap stationLevel = ConfigModule::getInstance()->getMaterialBufferParams(stationTypeId);
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
      if(name.isEmpty())
      {
          ui->comboBox_floor->addItem(QString::number(id), id);
      }
      else
      {
          ui->comboBox_floor->addItem(name, id);
      }
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
}

void WidgetAddTask::StationRowChanged(int index)
{
  ui->comboBox_target->clear();

  int stationTypeId = 0;

  QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
  if(!map.empty())
  {
    stationTypeId = map.value("stationTypeId").toInt();
  }

  ui->comboBox_agvFloor->setCurrentIndex(0);
  int          rowId       = ui->comboBox_agvFloor->itemData(index).toInt();
  QVariantList stationList = StationModule::getInstance()->getStationFromStationType(stationTypeId, rowId);
  if(stationList.isEmpty())
  {
    return;
  }

  for(int i = 0; i < stationList.size(); ++i)
  {
    QVariantMap map = stationList.at(i).toMap();

    int         stationTypeId = map.value("stationTypeId").toInt();
    int     id            = map.value("id").toInt();
    QVariantMap temp;
    temp.insert("stationTypeId", stationTypeId);
    temp.insert("id", id);
    QString name = map.value("name").toString();

    ui->comboBox_target->addItem(name, temp);
  }
}

void WidgetAddTask::TargetStationChanged(int index)
{
    ui->comboBox_action->clear();

    int stationTypeId = 0;

    QVariantMap map = ui->comboBox_target->itemData(ui->comboBox_target->currentIndex()).toMap();
    if(!map.empty())
    {
      stationTypeId = map.value("stationTypeId").toInt();
    }

    // 机构动作
    QVariantList actionList = ConfigModule::getInstance()->getActionTypeOfStationType(stationTypeId);
    for(int i = 0; i < actionList.size(); ++i)
    {
      int              type = actionList.at(i).toInt();
      NameAndLabelInfo info = GetActionType(type);
      ui->comboBox_action->addItem(info.name, type);
    }
}

void WidgetAddTask::TableViewMenu(const QPoint &pos)
{
    QModelIndex current_index = ui->tableView->currentIndex();
    if(current_index.isValid())
    {
        QMenu* menu = new QMenu(this);
        menu->addAction(tr("Delete"), this, SLOT(DeleteMenuTriggered()));
        menu->addAction(tr("Clear"), this, SLOT(ClearMenuTriggered()));

        menu->exec(QCursor::pos());
    }
}

void WidgetAddTask::DeleteMenuTriggered()
{
    model_->removeRow(ui->tableView->currentIndex().row());
    count_--;
}

void WidgetAddTask::ClearMenuTriggered()
{
    model_->removeRows(0, model_->rowCount());
    count_ = 0;
}

void WidgetAddTask::Initialize()
{
  QFont font = ui->label_title->font();
  font.setPixelSize(32);
  ui->label_title->setFont(font);
  ui->label_tragetList->setFont(font);
  ui->pushButton_add->setFont(font);
  ui->pushButton_create->setFont(font);

  font.setPixelSize(24);
  ui->label_type->setFont(font);
  ui->label_target->setFont(font);
  ui->label_loop->setFont(font);
  ui->label_floor->setFont(font);
  ui->label_action->setFont(font);
  ui->label_agvFloor->setFont(font);
  ui->label_device->setFont(font);
  ui->comboBox_type->setFont(font);
  ui->comboBox_floor->setFont(font);
  ui->comboBox_action->setFont(font);
  ui->comboBox_target->setFont(font);
  ui->comboBox_agvFloor->setFont(font);
  ui->comboBox_device->setFont(font);

  ui->pushButton_close->setFixedSize(QSize(45, 45));
  ui->label->setFixedSize(QSize(63, 22));
  ui->label_2->setFixedSize(QSize(63, 22));
  ui->pushButton_add->setFixedHeight(60);
  ui->pushButton_create->setFixedHeight(60);
}

void WidgetAddTask::TranslateLanguage()
{
  ui->label_title->setText(tr("Add task"));
  ui->label_type->setText(tr("Platform type"));
  ui->label_target->setText(tr("Target platform"));
  ui->label_floor->setText(tr("Platform level"));
  ui->label_agvFloor->setText(tr("Row number"));
  ui->label_action->setText(tr("Mechanism action"));
  ui->label_device->setText(tr("Car body position"));  //车体仓位
  ui->label_loop->setText(tr("Circular task"));
  ui->label_tragetList->setText(tr("Target List"));
  ui->pushButton_add->setText(tr("Add Target List"));
  ui->pushButton_create->setText(tr("Create Task"));

  QStringList header_List;
  header_List << tr("Number") << tr("Target platform") << tr("Mechanism action");
  model_->setHorizontalHeaderLabels(header_List);
}

void WidgetAddTask::InitTargetListTableView()
{
   model_ = new QStandardItemModel();
   ui->tableView->setModel(model_);
   model_->setColumnCount(3);

   ui->tableView->verticalHeader()->setVisible(false);
   ui->tableView->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setAutoScroll(true);

   ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
   ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

   ui->tableView->setColumnWidth(0, 50);
   ui->tableView->setColumnWidth(1, 200);
   ui->tableView->setColumnWidth(2, 168);
   ui->tableView->resizeColumnToContents(2);

   ui->tableView->setFocusPolicy(Qt::NoFocus);
   ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
   ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

   //
   ui->tableView->setShowGrid(false);                                                  // 设置不显示格子线
   ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);                  // 设置不可编辑

   QFont font = ui->tableView->font();
   font.setPixelSize(24);
   ui->tableView->setFont(font);

   ui->tableView->horizontalHeader()->setFont(font);

   ui->tableView->verticalHeader()->setDefaultSectionSize(48);
   ui->tableView->horizontalHeader()->setFixedHeight(48);                              // 设置表头的高度
   ui->tableView->setStyleSheet(
     "QTableView::Item{ border:none; }\
      QTableView::item:selected {background:transparent;color:rgb(240,179,28);border:none;}\
      QTableView{background:transparent;color:rgb(255,255,255);border:none;} ");
   ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{ color:rgb(227,186,56);background-color: rgb(32, 42, 67); border:none; }");

   connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(TableViewMenu(QPoint)));
   ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void WidgetAddTask::changeEvent(QEvent *e)
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
