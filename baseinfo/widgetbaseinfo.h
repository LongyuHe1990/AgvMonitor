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

struct BaseInfo {
  double current;
  int energy;
  double voltage;
  bool isCharging;
  int futureNodeId;
  int targetStationId;
  int walkStatus;
  int workStatus;
  int id;
};

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

private:
  void Initialize();
  void TranslateLanguage();

private:
  Ui::WidgetBaseInfo* ui;

  QVariantMap data_map_;
  BaseInfo    base_info_;
};

