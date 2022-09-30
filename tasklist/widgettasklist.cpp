#include "widgettasklist.h"
#include "ui_widgettasklist.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QScrollBar>
#include "tasklist/widgetaddtask.h"

static WidgetTaskList* widget_tasklist_ = nullptr;

WidgetTaskList::WidgetTaskList(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetTaskList)
{
  ui->setupUi(this);

  InitTasklistTable();
  Initialize();
  Translatelanguage();

  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(AddTaskButtonClicked()));

  widget_tasklist_ = this;
}

WidgetTaskList::~WidgetTaskList()
{
  delete ui;
}

WidgetTaskList * WidgetTaskList::GetInstance()
{
  return widget_tasklist_;
}

void WidgetTaskList::InitData(QVariantMap dataMap)
{
  qDebug() << "--------------------------------------";
  QJsonObject   obj = QJsonObject::fromVariantMap(dataMap);
  QJsonDocument doc(obj);
  QString       strRet = QString(doc.toJson(QJsonDocument::Indented));

  qDebug() << strRet;

  if(dataMap.isEmpty())
  {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    return;
  }

  ui->tableWidget->clearContents();
  ui->tableWidget->setRowCount(0);

  QList<QString> keys = dataMap.keys();

  qDebug() << keys.size();

  int row = 0;
  for(int i = 0; i < keys.size(); ++i)
  {
    QTableWidgetItem* item0 = new QTableWidgetItem;
    QTableWidgetItem* item1 = new QTableWidgetItem;
    QTableWidgetItem* item2 = new QTableWidgetItem;
    QTableWidgetItem* item3 = new QTableWidgetItem;
    QTableWidgetItem* item4 = new QTableWidgetItem;

    QVariantMap item = dataMap.value(keys[i]).toMap();

    ui->tableWidget->insertRow(row);
    item0->setText(QString::number(item.value("id").toInt()));
    item1->setText(item.value("agvName").toString());
    item2->setText(item.value("taskPeriod").toString());
    item3->setText(item.value("targetInfo").toString());

    ui->tableWidget->setItem(row, 0, item0);
    ui->tableWidget->setItem(row, 1, item1);
    ui->tableWidget->setItem(row, 2, item2);
    ui->tableWidget->setItem(row, 3, item3);

    QPushButton* btn_cancel = new QPushButton(ui->tableWidget);
    btn_cancel->setText(tr("取消"));
    btn_cancel->setCursor(QCursor(Qt::PointingHandCursor));
    btn_cancel->setFocusPolicy(Qt::NoFocus);
    btn_cancel->setFixedSize(QSize(43, 24));

    btn_cancel->setStyleSheet(
      "QPushButton::hover{background-color:rgb(227, 186, 56);color:rgb(0,0,0);border:1px solid #D9E7FF;border-radius:4px;} \
       QPushButton{background-color:rgba(7, 22, 41, 0.7);color:rgb(217,231,255);border:1px solid #D9E7FF;border-radius:4px;} \
       QPushButton::pressed{background-color:rgb(227, 186, 56);color:rgb(0,0,0);border:1px solid #D9E7FF;border-radius:4px;}");
    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(TaskCancelButtonClicked()));

    QPushButton* btn_delete = new QPushButton(ui->tableWidget);
    btn_delete->setText(tr("强制删除"));
    btn_delete->setCursor(QCursor(Qt::PointingHandCursor));
    btn_delete->setFocusPolicy(Qt::NoFocus);
    btn_delete->setFixedSize(QSize(43, 24));

    btn_delete->setStyleSheet(
       "QPushButton::hover{background-color:rgb(227, 186, 56);color:rgb(0,0,0);border:1px solid #D9E7FF;border-radius:4px;} \
       QPushButton{background-color:rgba(7, 22, 41, 0.7);color:rgb(217,231,255);border:1px solid #D9E7FF;border-radius:4px;} \
       QPushButton::pressed{background-color:rgb(227, 186, 56);color:rgb(0,0,0);border:1px solid #D9E7FF;border-radius:4px;}");
   connect(btn_delete, SIGNAL(clicked()), this, SLOT(TaskDeleteButtonClicked()));


    QWidget * widget1 = new QWidget(ui->tableWidget);
    widget1->setMinimumWidth(150);
    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);

    layout->addWidget(btn_cancel);
    layout->addWidget(btn_delete);
    widget1->setLayout(layout);

    item0->setTextAlignment(Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setCellWidget(row, 4, widget1);
    ui->tableWidget->setRowHeight(row, 24);

    ++row;
  }
}

void WidgetTaskList::VisitorModel(bool model)
{
    ui->pushButton_4->setHidden(model);
}

void WidgetTaskList::AddTaskButtonClicked()
{
   emit ShowAddTaskListWidget();
    WidgetAddTask::GetIntance()->SetInitData();
}

void WidgetTaskList::TaskCancelButtonClicked()
{

}

void WidgetTaskList::TaskDeleteButtonClicked()
{

}

void WidgetTaskList::Initialize()
{
  QFont font = ui->label_23->font();
  font.setPixelSize(16);
  ui->label_23->setFont(font);

  font.setPixelSize(12);
  ui->pushButton_4->setFont(font);

  ui->pushButton_4->setFixedSize(QSize(70, 24));
  ui->pushButton_4->setIconSize(QSize(12, 12));
}

void WidgetTaskList::Translatelanguage()
{
  ui->label_23->setText(tr("任务信息列表"));
  ui->pushButton_4->setText(tr("添加"));

  QStringList header;
  header << tr("ID") << tr("车号") << tr("阶段") << tr("目标列表") << tr("操作");
  for(int i = 0; i < header.size(); ++i)
  {
    QTableWidgetItem* item = new QTableWidgetItem;
    ui->tableWidget->setHorizontalHeaderItem(i, item);
    item->setText(header[i]);
    item->setTextAlignment(Qt::AlignCenter);
  }
}

void WidgetTaskList::InitTasklistTable()
{
  const int offset = 1;
  ui->tableWidget->setColumnCount(5);
  ui->tableWidget->horizontalHeader()->setDefaultSectionSize(50);
  ui->tableWidget->setColumnWidth(0, 50 * offset);
  ui->tableWidget->setColumnWidth(1, 100 * offset);
  ui->tableWidget->setColumnWidth(2, 100 * offset);
  ui->tableWidget->setColumnWidth(3, 100 * offset);
  ui->tableWidget->setColumnWidth(4, 100 * offset);

  // 设置表头字体加粗
  QFont font = ui->tableWidget->horizontalHeader()->font();
  font.setPixelSize(14);
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
  ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{ color:rgb(227, 186, 56);background-color: rgb(15, 24, 25); border:none; }");
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
