#include "agvItem.h"
#include <QPainter>

AgvItem::AgvItem(QGraphicsItem* parent /*= nullptr*/)
    :QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    m_rect.setRect(-10,-10,20,20);
    setZValue(2);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void AgvItem::setAgvType(AgvType type)
{
    m_type = type;
}

void AgvItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QImage image;
    switch (m_type) {
        case AGV_RED:
        {
            image.load(":/image/agv_red.png");
            break;
        }
        case AGV_GREEN:
        {
            image.load(":/image/agv_green.png");
            break;
        }
        case AGV_PURPLE:
        {
            image.load(":/image/agv_purple.png");
            break;
        }
        case AGV_YELLOW:
        {
            image.load(":/image/agv_ye.png");
            break;
        }
    default:
        image.load(":/image/agv_green.png");
    }

    QMatrix matrix;
    matrix.rotate(90.0);
    image = image.transformed(matrix,Qt::FastTransformation);
    painter->drawImage(m_rect, image);
}

QRectF AgvItem::boundingRect() const
{
    return m_rect;
}
