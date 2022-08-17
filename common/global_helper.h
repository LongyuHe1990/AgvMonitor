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
  QString key;

  inline bool operator ==(StatisticsInfo other) const
  {
    return key == other.key;
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


