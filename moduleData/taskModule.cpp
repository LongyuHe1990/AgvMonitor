#include "taskModule.h"
#include "customData.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "configModule.h"
#include "tasklist/widgettasklist.h"
#include <QDebug>
#include "customData.h"

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

void TaskModule::updataTask(QVariantMap data, int agv_id)
{
    int type = data.value("OperationType").toInt();
    QVariantMap content = data.value("Content").toMap();

    int id = content.value("id").toInt();
    int agvId = content.value("agvId").toInt();
    if(agvId == agv_id)
    {
        return;
    }

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
        QString actionName = m_actionName.find(QString::number(actionId))->toMap().value("name").toString();

        targetInfo += station.value("stationName").toString() +
          "(" +
          QString::number(action.value("agvAxisId").toInt()) +
          "-" +
          QString::number(action.value("stationAxisId").toInt()) +
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
