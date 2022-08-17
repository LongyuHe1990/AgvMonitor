#include "nodeItem.h"
#include <QPainter>

NodeItem::NodeItem()
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setZValue(1);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void NodeItem::setNodePoint(QPoint point)
{
    setPos(point);
    m_nodePoint = mapFromScene(point);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::yellow);
    QPainterPath path;
    path.addEllipse(-5,-5,10,10);
    m_nodePath = path;
    painter->drawPath(path);
}

QRectF NodeItem::boundingRect() const
{
    return m_nodePath.boundingRect();
}
