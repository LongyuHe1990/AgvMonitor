#include "widgethomepage.h"
#include "ui_widgethomepage.h"


WidgetHomepage::WidgetHomepage(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetHomepage)
  , m_configModule(nullptr)
  , m_stationModule(nullptr)
  , m_taskModule(nullptr)
{
  ui->setupUi(this);

  Initialize();

  connect(widget_tasklist_, SIGNAL(ShowAddTaskListWidget()), this, SLOT(ShowAddTaskListWidget()));
  connect(widget_addtask_, SIGNAL(HideAddTaskListWidget()), this, SLOT(HideAddTaskListWidget()));
  connect(widget_errorinfo_, SIGNAL(ShowDetailWidget()), this, SIGNAL(ShowErrorDetailWidget()));
}

WidgetHomepage::~WidgetHomepage()
{
    delete ui;
}

void WidgetHomepage::ShowAddTaskListWidget()
{
    StationModule::getInstance()->getStationFromStationType(4,1);
  ui->stackedWidget->setCurrentIndex(1);
}

void WidgetHomepage::HideAddTaskListWidget()
{
  ui->stackedWidget->setCurrentIndex(0);
}

void WidgetHomepage::Initialize()
{
    m_configModule = new ConfigModule(this);
    m_stationModule = new StationModule(this);
    m_taskModule = new TaskModule(this);

    widget_baseinfo_  = new WidgetBaseInfo(this);
    widget_errorinfo_ = new WidgetErrorInfo(this);
    widget_tasklist_  = new WidgetTaskList(this);
    widget_allinfo_   = new WidgetAllInfo(this);
    widget_addtask_   = new WidgetAddTask(this);
    MapMonitoringView* mapView = new MapMonitoringView(this);


    ui->verticalLayout->addWidget(widget_baseinfo_, 1);
    ui->verticalLayout->addWidget(widget_errorinfo_, 1);
    // 地图
    ui->verticalLayout_2->addWidget(mapView, 2);
    ui->verticalLayout_2->addWidget(widget_tasklist_, 1);
    ui->stackedWidget->addWidget(widget_allinfo_);
    ui->stackedWidget->addWidget(widget_addtask_);

    ui->stackedWidget->setCurrentIndex(0);
}
