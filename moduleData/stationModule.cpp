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

void StationModule::initCraftStationTypeInfo(QVariantMap info)
{
    QVariantList list = info.value("Content").toList();
    for(int i = 0; i<list.size(); ++i)
    {
        QVariantMap type = list.at(i).toMap();
        int id = type.value("id").toInt();
        m_craftStationTypeMap.insert(QString::number(id), type);
    }
}

QVariantMap StationModule::getCraftStationType(int stationTypeId)
{
    QVariantMap info;
    QVariantMap::iterator iter = m_craftStationTypeMap.find(QString::number(stationTypeId));
    if(iter != m_craftStationTypeMap.end())
    {
        info = iter.value().toMap();
    }

    return info;
}

QString StationModule::getStationName(int id)
{
    QVariantMap station = m_stationFromMap.value(QString::number(id)).toMap();
    QString name = station.value("name").toString();
    return name;
}

QVariantMap StationModule::getStation(int id)
{
    QVariantMap station = m_stationFromMap.value(QString::number(id)).toMap();
    return station;
}

void StationModule::initStationFromMap(int id, STATIONINFO stationInfo)
{
    QVariantMap station;
    station.insert("id",stationInfo.id);
    station.insert("name", stationInfo.name);
    station.insert("nodeId", stationInfo.node.id);
    station.insert("stationTypeId", stationInfo.stationTypeId);
    station.insert("row", stationInfo.row);
    station.insert("column", stationInfo.column);
    m_stationFromMap.insert(QString::number(id), station);

    initStationFromStationType(stationInfo.stationTypeId, stationInfo);
}

void StationModule::initStationFromStationType(int stationType, STATIONINFO stationInfo)
{
    QVariantMap station;
    station.insert("id",stationInfo.id);
    station.insert("name", stationInfo.name);
    station.insert("nodeId", stationInfo.node.id);
    station.insert("stationTypeId", stationInfo.stationTypeId);
    station.insert("row", stationInfo.row);
    station.insert("column", stationInfo.column);

    QVariantMap::iterator iter = m_stationTypeDistinguishesStation.find(QString::number(stationType));
    if(iter != m_stationTypeDistinguishesStation.end())
    {
        QVariantList list = iter.value().toList();
        list.push_back(station);
    }
    else
    {
        m_stationTypeDistinguishesStation.insert(QString::number(stationType), station);
    }
}

QVariantList StationModule::getStationFromStationType(int stationTypeId, int row)
{
    QVariantList list;

    if(stationTypeId == 0)
    {
        for(int i = 0; i<m_stationFromMap.size(); ++i)
        {
            QVariantMap station = list.at(i).toMap();
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

