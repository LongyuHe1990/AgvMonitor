#include "runningRouteLine.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>
#include <math.h>

RunningRouteLine::RunningRouteLine(QGraphicsItem *parent /*= nullptr*/)
    :QGraphicsItem(parent)
{
    //setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QRectF RunningRouteLine::boundingRect() const
{
    QRectF rect = m_painterPath.boundingRect();
    const qreal x1 = m_startPoint.x();
    const qreal x2 = m_endPoint.x();
    const qreal y1 = m_startPoint.y();
    const qreal y2 = m_endPoint.y();
    qreal lx = qMin(x1, x2);
    qreal rx = qMax(x1, x2);
    qreal ty = qMin(y1, y2);
    qreal by = qMax(y1, y2);
    rect = QRectF(lx, ty, rx - lx, by - ty);

    //这个会导致刷新不完整
    //rect = m_painterPath.boundingRect();
    return rect;
}

void RunningRouteLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPainterPath path;
    path.moveTo(m_startPoint);
    path.lineTo(m_endPoint);
    m_painterPath = path;

    if(fabs(painter->transform().m11()) > 1e-15)
    {
        QPen pen(Qt::yellow, 2 / painter->transform().m11(), Qt::SolidLine);
        painter->setPen(pen);
    }

    painter->drawPath(m_painterPath);
}

void RunningRouteLine::setStartPoint(QPoint point)
{
    setPos(point);

    m_startPoint = mapFromScene(point);
}

QPointF RunningRouteLine::getStartPoint()
{
    return m_startPoint;
}

void RunningRouteLine::setEndPoint(QPoint point)
{
    m_endPoint = mapFromScene(point);
}

QPointF RunningRouteLine::getEndPoint()
{
    return m_endPoint;
}
