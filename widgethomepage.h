/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include "baseinfo/widgetbaseinfo.h"
#include "errorinfo/widgeterrorinfo.h"
#include "tasklist/widgettasklist.h"
#include "allinfo/widgetallinfo.h"
#include "tasklist/widgetaddtask.h"
#include "mapView/mapMonitoringView.h"
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"
#include "moduleData/taskModule.h"

namespace Ui {
class WidgetHomepage;
}

class WidgetHomepage : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetHomepage(QWidget* parent = nullptr);
  ~WidgetHomepage();

Q_SIGNALS:
  void ShowErrorDetailWidget();

private Q_SLOTS:
  void ShowAddTaskListWidget();
  void HideAddTaskListWidget();

private:
  void Initialize();

private:
  Ui::WidgetHomepage* ui;

  WidgetBaseInfo*  widget_baseinfo_;
  WidgetTaskList*  widget_tasklist_;
//  WidgetAllInfo*   widget_allinfo_;
//  WidgetAddTask*   widget_addtask_;
  WidgetErrorInfo* Widget_error_;
  ConfigModule*    m_configModule;
  StationModule*   m_stationModule;
  TaskModule*      m_taskModule;
};

