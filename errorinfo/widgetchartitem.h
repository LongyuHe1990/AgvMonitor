#pragma once
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChart>
#include <common/global_helper.h>
#include <math.h>
#include <QtCharts/qsplineseries.h>
#include <QPaintEvent>

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
//  Q_OBJECT
public:
  explicit WidgetChartItem(QWidget* parent = nullptr);
  virtual ~WidgetChartItem();

  virtual void Clear() = 0;

private:
  // Override the widget event.
  virtual void paintEvent(QPaintEvent* e) override;

protected:
  bool    show_border_;
  QString show_bar_text_;
  QString show_title_text_;
  QChart* chart_;
};

class WidgetChartError : public WidgetChartItem
{
//  Q_OBJECT
public:
  explicit WidgetChartError(QWidget* parent = nullptr);
  virtual ~WidgetChartError();

  virtual void Clear() override;

  void SetInputData(StatisticsInfoList data);
  int GetTodayErrorNumber();

private:
  void Initialize();
  void TranslateLanguage();
  virtual void changeEvent(QEvent* e) override;

  QList<QLineSeries*> series_list_;
  QStringList list_;
  int curren_number_;
  QString current_week_;
};

