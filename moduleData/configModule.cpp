#include "configModule.h"
#include "mapView/mapMonitoringView.h"
#include "allinfo/widgetallinfo.h"


static ConfigModule *s_configModule = nullptr;

ConfigModule::ConfigModule(QObject *parent /*=nullptr*/)
    :QObject(parent)
{
    s_configModule = this;
}

ConfigModule *ConfigModule::getInstance()
{
    return s_configModule;
}

void ConfigModule::setAgvId(int id)
{
    m_agvId = id;
}

int ConfigModule::getAgvId()
{
    return m_agvId;
}

void ConfigModule::initConfig(QVariantMap data)
{
    QVariantMap moduleData = data.value("ModuleData").toMap();
    int type = moduleData.value("OperationType").toInt();
    switch(static_cast<ConfigOperatedType>(type))
    {
    case ConfigOperatedType::OPERATED_TYPE_INIT:
    {
        m_configs = moduleData.value("Content").toMap();
        WidgetAllInfo::GetInstance()->GetDeviceName(m_configs);
        break;
    }
    }
}

QVariantMap ConfigModule::getConfig(ConfigType configType, int id)
{
    QVariantMap list =  m_configs.value(QString::number(int(configType))).toMap();
    if(!list.isEmpty())
    {
        return list.value(QString::number(id)).toMap();
    }
}

QString ConfigModule::getAgvName(int id)
{
    QString name = "";
    QVariantMap agvs = m_configs.value(QString::number(int(ConfigType::Agv))).toMap();

    QVariantMap agv = agvs.value(QString::number(id)).toMap();
    name = agv.value("name").toString();
    return name;
}

QVariantMap ConfigModule::getcurrentAgvTemplateLoadDevice(int agvTemplateId)
{
    QVariantMap loadDeviceArray;
    int type = int(ConfigType::AgvTemplate);
    QVariantMap agvTemplateArray = m_configs.value(QString::number(type)).toMap();
    QVariantMap::iterator iter = agvTemplateArray.find(QString::number(agvTemplateId));
    if(iter != agvTemplateArray.end())
    {
        QVariantMap agvTemplate = iter.value().toMap();
        QVariantList loadDeviceParamList = agvTemplate.value("loadDeviceParamList").toList();

        for(auto device : loadDeviceParamList)
        {
            QVariantMap deviceMap = device.toMap();
            loadDeviceArray.insert(QString::number(deviceMap.value("id").toInt()), deviceMap);
        }
    }

    return loadDeviceArray;
}

QVariantMap ConfigModule::getMaterialType(int id)
{
    int type = int(ConfigType::MaterialType);
    QVariantMap materialArray = m_configs.value(QString::number(type)).toMap();
    QVariantMap material = materialArray.value(QString::number(id)).toMap();
    return material;
}

QVariantMap ConfigModule::getMaterialBufferParams(int stationTypeId)
{
    QVariantMap params;
    int type = int(ConfigType::StationType);
    QVariantMap stationTypeArray = m_configs.value(QString::number(type)).toMap();

    QVariantMap::iterator iter = stationTypeArray.find(QString::number(stationTypeId));
    if(iter != stationTypeArray.end())
    {
        QVariantMap stationType = iter.value().toMap();
        QVariantList materialList = stationType.value("materialBufferParamList").toList();

        for(auto materialIter : materialList)
        {
            QVariantMap material = materialIter.toMap();
            params.insert(QString::number(material.value("id").toInt()), material);
        }
    }

    return params;
}

QVariantList ConfigModule::getActionTypeOfStationType(int stationTypeId)
{
    QVariantList actionTypelList;
    int type = int(ConfigType::StationType);
    QVariantMap stationTypeArray = m_configs.value(QString::number(type)).toMap();

    QVariantMap::iterator iter = stationTypeArray.find(QString::number(stationTypeId));
    if(iter != stationTypeArray.end())
    {
        QVariantMap stationType = iter.value().toMap();
        actionTypelList = stationType.value("actionTypeList").toList();
    }

    return actionTypelList;
}

int ConfigModule::getAgvBelongMapId(int agvId)
{
    int mapId = 0;
    int agvIndex = int(ConfigType::Agv);
    QVariantMap agvArray = m_configs.value(QString::number(agvIndex)).toMap();
    QVariantMap::iterator iter = agvArray.find(QString::number(agvId));
    if(iter != agvArray.end())
    {
        QVariantMap agv = iter.value().toMap();
        int agvType = agv.value("agvTypeId").toInt();

        int agvTypeIndex = int(ConfigType::AgvType);
        QVariantMap agvTypeArray = m_configs.value(QString::number(agvTypeIndex)).toMap();
        QVariantMap::iterator iterType = agvTypeArray.find(QString::number(agvType));
        if(iterType != agvTypeArray.end())
        {
            QVariantMap agvType = iterType.value().toMap();
            mapId = agvType.value("mapId").toInt();
        }
    }

    return mapId;
}

QVariantMap ConfigModule::getStationTypeParams()
{
    int type = int(ConfigType::StationType);
    QVariantMap stationTypeArray = m_configs.value(QString::number(type)).toMap();

    return stationTypeArray;
}
