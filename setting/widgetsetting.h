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
class WidgetSetting;
}

class WidgetSetting : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetSetting(QWidget* parent = nullptr);
  ~WidgetSetting();

  static WidgetSetting * GetInstance();
  void InitData(QVariantMap dataMap);

Q_SIGNALS:
  void SwitchVisitor();

private Q_SLOTS:
  void SwitchButtonClicked();
  void EditButtonClicked();
  void AutoLoginButtonClicked();

private:
  void InitLogTable();
  void Initialize();
  void TranslateLanguage();
  virtual void changeEvent(QEvent* e) override;

private:
  Ui::WidgetSetting* ui;
};

