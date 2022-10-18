#include "widgeterror.h"
#include "ui_widgeterror.h"
#include <QScrollBar>
#include <QDebug>

static WidgetError* widget_error_ = nullptr;

WidgetError::WidgetError(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetError)
{
  ui->setupUi(this);

  widget_error_ = this;

  Initialize();
  InitErrorTable();
  TranslateLanguage();

  ui->dateEdit->setDateTime(QDateTime::currentDateTime());
  ui->dateEdit_2->setDateTime(QDateTime::currentDateTime());

  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(SearchButtonClicked()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SIGNAL(WidgetErrorBack()));
}

WidgetError::~WidgetError()
{
  delete ui;
}

WidgetError * WidgetError::GetInstance()
{
  return widget_error_;
}

void WidgetError::SearchButtonClicked()
{
  const qint64 start_time = ui->dateEdit->dateTime().toMSecsSinceEpoch() / 1000;
  const qint64 end_time   = ui->dateEdit_2->dateTime().toMSecsSinceEpoch() / 1000;

  if(start_time > end_time)
  {
    qDebug() << "Log query end time cannot be earlier than start time";
    return;
  }
}

void WidgetError::InitErrorTable()
{
  const int offset = 1;
  ui->tableWidget->setColumnCount(6);
  ui->tableWidget->horizontalHeader()->setDefaultSectionSize(150);
  ui->tableWidget->setColumnWidth(0, 130 * offset);
  ui->tableWidget->setColumnWidth(1, 200 * offset);
  ui->tableWidget->setColumnWidth(2, 200 * offset);
  ui->tableWidget->setColumnWidth(3, 200 * offset);
  ui->tableWidget->setColumnWidth(4, 200 * offset);
  ui->tableWidget->setColumnWidth(5, 200 * offset);

  // 设置表头字体加粗
  QFont font = ui->tableWidget->horizontalHeader()->font();
  font.setPixelSize(18);
  ui->tableWidget->horizontalHeader()->setFont(font);

  font = ui->tableWidget->font();
  font.setPixelSize(14);
  ui->tableWidget->setFont(font);

  ui->tableWidget->horizontalHeader()->setStretchLastSection(true);                     // 设置充满表宽度
  ui->tableWidget->setShowGrid(false);                                                  // 设置不显示格子线
  ui->tableWidget->verticalHeader()->setVisible(false);
  ui->tableWidget->setStyleSheet(
    " \
                                   QTableWidget::item{ border:none; border-top:0.5px solid rgb(255,255,255);} \
                                   QTableWidget::item:selected { background:transparent; color:rgb(240,179,28);} \
                                   QTableWidget{ color:rgb(255,255,255); background-color:rgb(7, 17, 30); border:none; }");
  ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{ color:rgb(227, 186, 56);background-color: rgb(49, 62, 84); border:none; }");
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

void WidgetError::Initialize()
{
  QFont font = ui->label->font();
  font.setPixelSize(18);
  ui->label->setFont(font);

  font.setPixelSize(16);
  ui->pushButton_2->setFont(font);

  font.setPixelSize(14);
  ui->pushButton->setFont(font);

  ui->label_2->setFixedSize(QSize(18, 18));
}

void WidgetError::TranslateLanguage()
{
  ui->label->setText(tr("Exception information list"));
  ui->pushButton->setText(tr("Search"));
  ui->pushButton_2->setText(tr("Back"));

  QStringList header;
  header << tr("Modular") << tr("Source") << tr("Exception code") << tr("Level") << tr("Describe") << tr("Tips");
  for(int i = 0; i < header.size(); ++i)
  {
    QTableWidgetItem* item = new QTableWidgetItem;
    ui->tableWidget->setHorizontalHeaderItem(i, item);
    item->setText(header[i]);
    item->setTextAlignment(Qt::AlignCenter);
  }
}
