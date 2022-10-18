#ifndef CONFIGMODULE_H
#define CONFIGMODULE_H

#include <QObject>
#include <QVariantMap>
#include "customData.h"

class ConfigModule:public QObject
{
    Q_OBJECT

public:
    ConfigModule(QObject* paretn = nullptr);

    static ConfigModule* getInstance();

    void initConfig(QVariantMap data);
    QVariantMap getConfig(ConfigType configType, int id);

    QString getAgvName(int id);
    QVariantMap getcurrentAgvTemplateLoadDevice(int agvTemplateId);
    QVariantMap getMaterialType(int id);
    QVariantMap getStationTypeParams();
    QVariantMap getMaterialBufferParams(int stationTypeId);
    QVariantList getActionTypeOfStationType(int stationTypeId);
    int getAgvBelongMapId(int agvId);
private:
    QVariantMap m_configs;
    int m_agvId;
};

#endif // CONFIGMODULE_H
