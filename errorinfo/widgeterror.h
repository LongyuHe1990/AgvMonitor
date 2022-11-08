/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include "common/widgetpage.h"

namespace Ui {
class WidgetError;
}

class WidgetError : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetError(QWidget* parent = nullptr);
  ~WidgetError();

  static WidgetError * GetInstance();
  void InitData(QVariantMap dataMap);

Q_SIGNALS:
  void WidgetErrorBack();

private Q_SLOTS:
  void SearchButtonClicked();

private:
  void InitErrorTable();
  void Initialize();
  void TranslateLanguage();
  virtual void changeEvent(QEvent* e) override;

private:
  Ui::WidgetError* ui;

  WidgetPage* widget_page_;
};

