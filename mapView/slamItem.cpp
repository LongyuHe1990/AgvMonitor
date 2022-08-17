#include "slamItem.h"
#include <QPainter>

SlamItem::SlamItem(QGraphicsPixmapItem *parent /*= nullptr*/)
    :QGraphicsItem(parent)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void SlamItem::setImageRect(QRect rect, const QImage& image)
{
    setPos(rect.x(),rect.y());

    QRectF imageRect(0, 0, rect.width(), rect.height());
    m_rect = imageRect;

    //m_image = image.mirrored(false, true);
    m_image = image;
}

QRectF SlamItem::boundingRect() const
{
    return m_rect;
}

void SlamItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawImage(m_rect, m_image);
    //painter->setPen(QPen(Qt::black, 2 / painter->transform().m11(), Qt::SolidLine));
    //painter->drawRect(m_rect);
}
