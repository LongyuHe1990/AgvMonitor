/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include <QButtonGroup>
#include "switch/widgetcode.h"

namespace Ui {
class WidgetSwitch;
}

class WidgetSwitch : public QWidget
{
    Q_OBJECT

public:
  explicit WidgetSwitch(QWidget* parent = nullptr);
  ~WidgetSwitch();

private:
  void Initialize();
  void TranslateLanguage();

Q_SIGNALS:
  void AdminModel();

private Q_SLOTS:
  void NumberButtonClicked(int num);
  void ConfirmButtonClicked();

private:
  Ui::WidgetSwitch* ui;

  QButtonGroup* group_;
  WidgetCode*   widget_code_;
};

