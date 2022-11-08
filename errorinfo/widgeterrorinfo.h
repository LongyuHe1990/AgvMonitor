/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include "errorinfo/widgetchartitem.h"
#include <QVariantMap>

namespace Ui {
class WidgetErrorInfo;
}

class WidgetErrorInfo : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetErrorInfo(QWidget* parent = nullptr);
  ~WidgetErrorInfo();

  static WidgetErrorInfo* GetInstance();
  void InitData(QVariantMap data_map);

Q_SIGNALS:
  void ShowDetailWidget();

private:
  void Initialize();
  void TranslateLanguage();
  virtual void changeEvent(QEvent* e) override;

private:
  Ui::WidgetErrorInfo* ui;

};

