#ifndef STATIONITEM_H
#define STATIONITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainterPath>
#include "linkLine.h"
#include "mapInfo.h"

enum StationType
{
    STATION_CHARGING,//充电桩
    STATION_WAITPOINT,//待命点
    STATION_COMMON//普通
};

class StationItem: public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    StationItem(QGraphicsItem *parent = nullptr);
    ~StationItem();

    void setStationPoint(QPoint point);
    QPointF getStationPoint();

    void setAssociateNodePoint(QPoint point);
    QPointF getAssoicateNodePoint();

    void showLinkLine(bool show = true);

    void setStationInfo(STATIONINFO info);
    void setStationType(StationType type);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;

private:

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void createTask();
private:
    QPointF m_stationPoint;
    QPointF m_associateNodePoint;

    LinkLine* m_linkLine;
    STATIONINFO m_stationInfo;

    StationType  m_type;
    QRect m_boundingRect;
    QTimer* m_timer;
};

#endif // STATIONITEM_H
