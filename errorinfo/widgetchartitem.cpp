#include "widgetchartitem.h"
#include <QtCharts/qcategoryaxis.h>
#include <QtCharts/qbarcategoryaxis.h>
#include <QtCharts/qsplineseries.h>

WidgetChartItem::WidgetChartItem(QWidget* parent)
  : QChartView(parent)
  , show_border_(false)
  , show_bar_text_()
  , show_title_text_()
{
  chart_ = new QChart;
  chart_->setTheme(QChart::ChartThemeLight);
  chart_->setBackgroundBrush(QColor(15, 24, 25));

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
    painter.fillRect(rect(), QColor(15, 24, 25));
  }

  QChartView::paintEvent(e);

  if(!show_bar_text_.isEmpty())
  {
    QPainter painter(viewport());
    painter.setPen(QColor("#FFFFFF"));
    painter.setRenderHint(QPainter::Antialiasing);

    //
    QFont ft = painter.font();
    ft.setPixelSize(6);
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
    ft.setPixelSize(6);
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
  series_ = new QSplineSeries(chart_);
  series_->setPen(QPen(QColor("#3CC2FF"), 2));
  series_->setPointLabelsColor(QColor("#3CC2FF"));
  series_->setPointLabelsFormat("@yPoint");
  series_->setPointLabelsVisible(true);
  series_->setPointsVisible(true);
  series_->setPointLabelsClipping(false);
  chart_->addSeries(series_);

  QCategoryAxis* axis_x = new QCategoryAxis(chart_);
  axis_x->setLabelsColor(QColor("#3CDFFF"));
  axis_x->setGridLineVisible(false);
  axis_x->setLinePen(QPen(QColor("#3CC2FF"), 1));

  QValueAxis* axis_y = new QValueAxis(chart_);
  axis_y->setTickCount(6);
  axis_y->setRange(0, 50);
  axis_y->setGridLinePen(QPen(QColor("#3CC2FF"), 0.25));
  axis_y->setLinePen(QPen(QColor("#3CC2FF"), 1));
  axis_y->setLabelsColor(QColor("#3CDFFF"));
  axis_y->setLabelFormat("%d");

  chart_->setAxisX(axis_x, series_);
  chart_->setAxisY(axis_y, series_);
  chart_->legend()->setVisible(false);

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

  series_->clear();
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
    if(itr->value > max_value)
    {
      max_value = itr->value;
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
    QCategoryAxis* axis_x = static_cast<QCategoryAxis *>(chart_->axisX());
    const int      offset = (axis_x->count() + 1) * 2;
    if(axis_x)
    {
      axis_x->append(itr->key.mid(5), offset);
    }
    series_->append(QPointF(offset - 1, itr->value));
  }
}

void WidgetChartError::Initialize()
{
  QFont ft = font();
  ft.setPixelSize(6);

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
