
#include "widgetchartitem.h"
#include <QtCharts/qcategoryaxis.h>
#include <QtCharts/qbarcategoryaxis.h>
#include <QMetaObject>
#include <QDebug>

#pragma comment  (lib, "Qt5Charts.lib")

WidgetChartItem::WidgetChartItem(QWidget* parent)
  : QChartView(parent)
  , show_border_(false)
  , show_bar_text_()
  , show_title_text_()
{
  chart_ = new QChart;
  chart_->setTheme(QChart::ChartThemeLight);
  chart_->setBackgroundBrush(QColor(7, 22, 41));

  chart_->legend()->setVisible(true);
  chart_->legend()->setAlignment(Qt::AlignRight);
  chart_->legend()->setLabelColor(QColor("#FFFFFF"));

  setRenderHint(QPainter::Antialiasing);
  setChart(chart_);
}

WidgetChartItem::~WidgetChartItem()
{
  delete chart_;
}

void WidgetChartItem::paintEvent(QPaintEvent* e)
{
  {
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(7, 22, 41));
  }

  QChartView::paintEvent(e);

  if(!show_bar_text_.isEmpty())
  {
    QPainter painter(viewport());
    painter.setPen(QColor("#FFFFFF"));
    painter.setRenderHint(QPainter::Antialiasing);

    //
    QFont ft = painter.font();
    ft.setPixelSize(10);
    painter.setFont(ft);
    painter.drawText(20, 20, show_bar_text_);
  }

  if(!show_title_text_.isEmpty())
  {
    QPainter painter(viewport());
    painter.setPen(QColor("#FFFFFF"));
    painter.setRenderHint(QPainter::Antialiasing);

    //
    QFont ft = painter.font();
    ft.setPixelSize(10);
    painter.setFont(ft);
    const int text_height = QFontMetrics(ft).height() + chart_->margins().bottom();
    painter.drawText(0, height() - text_height, width(), text_height, Qt::AlignCenter, show_title_text_);
  }

  if(show_border_)
  {
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
  }
}

WidgetChartError::WidgetChartError(QWidget* parent)
  : WidgetChartItem(parent)
{
  QLineSeries* series = new QLineSeries(chart_);
  series->setPen(QPen(QColor("#E3BA38"), 2));
  series->setPointLabelsColor(QColor("#E3BA38"));
  series->setPointLabelsFormat("@yPoint");
  series->setPointLabelsVisible(true);
  series->setPointsVisible(true);
  series->setPointLabelsClipping(false);
  series->setName(tr("本周"));

  QLineSeries* series1 = new QLineSeries(chart_);
  series1->setPen(QPen(QColor("#E6F7FF"), 2));
  series1->setPointLabelsColor(QColor("#E6F7FF"));
  series1->setPointLabelsFormat("@yPoint");
  series1->setPointLabelsVisible(true);
  series1->setPointsVisible(true);
  series1->setPointLabelsClipping(false);
  series1->setName(tr("过去一周"));

  series_list_.append(series);
  series_list_.append(series1);
  chart_->addSeries(series_list_[0]);
  chart_->addSeries(series_list_[1]);

  QCategoryAxis* axis_x = new QCategoryAxis(chart_);
  axis_x->setLabelsColor(QColor("#E6F7FF"));
  axis_x->setGridLineVisible(false);
  axis_x->setLinePen(QPen(QColor("#E6F7FF"), 1));
  axis_x->setRange(0, 14);

  QValueAxis* axis_y = new QValueAxis(chart_);
  axis_y->setTickCount(6);
  axis_y->setRange(0, 50);
  axis_y->setGridLinePen(QPen(QColor("#E6F7FF"), 0.25));
  axis_y->setLinePen(QPen(QColor("#E6F7FF"), 1));
  axis_y->setLabelsColor(QColor("#E6F7FF"));
  axis_y->setLabelFormat("%d");

  chart_->setAxisX(axis_x, series_list_[0]);
  chart_->setAxisY(axis_y, series_list_[0]);
  chart_->setAxisX(axis_x, series_list_[1]);
  chart_->setAxisY(axis_y, series_list_[1]);
  chart_->legend()->setVisible(true);
  chart_->legend()->setAlignment(Qt::AlignTop);


  Initialize();
  TranslateLanguage();
}

WidgetChartError::~WidgetChartError()
{
}

void WidgetChartError::Clear()
{
  QCategoryAxis* axis_x = static_cast<QCategoryAxis *>(chart_->axisX());
  if(!axis_x)
  {
    return;
  }

  const QList<QLineSeries *> series_list = series_list_;
  if(series_list.count() < 1)
  {
    return;
  }
  series_list.at(0)->clear();
  series_list.at(1)->clear();

  const QStringList labels(axis_x->categoriesLabels());
  for(const QString& label : labels)
  {
    axis_x->remove(label);
  }
}

void WidgetChartError::SetInputData(StatisticsInfoList data)
{
  Clear();
  // Set max value
  int max_value = 0;
  for(auto itr = data.begin(); itr != data.end(); ++itr)
  {
    StatisticsInfo info = *itr;
    if(info.Max() > max_value)
    {
      max_value = info.Max();
    }
  }

  QValueAxis* axis_y = static_cast<QValueAxis *>(chart_->axisY());
  if(axis_y)
  {
    axis_y->setMax(int(std::floor(max_value / 10.0)) * 10 + 10);
  }
  //

  for(auto itr = data.begin(); itr != data.end(); ++itr)
  {
    StatisticsInfo info   = *itr;
    QCategoryAxis* axis_x = static_cast<QCategoryAxis *>(chart_->axisX());
    const int      offset = (axis_x->count() + 1) * 2;
    if(axis_x)
    {
      axis_x->append(info.key.mid(5), offset);
    }

    const QList<QLineSeries *> serise_list = series_list_;
    if(serise_list.count() < 1)
    {
      return;
    }

    serise_list.at(0)->append(QPointF(offset - 1, info.value));
    serise_list.at(1)->append(QPointF(offset - 1, info.value1));
  }
}

void WidgetChartError::Initialize()
{
  QFont ft = font();
  ft.setPixelSize(10);

  QBarCategoryAxis* axis_x = static_cast<QBarCategoryAxis *>(chart_->axisX());
  if(axis_x)
  {
    axis_x->setLabelsFont(ft);
  }

  QValueAxis* axis_y = static_cast<QValueAxis *>(chart_->axisY());
  if(axis_y)
  {
    axis_y->setLabelsFont(ft);
  }
}

void WidgetChartError::TranslateLanguage()
{
    show_bar_text_ = tr("单位：次");
}

void WidgetChartError::changeEvent(QEvent *e)
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

