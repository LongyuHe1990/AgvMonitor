/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include <QLabel>

namespace Ui {
class WidgetAddTask;
}

struct TargetListInfo
{
    int action;
    int agvAxisId;
    int stationAxisId;
    QString stationId;
    QString actionName;
    QString stationName;
};

class WidgetTargetListItem : public QWidget
{
  Q_OBJECT
public:
  explicit WidgetTargetListItem(QWidget* parent = nullptr);
  ~WidgetTargetListItem();
  void SetInput(TargetListInfo info);
  TargetListInfo GetInput();

private:
  void SetupUi();

private:
  QLabel* station_label_;
  QLabel* action_label_;
  QFrame* frame_;

  TargetListInfo info_;
};

class WidgetAddTask : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetAddTask(QWidget* parent = nullptr);
  ~WidgetAddTask();

  static WidgetAddTask * GetIntance();
  void                   InitTargetList();
  void                   GetTargetListInfo();
  void                   SetInitData();

Q_SIGNALS:
  void HideAddTaskListWidget();

private Q_SLOTS:
  void AddButtonClicked();
  void CreateButtonClicked();
  void StationTypeChanged(int index);
  void StationRowChanged(int index);

private:
  void Initialize();
  void TranslateLanguage();

private:
  Ui::WidgetAddTask* ui;

  QList<WidgetTargetListItem *> items_;
};

