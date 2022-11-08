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

  widget_page_ = new WidgetPage(this);
  widget_page_->SetPageSize(8);

  ui->horizontalLayout_3->addWidget(widget_page_);

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

void WidgetError::InitData(QVariantMap dataMap)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if(dataMap.empty())
    {
        return;
    }

    QList<QString> keys = dataMap.keys();

    for(int i = 0; i < keys.size(); ++i)
    {
        QTableWidgetItem* item0 = new QTableWidgetItem;
        QTableWidgetItem* item1 = new QTableWidgetItem;
        QTableWidgetItem* item2 = new QTableWidgetItem;
        QTableWidgetItem* item3 = new QTableWidgetItem;
        QTableWidgetItem* item4 = new QTableWidgetItem;
        QTableWidgetItem* item5 = new QTableWidgetItem;

        QVariantMap item = dataMap.value(keys[i]).toMap();

        ui->tableWidget->insertRow(i);
        item0->setText(item.value("AlarmModule").toString());
        item1->setText(item.value("AlarmSource").toString());
        item2->setText(QString::number(item.value("AlarmCode").toInt()));
        item3->setText(item.value("AlarmLevel").toString());
        item4->setText(item.value("AlarmDetail").toString());
        item5->setText(item.value("AlarmTip").toString());

        ui->tableWidget->setItem(i, 0, item0);
        ui->tableWidget->setItem(i, 1, item1);
        ui->tableWidget->setItem(i, 2, item2);
        ui->tableWidget->setItem(i, 3, item3);
        ui->tableWidget->setItem(i, 4, item4);
        ui->tableWidget->setItem(i, 5, item5);

        QWidget* widget1 = new QWidget(ui->tableWidget);
        widget1->setMinimumWidth(150);
        QHBoxLayout* layout = new QHBoxLayout;
        layout->setContentsMargins(0, 0, 0, 0);

        item0->setTextAlignment(Qt::AlignCenter);
        item1->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item3->setTextAlignment(Qt::AlignCenter);
        item4->setTextAlignment(Qt::AlignCenter);
        item5->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setCellWidget(i, 5, widget1);
        ui->tableWidget->setRowHeight(i, 24);
    }
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
                                   QTableWidget{ color:rgb(255,255,255); background-color:rgba(255, 255, 255, 0); border:none; }");
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

void WidgetError::changeEvent(QEvent *e)
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
