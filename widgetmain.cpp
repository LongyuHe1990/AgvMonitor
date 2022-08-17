#include "widgetmain.h"
#include "ui_widgetmain.h"
#include <QKeyEvent>

#define WIDGET_HOME 0
#define WIDGET_SETTING 1
#define WIDGET_ERROR 2

WidgetMain::WidgetMain(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetMain)
  , event_loop_(nullptr)
{
  ui->setupUi(this);

#ifdef Q_OS_WIN
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif

  Initialize();
  TranslateLanguage();

  widget_home_    = new WidgetHomepage(this);
  widget_setting_ = new WidgetSetting(this);
  widget_switch_  = new WidgetSwitch(this);
  widget_error_   = new WidgetError(this);

  ui->stackedWidget->addWidget(widget_home_);
  ui->stackedWidget->addWidget(widget_setting_);
  ui->stackedWidget->addWidget(widget_error_);

  connect(ui->pushButton_switch, SIGNAL(clicked()), this, SLOT(SwitchButtonClicked()));
  connect(ui->pushButton_setting, SIGNAL(clicked()), this, SLOT(SettingButtonClicked()));
  connect(ui->pushButton_back, SIGNAL(clicked()), this, SLOT(BackButtonClicked()));
  connect(widget_home_, SIGNAL(ShowErrorDetailWidget()), this, SLOT(ShowErrorDetailWidget()));
  connect(widget_setting_, SIGNAL(SwitchVisitor()), this, SLOT(VisitorModel()));
  connect(widget_switch_, SIGNAL(AdminModel()), this, SLOT(AdminModel()));

  VisitorModel();
}

WidgetMain::~WidgetMain()
{
  delete ui;
}

void WidgetMain::SetEventLoop(QEventLoop* loop)
{
  event_loop_ = loop;
}

void WidgetMain::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Escape)
  {
    event_loop_->exit(0);
    QApplication::exit(0);
    QApplication::closeAllWindows();
    qDebug()<<"closeApp";
  }
}

void WidgetMain::SettingButtonClicked()
{
  ui->stackedWidget->setCurrentIndex(WIDGET_SETTING);
  ui->pushButton_back->setHidden(false);
}

void WidgetMain::SwitchButtonClicked()
{
  widget_switch_->show();
  ui->pushButton_back->setHidden(true);
}

void WidgetMain::ShowErrorDetailWidget()
{
  ui->stackedWidget->setCurrentIndex(WIDGET_ERROR);
  ui->pushButton_back->setHidden(false);
}

void WidgetMain::VisitorModel()
{
  ui->pushButton_back->setHidden(true);
  ui->pushButton_setting->setHidden(true);
  ui->pushButton_switch->setHidden(false);
  WidgetBaseInfo::GetInstance()->VisitorModel(true);
  WidgetTaskList::GetInstance()->VisitorModel(true);
  ui->stackedWidget->setCurrentIndex(WIDGET_HOME);
}

void WidgetMain::AdminModel()
{
  ui->pushButton_back->setHidden(true);
  ui->pushButton_setting->setHidden(false);
  ui->pushButton_switch->setHidden(true);
  WidgetBaseInfo::GetInstance()->VisitorModel(false);
  WidgetTaskList::GetInstance()->VisitorModel(false);
}

void WidgetMain::BackButtonClicked()
{
  ui->stackedWidget->setCurrentIndex(WIDGET_HOME);
  ui->pushButton_back->setHidden(true);
}

void WidgetMain::Initialize()
{
  QFont font = ui->label_2->font();
  font.setPixelSize(20);
  ui->label_2->setFont(font);

  font.setPixelSize(16);
  ui->pushButton_switch->setFont(font);
  ui->pushButton_setting->setFont(font);
  ui->pushButton_back->setFont(font);

  ui->label->setFixedSize(QSize(46, 18));
  ui->label_5->setFixedSize(QSize(28, 28));
}

void WidgetMain::TranslateLanguage()
{
  ui->label_2->setText("牵星系统调度平台");
  ui->pushButton_switch->setText("切换");
  ui->pushButton_setting->setText("设置");
  ui->pushButton_back->setText(tr("返回"));
}
