#ifndef STATIONMODULE_H
#define STATIONMODULE_H

#include <QObject>
#include <QVariantMap>
#include "mapView/mapInfo.h"

class StationModule : public QObject
{
    Q_OBJECT
public:
    explicit StationModule(QObject *parent = nullptr);
    static StationModule* getInstance();

    void updataStation(QVariantMap data);

    void initCraftStationTypeInfo(QVariantMap info);
    QVariantMap getCraftStationType(int stationTypeId);

    QString getStationName(int id);
    QVariantMap getStation(int id);
    void initStationFromMap(int id, STATIONINFO stationInfo);
    void initStationFromStationType(int stationType, STATIONINFO stationInfo);
    QVariantList getStationFromStationType(int stationTypeId = 0, int row = 0);
signals:

private:
    QVariantMap m_stations;

    QVariantMap m_stationFromMap;
    QVariantMap m_stationTypeDistinguishesStation;
    QVariantMap m_craftStationTypeMap;
};

#endif // STATIONMODULE_H
