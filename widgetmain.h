﻿/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include "widgethomepage.h"
#include "setting/widgetsetting.h"
#include "switch/widgetswitch.h"
#include "errorinfo/widgeterror.h"
#include <QEventLoop>
#include <QButtonGroup>

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetMain(QWidget* parent = nullptr);
  virtual ~WidgetMain();

  void SetEventLoop(QEventLoop* loop);

private:
  void keyPressEvent(QKeyEvent* event) override;

private Q_SLOTS:
  void ShowSystemTime();
  void MenuButtonClicked(int index);

private:
  void Initialize();
  void TranslateLanguage();

private:
  Ui::WidgetMain* ui;

  QEventLoop*     event_loop_;
  WidgetHomepage* widget_home_;
  WidgetSetting*  widget_setting_;
  WidgetSwitch*   widget_switch_;
  WidgetError*    widget_error_;
  QButtonGroup*   button_group_;
  QTimer*         timer_;
};

