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

  GetTargetListInfo();
// InitTargetList();

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

  connect(ui->pushButton_close, SIGNAL(clicked()), this, SIGNAL(HideAddTaskListWidget()));
  connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(AddButtonClicked()));
  connect(ui->pushButton_create, SIGNAL(clicked()), this, SLOT(CreateButtonClicked()));
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

  // 数据
  WidgetTargetListItem* item = new WidgetTargetListItem();
  item->SetInput("充电桩01", "充电");
  items_ << item;
}

void WidgetAddTask::AddButtonClicked()
{
  WidgetTargetListItem* item = new WidgetTargetListItem();
  item->SetInput(ui->comboBox_target->currentText(), ui->comboBox_type->currentText());
  items_ << item;
}

void WidgetAddTask::CreateButtonClicked()
{
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
  ui->comboBox_type->setFont(font);
  ui->comboBox_floor->setFont(font);
  ui->comboBox_action->setFont(font);
  ui->comboBox_target->setFont(font);
  ui->comboBox_agvFloor->setFont(font);

  ui->pushButton_close->setFixedSize(QSize(34, 34));
}

void WidgetAddTask::TranslateLanguage()
{
  ui->label_title->setText(tr("添加任务"));
  ui->label_type->setText(tr("站台类型"));
  ui->label_target->setText(tr("目标站台"));
  ui->label_floor->setText(tr("站台层级"));
  ui->label_agvFloor->setText(tr("AGV上下层"));
  ui->label_action->setText(tr("机构动作"));
  ui->label_loop->setText(tr("循环任务"));
  ui->label_tragetList->setText(tr("目标列表"));
  ui->label_target1->setText(tr("目标站台"));
  ui->label_action1->setText(tr("机构动作"));
  ui->pushButton_add->setText(tr("添加目标列表"));
  ui->pushButton_create->setText(tr("创建任务"));

  ui->comboBox_type->addItem("All");
  ui->comboBox_type->addItem("普通站台");
  ui->comboBox_type->addItem("待命站台");
  ui->comboBox_type->addItem("充电站台");
  ui->comboBox_type->addItem("取货点");
  ui->comboBox_type->addItem("放货点");
}

WidgetTargetListItem::WidgetTargetListItem(QWidget* parent)
  : QWidget(parent)
{
  SetupUi();
}

WidgetTargetListItem::~WidgetTargetListItem()
{
}

void WidgetTargetListItem::SetupUi()
{
  QVBoxLayout* main_layout = new QVBoxLayout(this);

  charge_label_ = new QLabel(this);
  status_label_ = new QLabel(this);

  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(charge_label_);
  layout->addWidget(status_label_);

  QFont font = charge_label_->font();
  font.setPixelSize(12);
  charge_label_->setFont(font);
  status_label_->setFont(font);
  QString style = "QLabel{background-color:rgba(255,255,255,0);color:rgb(255,255,255);border:none;}";
  charge_label_->setStyleSheet(style);
  status_label_->setStyleSheet(style);

  layout->setMargin(11);
  frame_ = new QFrame(this);
  frame_->setFixedHeight(1);
  frame_->setStyleSheet("background:rgb(184,184,184)");

  main_layout->addLayout(layout);
  main_layout->addWidget(frame_);
  main_layout->setSpacing(11);

  setFixedHeight(35);
}

void WidgetTargetListItem::SetInput(const QString& charge_label, const QString& status_label)
{
  charge_label_->setText(charge_label);
  status_label_->setText(status_label);
}
