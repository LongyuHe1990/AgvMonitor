#ifndef TASKMODULE_H
#define TASKMODULE_H

#include <QObject>
#include <QVariantMap>


class TaskModule : public QObject
{
    Q_OBJECT
public:
    explicit TaskModule(QObject *parent = nullptr);

    static TaskModule* getInstance();

    void updataTask(QVariantMap data, int agvId);
signals:

private:
    QVariantMap  m_tasks;
    QVariantMap m_actionName;
};

#endif // TASKMODULE_H
