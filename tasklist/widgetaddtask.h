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

class WidgetTargetListItem : public QWidget
{
  Q_OBJECT
public:
  explicit WidgetTargetListItem(QWidget* parent = nullptr);
  ~WidgetTargetListItem();
  void SetInput(const QString& charge_label, const QString& status_label);

private:
  void SetupUi();

private:
  QLabel* charge_label_;
  QLabel* status_label_;
  QFrame* frame_;
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

Q_SIGNALS:
  void HideAddTaskListWidget();

private Q_SLOTS:
  void AddButtonClicked();
  void CreateButtonClicked();

private:
  void Initialize();
  void TranslateLanguage();

private:
  Ui::WidgetAddTask* ui;

  QList<WidgetTargetListItem *> items_;
};

