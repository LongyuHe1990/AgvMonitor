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
#include <QTableView>
#include <QStandardItemModel>

namespace Ui {
class WidgetAddTask;
}

struct TargetListInfo
{
  int action;
  int agvBufferIndex;
  int stationBufferIndex;
  int stationId;
  QString actionName;
  QString stationName;
};

class WidgetAddTask : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetAddTask(QWidget* parent = nullptr);
  ~WidgetAddTask();

  static WidgetAddTask * GetIntance();
  void                   InitTargetList();
  void                   SetInitData();

Q_SIGNALS:
  void HideAddTaskListWidget();

private Q_SLOTS:
  void AddButtonClicked();
  void CreateButtonClicked();
  void StationTypeChanged(int index);
  void StationRowChanged(int index);
  void TargetStationChanged(int index);
  void TableViewMenu(const QPoint &pos);
  void DeleteMenuTriggered();
  void ClearMenuTriggered();

private:
  void Initialize();
  void TranslateLanguage();
  void InitTargetListTableView();
  virtual void changeEvent(QEvent* e) override;

private:
  Ui::WidgetAddTask* ui;

  QStandardItemModel* model_;
  int                 count_;
};
Q_DECLARE_METATYPE(TargetListInfo)
