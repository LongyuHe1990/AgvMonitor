#include "widgetswitch.h"
#include "ui_widgetswitch.h"

WidgetSwitch::WidgetSwitch(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetSwitch)
{
  ui->setupUi(this);
  setWindowModality(Qt::ApplicationModal);
  setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);

  group_ = new QButtonGroup(this);
  group_->addButton(ui->pushButton_12, 0);
  group_->addButton(ui->pushButton_2, 1);
  group_->addButton(ui->pushButton_3, 2);
  group_->addButton(ui->pushButton_4, 3);
  group_->addButton(ui->pushButton_5, 4);
  group_->addButton(ui->pushButton_6, 5);
  group_->addButton(ui->pushButton_7, 6);
  group_->addButton(ui->pushButton_8, 7);
  group_->addButton(ui->pushButton_9, 8);
  group_->addButton(ui->pushButton_10, 9);

  widget_code_ = new WidgetCode(this);
  ui->horizontalLayout_3->addWidget(widget_code_);

  Initialize();
  TranslateLanguage();

  connect(group_, SIGNAL(buttonClicked(int)), this, SLOT(NumberButtonClicked(int)));
  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(ui->pushButton_13, SIGNAL(clicked()), this, SLOT(ConfirmButtonClicked()));
}

WidgetSwitch::~WidgetSwitch()
{
  delete ui;
}

void WidgetSwitch::Initialize()
{
  QFont font = ui->label_title->font();
  font.setPixelSize(24);
  ui->label_title->setFont(font);
  ui->pushButton->setFont(font);
  ui->pushButton_2->setFont(font);
  ui->pushButton_3->setFont(font);
  ui->pushButton_4->setFont(font);
  ui->pushButton_5->setFont(font);
  ui->pushButton_6->setFont(font);
  ui->pushButton_7->setFont(font);
  ui->pushButton_8->setFont(font);
  ui->pushButton_9->setFont(font);
  ui->pushButton_10->setFont(font);
  ui->pushButton_11->setFont(font);
  ui->pushButton_12->setFont(font);
  ui->pushButton_13->setFont(font);

  font.setPixelSize(12);
  ui->label_tip->setFont(font);

  ui->pushButton->setFixedSize(QSize(43, 43));
}

void WidgetSwitch::TranslateLanguage()
{
  ui->label_title->setText("安全码验证");
  ui->label_tip->setText("*请输入正确的安全码");
  ui->pushButton_12->setText("0");
  ui->pushButton_2->setText("1");
  ui->pushButton_3->setText("2");
  ui->pushButton_4->setText("3");
  ui->pushButton_5->setText("4");
  ui->pushButton_6->setText("5");
  ui->pushButton_7->setText("6");
  ui->pushButton_8->setText("7");
  ui->pushButton_9->setText("8");
  ui->pushButton_10->setText("9");
  ui->pushButton_11->setText("删除");
  ui->pushButton_13->setText("确定");
}

void WidgetSwitch::NumberButtonClicked(int num)
{
}

void WidgetSwitch::ConfirmButtonClicked()
{
  //

  emit AdminModel();
  close();
}
