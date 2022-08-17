#include "widgeterrorinfo.h"
#include "ui_widgeterrorinfo.h"


WidgetErrorInfo::WidgetErrorInfo(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetErrorInfo)
{
  ui->setupUi(this);

  Initialize();
  TranslateLanguage();

// chart_error_ = new WidgetChartError(this);
// ui->horizontalLayout_4->addWidget(chart_error_);
  ui->stackedWidget->setCurrentIndex(0);

  connect(ui->pushButton_3, SIGNAL(clicked()), this, SIGNAL(ShowDetailWidget()));
}

WidgetErrorInfo::~WidgetErrorInfo()
{
  delete ui;
}

void WidgetErrorInfo::InitData(QVariantMap data_map, int agvId)
{
  if(data_map.empty())
  {
    return;
  }
}

void WidgetErrorInfo::Initialize()
{
  QFont font = ui->label_10->font();
  font.setPixelSize(16);
  ui->label_10->setFont(font);
  ui->label_19->setFont(font);

  font.setPixelSize(12);
  ui->pushButton_3->setFont(font);
  ui->label_20->setFont(font);
  ui->label_22->setFont(font);

  font.setPixelSize(14);
  ui->label_21->setFont(font);

  ui->pushButton_3->setFixedSize(QSize(70, 24));
}

void WidgetErrorInfo::TranslateLanguage()
{
  ui->label_10->setText(tr("异常数据"));
  ui->label_20->setText(tr("已预警数据"));
  ui->label_22->setText(tr("历史预警数据"));
  ui->pushButton_3->setText(tr("详情"));
}
