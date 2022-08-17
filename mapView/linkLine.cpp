#include "linkLine.h"
#include <QPainter>
#include <QDebug>
#include <math.h>

LinkLine::LinkLine(QGraphicsItem *parent /*= nullptr*/)
    :QGraphicsItem(parent)
{
    //setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void LinkLine::setStartPoint(QPoint point)
{
    setPos(point);
    m_startPoint = mapFromScene(point);
}

QPointF LinkLine::getStartPoint()
{
    return m_startPoint;
}

void LinkLine::setEndPoint(QPoint point)
{
    m_endPoint = mapFromScene(point);
}

QPointF LinkLine::getEndPoint()
{
    return m_endPoint;
}

QRectF LinkLine::boundingRect() const
{
    return m_linePath.boundingRect();
}

void LinkLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPainterPath path;
    path.moveTo(m_startPoint);
    path.lineTo(m_endPoint);
    m_linePath = path;

    if(fabs(painter->transform().m11()) > 1e-15)
    {
        painter->setPen(QPen(QColor("#DAA520"), 1 / painter->transform().m11(), Qt::DashLine));
    }

    painter->drawPath(m_linePath);
}
