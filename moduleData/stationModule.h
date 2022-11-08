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

    void initStationTypeInfo(QVariantMap info);
    QVariantMap getStationType(int stationTypeId);

    QString getStationName(int id);
    QVariantMap getStation(int id);
    void initStationsInfo(QVariantMap info);
    void initStationFromStationType(int stationType, QVariantMap stationInfo);
    QVariantList getStationFromStationType(int stationTypeId = 0, int row = 0);
signals:

private:
    QVariantMap m_stations;
    QVariantMap m_stationTypeDistinguishesStation;
    QVariantMap m_stationTypeMap;
};

#endif // STATIONMODULE_H
