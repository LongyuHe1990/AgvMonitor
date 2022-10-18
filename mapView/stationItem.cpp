#include "stationItem.h"
#include <QPainter>
#include <QTransform>
#include <QGraphicsScene>
#include <QDebug>
#include <QApplication>

StationItem::StationItem(QGraphicsItem *parent/* = nullptr*/)
    :QGraphicsItem(parent)
    ,m_linkLine(nullptr)
    ,m_stationName("")
    ,m_type(STATION_COMMON)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    m_linkLine = new LinkLine();
}

StationItem::~StationItem()
{
    if(m_linkLine != nullptr)
    {
        m_linkLine = nullptr;
    }
}

void StationItem::setStationPoint(QPoint point)
{
    setPos(point);
    m_stationPoint = mapFromScene(point);
    m_linkLine->setStartPoint(point);
}

QPointF StationItem::getStationPoint()
{
    return m_stationPoint;
}

void StationItem::setAssociateNodePoint(QPoint point)
{
    m_associateNodePoint = mapFromScene(point);
    m_linkLine->setEndPoint(point);
}

QPointF StationItem::getAssoicateNodePoint()
{
    return m_associateNodePoint;
}

void StationItem::showLinkLine(bool show /*=true*/)
{
    if(scene() != nullptr)
    {
        if(show)
        {
            scene()->addItem(m_linkLine);
        }
        else{
            scene()->removeItem(m_linkLine);
        }
    }
}

void StationItem::setStationName(QString name)
{
    m_stationName = name;
}

void StationItem::setStationType(StationType type)
{
    m_type = type;
}

QRectF StationItem::boundingRect() const
{
    return QRect(m_boundingRect.topLeft(),m_boundingRect.bottomRight());
}

void StationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRect imageRect(-10,-10,20,20);
    painter->setPen(QPen(Qt::green));

    QImage image;
    switch (m_type)
    {
        case STATION_CHARGING:
        {
            image.load(":/image/charge_station.png");
            break;
        }
        case STATION_WAITPOINT:
        {
            image.load(":/image/park_station.png");
            break;
        }
        case STATION_COMMON:
        {
            image.load(":/image/common_station.png");
            break;
        }
    }
    painter->drawImage(imageRect, image);


    QFont font = QApplication::font();
    QFontMetrics metrics(font);
    int nameHeight = metrics.height();
    int nameWidth = metrics.width(m_stationName);
    QRect titleRect(-(nameWidth / 2), imageRect.bottomLeft().y(), nameWidth, nameHeight);
    painter->setPen(QPen("#1afa29"));
    painter->setBrush(QBrush("#1afa29"));
    painter->drawText(titleRect, m_stationName);

    m_boundingRect = imageRect;
    m_boundingRect = m_boundingRect | titleRect;
    //m_boundingRect.adjust(-10,-10,10,10);

    painter->setBrush(Qt::transparent);
    painter->drawRect(m_boundingRect);

    painter->setPen(Qt::red);
    painter->drawRect(titleRect);
}
