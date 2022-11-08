/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include "common/global_helper.h"

namespace Ui {
class WidgetAllInfo;
}

struct AgvStateInfo {  // 车体状态
  int workStatus;
  int walkStatus;
  double speed;
  int paramConfigStatus;
  int mapConfigStatus;
  bool isScram;      // 急停
  bool isEdged;      // 触边
  bool isAvoid;
  int executeStatus; // 命令执行状态
};

struct AgvInfo {   // 车体
  int latestCmdId;
  int currentCmdId;
  int taskState;
  bool isNormal;
  bool isAuto;
  int angle;    ///100
  int confidence;
};

struct AgvConfigInfo {
  int mapConfigStatus;
  QString latestMapUuid;
  int latestMapSize;
  QString updatedMapUuid;
  int updatedMapSize;
  QString currentMapUuid;
  int currentMapSize;
  int paramConfigStatus;
  QString latestParamUuid;
  QString updatedParamUuid;
  QString currentParamUuid;
};

struct AgvDeviceInfo {
  int lastestExecuteStatus;
  int expectMaterialNum;
  int currentExecuteStatus;
  int cmdId_system;
  int action_system;
  int step_system;
  int materialNum_system;
  int materialType_system;
  int cmdId_device;
  int action_device;
  int step_device;
  int materialNum_device;
  int materialType_device;
};

class WidgetAllInfo : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetAllInfo(QWidget* parent = nullptr);
  ~WidgetAllInfo();

  static WidgetAllInfo * GetInstance();
  void                   InitData(QVariantMap dataMap);

private Q_SLOTS:
  void CurrentIndexChanged(int index);

private:
  void ShowAgvBodyStateInfo();
  void ShowAgvInfo();
  void ShowAgvConfigInfo();
  void ShowAgvDeviceInfo();
  void Initialize();
  void TranslateLanguage();
  void GetDeviceName();

  virtual void changeEvent(QEvent* e) override;

private:
  Ui::WidgetAllInfo* ui;

  AgvStateInfo  body_state_;
  AgvInfo       agv_info_;
  AgvConfigInfo config_info_;
  AgvDeviceInfo device_info_;
};

