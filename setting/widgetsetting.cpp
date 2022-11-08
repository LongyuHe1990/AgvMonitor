#include "widgetsetting.h"
#include "ui_widgetsetting.h"
#include <QScrollBar>

static WidgetSetting* widget_setting_ = nullptr;

WidgetSetting::WidgetSetting(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetSetting)
{
  ui->setupUi(this);

  widget_setting_ = this;

  InitLogTable();
  Initialize();
  TranslateLanguage();

  connect(ui->pushButton_qiehuan, SIGNAL(clicked()), this, SLOT(SwitchButtonClicked()));
  connect(ui->pushButton_edit, SIGNAL(clicked()), this, SLOT(EditButtonClicked()));
  connect(ui->checkBox_2, SIGNAL(clicked()), this, SLOT(AutoLoginButtonClicked()));
}

WidgetSetting::~WidgetSetting()
{
  delete ui;
}

WidgetSetting * WidgetSetting::GetInstance()
{
  return widget_setting_;
}

void WidgetSetting::InitData(QVariantMap dataMap)
{
  if(dataMap.isEmpty())
  {
    return;
  }

  ui->tableWidget->clearContents();
  ui->tableWidget->setRowCount(0);
}

void WidgetSetting::SwitchButtonClicked()
{
  emit SwitchVisitor();
}

void WidgetSetting::EditButtonClicked()
{
}

void WidgetSetting::AutoLoginButtonClicked()
{
}

void WidgetSetting::InitLogTable()
{
  ui->tableWidget->setColumnCount(4);
  ui->tableWidget->horizontalHeader()->setDefaultSectionSize(150);
  ui->tableWidget->setColumnWidth(0, 130);
  ui->tableWidget->setColumnWidth(1, 100);
  ui->tableWidget->setColumnWidth(2, 100);
  ui->tableWidget->setColumnWidth(3, 100);

  // 设置表头字体加粗
  QFont font = ui->tableWidget->horizontalHeader()->font();
  font.setPixelSize(11);
  ui->tableWidget->horizontalHeader()->setFont(font);

  font = ui->tableWidget->font();
  font.setPixelSize(12);
  ui->tableWidget->setFont(font);

  ui->tableWidget->horizontalHeader()->setStretchLastSection(true);                   // 设置充满表宽度
  ui->tableWidget->setShowGrid(false);                                                // 设置不显示格子线
  ui->tableWidget->verticalHeader()->setVisible(false);
  ui->tableWidget->setStyleSheet(
    " \
                                 QTableWidget::item{ border:none; border-top:0.5px solid rgb(255,255,255);} \
                                 QTableWidget::item:selected { background:transparent; color:rgb(240,179,28);} \
                                 QTableWidget{ color:rgb(255,255,255); background-color:rgb(15, 24, 25); border:none; }");
  ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{ color:rgb(255, 255, 255);background-color: rgb(15, 24, 25); border:none; }");
  ui->tableWidget->verticalScrollBar()->setStyleSheet(
    "QScrollBar{ background:#0F1819; width:4px; padding-top:0px; padding-bottom:0px; } \
                                                      QScrollBar::handle:vertical{ border-radius:4px; background:#5D6068; min-height: 30px; } \
                                                      QScrollBar::handle:vertical::disabled{ background:#DBDBDB; } \
                                                      QScrollBar::handle:vertical:hover{ background:#5D6068; } \
                                                      QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none; } \
                                                      QScrollBar::add-page, QScrollBar::sub-page{ background:none; }");
  ui->tableWidget->setFocusPolicy(Qt::NoFocus);
  ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void WidgetSetting::Initialize()
{
  QFont font = ui->label_set->font();
  font.setPixelSize(18);
  ui->label_set->setFont(font);
  ui->label_id->setFont(font);
  ui->label_ip->setFont(font);
  ui->label_log->setFont(font);
  ui->label_safeCode->setFont(font);
  ui->label_autoLogin->setFont(font);
  ui->lineEdit_id->setFont(font);
  ui->lineEdit_ip->setFont(font);
  ui->lineEdit_safeCode->setFont(font);
  ui->pushButton_edit->setFont(font);
  ui->pushButton_qiehuan->setFont(font);

  font.setPixelSize(12);
  ui->label_tip->setFont(font);

  ui->pushButton_edit->setFixedSize(QSize(320, 42));
  ui->pushButton_qiehuan->setFixedSize(QSize(160, 42));
  ui->label_touxiang->setFixedSize(QSize(80, 80));
}

void WidgetSetting::TranslateLanguage()
{
  ui->label_set->setText(tr("Setting"));
  ui->label_id->setText(tr("Vehicle ID"));
  ui->label_ip->setText(tr("IP address"));
  ui->label_autoLogin->setText(tr("Automatic login"));
  ui->label_tip->setText(tr("End automatic login after deselecting"));
  ui->label_safeCode->setText(tr("Security code change"));
  ui->pushButton_qiehuan->setText(tr("Switch visitor interface"));
  ui->pushButton_edit->setText(tr("Edit"));
  ui->label_log->setText(tr("Operation log"));

  QStringList header;
  header << tr("Number") << tr("Date") << tr("Order") << tr("Operation");
  for(int i = 0; i < header.size(); ++i)
  {
    QTableWidgetItem* item = new QTableWidgetItem;
    ui->tableWidget->setHorizontalHeaderItem(i, item);
    item->setText(header[i]);
    item->setTextAlignment(Qt::AlignCenter);
  }
}

void WidgetSetting::changeEvent(QEvent *e)
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
