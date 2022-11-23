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
  int this_week;
  int last_week;
  QString key;

  inline int Max()
  {
    return this_week > last_week ? this_week : last_week;
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
QString  HandleLongString(QWidget *  widget, QString str, int length);


