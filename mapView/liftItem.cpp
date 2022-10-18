#include "liftItem.h"
#include <QImage>
#include <QPainter>

LiftItem::LiftItem(QGraphicsItem* parent /*= nullptr*/)
    :QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setZValue(1);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QRectF LiftItem::boundingRect() const
{
    return m_boundingRect;
}

void LiftItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QImage image;
    image.load(":/image/lift.png");

    painter->drawImage(m_boundingRect, image);
    painter->drawRect(m_boundingRect);
}

void LiftItem::setLiftPoint(QPoint point)
{
    setPos(point);
    m_boundingRect = QRect(-15,-15,30,30);
}
