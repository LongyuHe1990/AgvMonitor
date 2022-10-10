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
class WidgetError;
}

class WidgetError : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetError(QWidget* parent = nullptr);
  ~WidgetError();

  static WidgetError * GetInstance();

Q_SIGNALS:
  void WidgetErrorBack();

private Q_SLOTS:
  void SearchButtonClicked();

private:
  void InitErrorTable();
  void Initialize();
  void TranslateLanguage();

private:
  Ui::WidgetError* ui;
};

