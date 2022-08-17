#include "global_helper.h"

QString GetWalkStatus(int walkStatus)
{
  switch(walkStatus)
  {
  case 0: return QString("获取位置");
  case 1: return QString("空闲");
  case 2: return QString("行走中");
  case 3: return QString("等待");
  case 4: return QString("动作中");
  case 5: return QString("等待空闲");
  case 6: return QString("任务开始");
  case 7: return QString("前往站台");
  case 8: return QString("进入站台");
  case 9: return QString("抵达站台");
  case 10: return QString("离开站台");
  case 11: return QString("任务完成");
  case 12: return QString("规划路径");
  case 13: return QString("规划路径");
  case 14: return QString("充电中");
  default:
    return QString();
  }
}

StatusAndColorInfo GetWorkStatusInfo(int workStatus)
{
  StatusAndColorInfo info;
  switch(workStatus)
  {
  case 0:
    info.status = "离线";
    info.color  = "#ec1c24";
    break;
  case 1:
    info.status = "空闲";
    info.color  = "#42ec20";
    break;
  case 2:
    info.status = "任务";
    info.color  = "#cca436";
    break;
  case 3:
    info.status = "异常";
    info.color  = "#ec1c24";
    break;
  case 4:
    info.status = "挂起";
    info.color  = "#808080";
    break;
  case 5:
    info.status = "暂停";
    info.color  = "#ec1c24";
    break;
  case 6:
    info.status = "上线中";
    info.color  = "#ec1c24";
    break;
  case 7:
    info.status = "未运行";
    info.color  = "#808080";
    break;
  case 8:
    info.status = "低电量";
    info.color  = "#cca436";
    break;
  case 9:
    info.status = "手动";
    info.color  = "#00BFFF";
    break;
  case 10:
    info.status = "强制充电";
    info.color  = "#cca436";
    break;
  case 11:
    info.status = "待命";
    info.color  = "#42ec20";
    break;
  case 3000:
    info.status = "服务断开";
    info.color  = "#ec1c24";
    break;
  default:
    info.status = "";
    info.color  = "";
    break;
  }
  return info;
}

StatusAndColorInfo GetExecuteStatusInfo(int executeStatus)
{
  StatusAndColorInfo info;
  switch(executeStatus)
  {
  case -1:
    info.status = "命令未接收";
    info.color  = "#ec1c24";
    break;
  case 0:
    info.status = "命令未执行";
    info.color  = "#ec1c24";
    break;
  case 1:
    info.status = "命令执行中";
    info.color  = "#cca436";
    break;
  case 2:
    info.status = "命令执行完成";
    info.color  = "#cca436";
    break;
  default:
    info.status = "";
    info.color  = "";
    break;
  }
  return info;
}

StatusAndColorInfo GetSafeDeviceStateInfo(bool status)
{
  StatusAndColorInfo info;
  if(status)
  {
    info.status = "触发";
    info.color  = "#ec1c24";
  }
  else
  {
    info.status = "正常";
    info.color  = "#34ffff";
  }
  return info;
}

StatusAndColorInfo GetConfigStatus(int status)
{
  StatusAndColorInfo info;
  switch(status)
  {
  case 0:
    info.status = "未更新";
    info.color  = "#ec1c24";
    break;
  case 1:
    info.status = "更新未重启";
    info.color  = "#cca436";
    break;
  case 2:
    info.status = "已最新";
    info.color  = "#cca436";
    break;
  default:
    info.status = "";
    info.color  = "";
    break;
  }
  return info;
}

QString GetAgvHoverState(int taskState)
{
  switch(taskState)
  {
  case 0: return "NotReady";
  case 1: return "Idle";
  case 2: return "Busy";
  default:
    return QString();
  }
}

NameAndLabelInfo GetActionType(int actionType)
{
  NameAndLabelInfo info;
  switch(actionType)
  {
  case 0:
    info.name  = "无";
    info.label = "None";
    break;
  case 1:
    info.name  = "取货";
    info.label = "Load";
    break;
  case 2:
    info.name  = "放货";
    info.label = "Unload";
    break;
  case 3:
    info.name  = "充电";
    info.label = "Charge";
    break;
  case 4:
    info.name  = "待命";
    info.label = "Parking";
    break;
  case 5:
    info.name  = "预取货";
    info.label = "PreLoad";
    break;
  case 6:
    info.name  = "预放货";
    info.label = "PreUnLoad";
    break;
  default:
    info.name  = "";
    info.label = "";
    break;
  }

  return info;
}
