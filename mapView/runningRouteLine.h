#ifndef RUNNINGROUTELINE_H
#define RUNNINGROUTELINE_H
#include <QGraphicsItem>
#include <QPainterPath>

class RunningRouteLine : public QGraphicsItem
{
public:
    RunningRouteLine(QGraphicsItem* parent = nullptr);

    void setStartPoint(QPoint point);
    QPointF getStartPoint();

    void setEndPoint(QPoint point);
    QPointF getEndPoint();

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPointF m_startPoint;
    QPointF m_endPoint;

    QPainterPath m_painterPath;
    QTransform m_transform;
};

#endif // RUNNINGROUTELINE_H
