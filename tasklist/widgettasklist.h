/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>

namespace Ui {
class WidgetTaskList;
}

class WidgetTaskList : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetTaskList(QWidget* parent = nullptr);
  ~WidgetTaskList();

  static WidgetTaskList* GetInstance();
  void InitData(QVariantMap dataMap);
  void VisitorModel(bool model);

Q_SIGNALS:
  void ShowAddTaskListWidget();
private Q_SLOTS:
  void AddTaskButtonClicked();
  void TaskCancelButtonClicked();
  void TaskDeleteButtonClicked();
private:
  void Initialize();
  void TranslateLanguage();
  void InitTasklistTable();
  virtual void changeEvent(QEvent* e) override;

private:
  Ui::WidgetTaskList* ui;
};
