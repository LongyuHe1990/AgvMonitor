#pragma once

#include <QWidget>
#include <QChartView>
#include <QtWidgets/QGraphicsView>
#include <qchart.h>
#include <common/global_helper.h>
#include <math.h>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QPieSeries;
class QLineSeries;
class QBarSeries;
class QSplineSeries;
QT_CHARTS_END_NAMESPACE

  QT_CHARTS_USE_NAMESPACE
class WidgetChartItem : public QChartView
{
  Q_OBJECT
public:
  explicit WidgetChartItem(QWidget* parent = nullptr);
  virtual ~WidgetChartItem();

  virtual void Clear() = 0;

private:
  /*!
   * Override the widget event.
   */
  virtual void paintEvent(QPaintEvent* e) override;

protected:
  bool    show_border_;
  QString show_bar_text_;
  QString show_title_text_;
  QChart* chart_;
};

class WidgetChartError : WidgetChartItem
{
  Q_OBJECT
public:
  explicit WidgetChartError(QWidget* parent = nullptr);
  virtual ~WidgetChartError();

  virtual void Clear() override;

  void SetInputData(StatisticsInfoList data);


private:
  void Initialize();
  void TranslateLanguage();

  QSplineSeries* series_;
};
