#include "widgeterrorinfo.h"
#include "ui_widgeterrorinfo.h"
#include "common/global_helper.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"

static WidgetErrorInfo* widget_error_ = nullptr;

WidgetErrorInfo::WidgetErrorInfo(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetErrorInfo)
{
  ui->setupUi(this);

  Initialize();
  TranslateLanguage();

  ui->stackedWidget->setCurrentIndex(1);

  connect(ui->pushButton_3, SIGNAL(clicked()), this, SIGNAL(ShowDetailWidget()));
  widget_error_ = this;
}

WidgetErrorInfo::~WidgetErrorInfo()
{
  delete ui;
}

WidgetErrorInfo * WidgetErrorInfo::GetInstance()
{
  return widget_error_;
}

void WidgetErrorInfo::InitData(QVariantMap data_map)
{
  if(data_map.empty())
  {
    return;
  }

  QVariantMap module = data_map.value("ModuleData").toMap();
  if(module.isEmpty())
  {
    return;
  }

  int operation = module.value("OperationType").toInt();
  if(static_cast<AgvOperationType>(operation) != AgvOperationType::AGV_STATUS_UPDATED)
  {
    return;
  }

  StatisticsInfoList list;
  for(int i = 10; i < 17; ++i)
  {
    StatisticsInfo info;
    info.key   = QString("2022/09/%1").arg(i);
    info.value = i;
    info.value1 = i + 5;
    list.push_back(info);
  }

  ui->widgetchart->SetInputData(list);
}

void WidgetErrorInfo::Initialize()
{
  QFont font = ui->label_10->font();
  font.setPixelSize(16);

  ui->label_19->setFont(font);

  font.setPixelSize(12);
  ui->pushButton_3->setFont(font);
  ui->label_20->setFont(font);
  ui->label_10->setFont(font);

  ui->pushButton_3->setFixedSize(QSize(70, 24));
}

void WidgetErrorInfo::TranslateLanguage()
{
  ui->label_10->setText(tr("Abnormal alarm"));
  ui->label_20->setText(tr("Today's alert data"));
  ui->pushButton_3->setText(tr("Details"));
}
