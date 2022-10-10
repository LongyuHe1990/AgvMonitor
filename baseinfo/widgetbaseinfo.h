/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include <QVariantMap>
#include "common/global_helper.h"

namespace Ui {
class WidgetBaseInfo;
}

class WidgetBaseInfo : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetBaseInfo(QWidget* parent = nullptr);
  ~WidgetBaseInfo();

  static WidgetBaseInfo * GetInstance();
  void                    InitData(QVariantMap data_map);
  void                    VisitorModel(bool model);

private:
  void ShowBaseInfo();

private Q_SLOTS:
  void LogoutButtonClicked();
  void PauseButtonClicekd();

private:
  void Initialize();
  void TranslateLanguage();

private:
  Ui::WidgetBaseInfo* ui;

  QVariantMap data_map_;
};

