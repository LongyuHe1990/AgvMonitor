#include "widgethomepage.h"
#include "ui_widgethomepage.h"
#include "statisticAnalysis/timeUtilizationRateView.h"
#include "statisticAnalysis/performanceUtilizationRateView.h"

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
  connect(Widget_error_, SIGNAL(ShowDetailWidget()), this, SIGNAL(ShowErrorDetailWidget()));
}

WidgetHomepage::~WidgetHomepage()
{
  delete ui;
}

void WidgetHomepage::ShowAddTaskListWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void WidgetHomepage::HideAddTaskListWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void WidgetHomepage::Initialize()
{
  m_configModule  = new ConfigModule(this);
  m_stationModule = new StationModule(this);
  m_taskModule    = new TaskModule(this);

  widget_baseinfo_ = new WidgetBaseInfo(this);
  widget_tasklist_ = new WidgetTaskList(this);
  widget_addtask_  = new WidgetAddTask(this);
  Widget_error_    = new WidgetErrorInfo(this);
  MapMonitoringView* mapView = new MapMonitoringView(this);

  ui->verticalLayout->addWidget(widget_baseinfo_);
  // 地图
  ui->stackedWidget->addWidget(mapView);
  ui->stackedWidget->addWidget(widget_addtask_);
  ui->verticalLayout_5->addWidget(widget_tasklist_);

  ui->verticalLayout_4->addWidget(new TimeUtilizationRateView(this));
  ui->verticalLayout_4->setStretch(0,1);
  ui->verticalLayout_4->addWidget(new PerformanceUtilizationRateView(this));
  ui->verticalLayout_4->setStretch(1,1);
  //异常报警
  ui->verticalLayout_4->addWidget(Widget_error_);
  ui->verticalLayout_4->setStretch(2,1);
  ui->stackedWidget->setCurrentIndex(0);

}
