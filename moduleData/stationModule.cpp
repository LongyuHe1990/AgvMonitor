#include "stationModule.h"
#include "configModule.h"
#include "customData.h"

static StationModule* s_stationModule = nullptr;

StationModule::StationModule(QObject *parent) : QObject(parent)
{
    s_stationModule = this;
}

StationModule *StationModule::getInstance()
{
    return s_stationModule;
}

void StationModule::updataStation(QVariantMap data)
{
    QVariantMap content = data.value("Content").toMap();
    int id = content.value("id").toInt();

    m_stations.insert(QString::number(id), content);
}

void StationModule::initStationTypeInfo(QVariantMap info)
{
    QVariantList list = info.value("Content").toList();
    for(int i = 0; i<list.size(); ++i)
    {
        QVariantMap type = list.at(i).toMap();
        int id = type.value("id").toInt();
        m_stationTypeMap.insert(QString::number(id), type);
    }
}

QVariantMap StationModule::getStationType(int stationTypeId)
{
    QVariantMap info;
    QVariantMap::iterator iter = m_stationTypeMap.find(QString::number(stationTypeId));
    if(iter != m_stationTypeMap.end())
    {
        info = iter.value().toMap();
    }

    return info;
}

QString StationModule::getStationName(int id)
{
    QVariantMap station = m_stations.value(QString::number(id)).toMap();
    QString name = station.value("name").toString();
    return name;
}

QVariantMap StationModule::getStation(int id)
{
    QVariantMap station = m_stations.value(QString::number(id)).toMap();
    return station;
}

void StationModule::initStationsInfo(QVariantMap info)
{
    m_stations.clear();
    QVariantList list = info.value("Content").toList();
    for(auto item : list)
    {
        QVariantMap stationInfo = item.toMap();
        int id = stationInfo.value("id").toInt();
        int typeId = stationInfo.value("stationTypeId").toInt();
        m_stations.insert(QString::number(id), stationInfo);

        initStationFromStationType(typeId, stationInfo);
    }
}

void StationModule::initStationFromStationType(int stationType, QVariantMap stationInfo)
{
    QVariantMap::iterator iter = m_stationTypeDistinguishesStation.find(QString::number(stationType));
    if(iter != m_stationTypeDistinguishesStation.end())
    {
        QVariantList list = iter.value().toList();
        list.push_back(stationInfo);
        m_stationTypeDistinguishesStation.insert(QString::number(stationType), list);
    }
    else
    {
        QVariantList list;
        list.push_back(stationInfo);
        m_stationTypeDistinguishesStation.insert(QString::number(stationType), list);
    }
}

QVariantList StationModule::getStationFromStationType(int stationTypeId, int row)
{
    QVariantList list;

    if(stationTypeId == 0)
    {
        QVariantMap::iterator iter = m_stations.begin();
        for(;iter != m_stations.end(); ++iter)
        {
            QVariantMap station = iter.value().toMap();
            list.push_back(station);
        }
        return list;
    }
    else
    {
        QVariantMap::iterator iter = m_stationTypeDistinguishesStation.find(QString::number(stationTypeId));
        if(iter != m_stationTypeDistinguishesStation.end())
        {
            list = iter.value().toList();
        }

        if(row == 0)
        {
            return list;
        }
        else
        {
            QVariantList rowList;
            for(int i = 0; i<list.size(); ++i)
            {
                QVariantMap station = list.at(i).toMap();
                int stationRow = station.value("row").toInt();
                if(stationRow == row)
                {
                    rowList.push_back(station);
                }
            }
            return rowList;
        }
    }
}

