#include "widgetmain.h"
#include "ui_widgetmain.h"
#include <QKeyEvent>
#include <QDebug>

#define WIDGET_HOME 0
#define WIDGET_CONTROL 1
//#define WIDGET_DETAIL 2
//#define WIDGET_LOG 3
//#define WIDGET_SETTING 4
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

  button_group_ = new QButtonGroup(this);
  button_group_->addButton(ui->pushButton_home, 0);
  button_group_->addButton(ui->pushButton_control, 1);
  button_group_->addButton(ui->pushButton_detail, 2);
  button_group_->addButton(ui->pushButton_log, 3);
  button_group_->addButton(ui->pushButton_setting, 4);


  Initialize();
  TranslateLanguage();

  widget_home_    = new WidgetHomepage(this);
  widget_manual_control_ = new WidgetManualControl(this);
//  widget_setting_ = new WidgetSetting(this);
//  widget_switch_  = new WidgetSwitch(this);
  widget_error_   = new WidgetError(this);

  ui->stackedWidget->addWidget(widget_home_);
  ui->stackedWidget->addWidget(widget_manual_control_);
//  ui->stackedWidget->addWidget(widget_setting_);
  ui->stackedWidget->addWidget(widget_error_);

  MenuButtonClicked(0);

  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()), this, SLOT(ShowSystemTime()));
  timer_->start(5000);

  ui->stackedWidget->setCurrentIndex(0);
  connect(button_group_, SIGNAL(buttonClicked(int)), this, SLOT(MenuButtonClicked(int)));
  connect(widget_error_, SIGNAL(WidgetErrorBack()), this, SLOT(HideErrorDetialWidget()));
  connect(widget_home_, SIGNAL(ShowErrorDetailWidget()), this, SLOT(ShowErrorDetailWidget()));
}

WidgetMain::~WidgetMain()
{
  delete ui;
}

void WidgetMain::SetEventLoop(QEventLoop* loop)
{
    event_loop_ = loop;
}

void WidgetMain::SetTranslator(QTranslator *translator)
{
    translator_ = translator;
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

void WidgetMain::ShowSystemTime()
{
    QDateTime current_time = QDateTime::currentDateTime();
    QString time = current_time.toString("yyyy.MM.dd hh:mm");
    QStringList date = time.split(" ");
    if(date.empty())
    {
        return;
    }

    ui->label_date->setText(date[0]);
    ui->label_time->setText(date[1]);
}

void WidgetMain::MenuButtonClicked(int index)
{
  const auto& buttons = button_group_->buttons();
  for(int i = 0; i < buttons.count(); ++i)
  {
    if(index == i)
    {
      buttons[i]->setStyleSheet("QPushButton{background-color:rgb(227, 186, 56);color:rgb(57, 64, 75);border:none;}");
    }
    else
    {
      buttons[i]->setStyleSheet("QPushButton{background-color:rgb(75, 90, 114);color:rgb(255, 255, 255);border:none;}");
    }
  }

  if(index != 0 && index != 1)
  {
    return;
  }
  ui->stackedWidget->setCurrentIndex(index);
}

void WidgetMain::ShowErrorDetailWidget()
{
    ui->stackedWidget->setCurrentIndex(WIDGET_ERROR);
}

void WidgetMain::HideErrorDetialWidget()
{
    ui->stackedWidget->setCurrentIndex(WIDGET_HOME);
}

void WidgetMain::Initialize()
{
  QFont font = ui->label_title->font();
  font.setPixelSize(32);
  ui->label_title->setFont(font);
  ui->label_time->setFont(font);

  font.setPixelSize(24);
  ui->pushButton_home->setFont(font);
  ui->pushButton_control->setFont(font);
  ui->pushButton_detail->setFont(font);
  ui->pushButton_log->setFont(font);
  ui->pushButton_setting->setFont(font);

  font.setPixelSize(20);
  ui->label_date->setFont(font);

  ui->label_icon->setFixedSize(QSize(220, 30));
  ui->label_time->setFixedSize(QSize(100, 28));
  ui->label_date->setFixedSize(QSize(100, 28));
  ui->pushButton_home->setFixedSize(QSize(210, 56));
  ui->pushButton_control->setFixedSize(QSize(210, 56));
  ui->pushButton_detail->setFixedSize(QSize(210, 56));
  ui->pushButton_log->setFixedSize(QSize(210, 56));
  ui->pushButton_setting->setFixedSize(QSize(210, 56));
}

void WidgetMain::TranslateLanguage()
{
  ui->label_title->setText(tr("Dispatching platform of traction satellite system"));
  ui->pushButton_home->setText(tr("Home"));
  ui->pushButton_control->setText(tr("Manual control"));
  ui->pushButton_detail->setText(tr("Status Details"));
  ui->pushButton_log->setText(tr("Log"));
  ui->pushButton_setting->setText(tr("Setting"));
}

void WidgetMain::changeEvent(QEvent *e)
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
