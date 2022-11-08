#pragma once

#include <QWidget>
#include <QString>

struct StatusAndColorInfo {
  QString status;
  QString color;
};

struct NameAndLabelInfo {
  QString name;
  QString label;
};

struct StatisticsInfo
{
  int value;
  int value1;
  QString key;

  inline int Max()
  {
    return value > value1 ? value : value1;
  }
};
using StatisticsInfoList = std::list<StatisticsInfo>;

QString            GetWalkStatus(int walkStatus);
StatusAndColorInfo GetWorkStatusInfo(int workStatus);
StatusAndColorInfo GetExecuteStatusInfo(int executeStatus);
StatusAndColorInfo GetSafeDeviceStateInfo(bool status);
StatusAndColorInfo GetConfigStatus(int status);
QString            GetAgvHoverState(int taskState);
NameAndLabelInfo   GetActionType(int actionType);
QString            GetCurrentBinPath();


