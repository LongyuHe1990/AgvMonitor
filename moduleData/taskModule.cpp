#include "taskModule.h"
#include "customData.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "configModule.h"
#include "tasklist/widgettasklist.h"
#include <QDebug>
#include "common/widgetmessagebox.h"

static TaskModule* s_taskModule = nullptr;

TaskModule::TaskModule(QObject *parent) : QObject(parent)
{
    s_taskModule = this;

    QString action = QString::fromLocal8Bit("{\"0\":{\"id\":\"0\",\"name\":\"无\"},\"1\":{\"id\":\"1\",\"name\":\"取货\"},\"2\":{\"id\":\"2\",\"name\":\"放货\"},\"3\":{\"id\":\"3\",\"name\":\"充电\"},\"4\":{\"id\":\"4\",\"name\":\"待命\"},\"5\":{\"id\":\"5 \",\"name\":\"预取货\"},\"6\":{\"id\":\"6\",\"name\":\"预放货\"}}");
    QJsonDocument doc = QJsonDocument::fromJson(action.toUtf8());
    m_actionName = doc.object().toVariantMap();
}

TaskModule *TaskModule::getInstance()
{
    return s_taskModule;
}

void TaskModule::updataTask(QVariantMap data)
{
    int type = data.value("OperationType").toInt();
    QVariantMap content = data.value("Content").toMap();

    int id = content.value("id").toInt();
    int agvId = content.value("agvId").toInt();

    int curTargetStationIndex = content.value("curTargetStationIndex").toInt();
    int executeStatus = content.value("executeStatus").toInt();
    QVariantList stationList = content.value("stationList").toList();

    QVariantMap taskData;
    if(curTargetStationIndex >= stationList.size())
    {
        taskData.insert("taskPeriod", "执行完成");
    }
    else if(executeStatus)
    {
        QVariantMap station = stationList.at(curTargetStationIndex).toMap();
        taskData.insert("taskPeriod", "前往" + station.value("stationName").toString());
    }
    else
    {
        taskData.insert("taskPeriod", "等待");
    }

    QString targetInfo = "";
    for(int i = 0; i < stationList.size(); ++i)
    {
        QVariantMap station = stationList.at(i).toMap();
        QVariantList actionList = station.value("actionList").toList();
        QVariantMap action = actionList.at(0).toMap();
        int actionId = action.value("action").toInt();
        QString actionName = "";
        if(m_actionName.find(QString::number(actionId)) != m_actionName.end())
        {
           actionName = m_actionName.find(QString::number(actionId))->toMap().value("name").toString();
        }
        targetInfo += station.value("stationName").toString() +
          "(" + "A" +
          QString::number(action.value("agvBufferIndex").toInt()) +
          "-" + "S" +
          QString::number(action.value("stationBufferIndex").toInt()) +
          "-" +
          (actionName.isEmpty() ? "无" : actionName) +
          ");";
    }

    taskData.insert("targetInfo", targetInfo);
    taskData.insert("id", id);
    taskData.insert("agvName", ConfigModule::getInstance()->getAgvName(agvId));
    if(type == int(TaskOperationType::Task_Created) || type == int(TaskOperationType::Task_Updated))
    {
        m_tasks.insert(QString::number(id), taskData);
    }
    else if(type == int(TaskOperationType::Task_Erased))
    {
        m_tasks.erase(m_tasks.find(QString::number(id)));
    }

    //界面更新
    WidgetTaskList::GetInstance()->InitData(m_tasks);
}

void TaskModule::taskOperaterState(int result)
{
    if(result == static_cast<int>(TaskCreateError::NoError))
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("任务创建成功"),false);
    }
    else if(result == static_cast<int>(TaskCreateError::AgvNotExist),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("当指定AGV时 ，指定的AGV不存在"),false);
    }
    else if(result == static_cast<int>(TaskCreateError::StationListEmpty),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("目标站台为空"),false);
    }
    else if(result == static_cast<int>(TaskCreateError::StationNotExist),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("目标站台不存在"),false);
    }
    else if(result == static_cast<int>(TaskCreateError::StationMapAtypism),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("目标站台不在一张地图上"),false);
    }
    else if(result == static_cast<int>(TaskCreateError::StationAgvMapAtypism),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("目标站台与指定AGV不在一张地图上"));
    }
    else if(result == static_cast<int>(TaskCreateError::AgvLock),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("指定AGV被锁定"));
    }
    else if(result == static_cast<int>(TaskCreateError::StationMapIdNotExplicit),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("站台的地图编号不明确"));
    }
    else if(result == static_cast<int>(TaskCreateError::StationActionNotExist),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("站台无法执行此动作"));
    }
    else if(result == static_cast<int>(TaskCreateError::NotFindStationAction),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("未下发站台动作"));
    }
    else if(result == static_cast<int>(TaskCreateError::TaskNumRepeat),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("TaskNumRepeat"));
    }
    else if(result == static_cast<int>(TaskCreateError::TaskIdRepeat),false)
    {
        WidgetMessageBox().MessageHint(tr("任务"), tr("MasterDisconnected"));
    }
    else
    {
         WidgetMessageBox().MessageHint(tr("任务"), tr("未知错误"),false);
    }
}
