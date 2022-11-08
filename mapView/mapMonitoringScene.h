#ifndef MAPMONITORINGSCENE_H
#define MAPMONITORINGSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "slamItem.h"
#include "mapInfo.h"
#include "agvItem.h"

class MapMonitoringScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MapMonitoringScene();
    ~MapMonitoringScene();

    const SlamItem* getSlamItem()const;
    const AgvItem* getAgvItem()const;

    void addNodeItems(QMap<QString, NODEINFO> nodes);
    void addLineItems(QMap<QString, LINEINFO> lines);
    void addStationItems(QMap<QString, STATIONINFO> stations);
    void addLiftItems(QMap<QString, LIFTINFO> lifts);
    void addSlamItem(const QImage &image, QRect rect);
    void addAgvItem(int floor);
    void clearAllItem();

    void updataAgvItemPos(QPointF pos, int angle, bool show);
private:
    SlamItem *m_slamItem;
    AgvItem *m_agvItem;
    QTransform m_agvTransform;
};

#endif // MAPMONITORINGSCENE_H
