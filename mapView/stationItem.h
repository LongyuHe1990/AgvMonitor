#ifndef STATIONITEM_H
#define STATIONITEM_H

#include <QGraphicsItem>
#include <QPainterPath>
#include "linkLine.h"

enum StationType
{
    STATION_CHARGING,//充电桩
    STATION_WAITPOINT,//待命点
    STATION_COMMON//普通
};

class StationItem : public QGraphicsItem
{
public:
    StationItem(QGraphicsItem *parent = nullptr);
    ~StationItem();

    void setStationPoint(QPoint point);
    QPointF getStationPoint();

    void setAssociateNodePoint(QPoint point);
    QPointF getAssoicateNodePoint();

    void showLinkLine(bool show = true);

    void setStationName(QString name);
    void setStationType(StationType type);
private:

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPointF m_stationPoint;
    QPainterPath m_painterPath;
    QPointF m_associateNodePoint;

    LinkLine* m_linkLine;
    QString m_stationName;

    StationType  m_type;
};

#endif // STATIONITEM_H
