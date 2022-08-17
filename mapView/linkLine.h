#ifndef LINKLINE_H
#define LINKLINE_H
#include <QGraphicsItem>

class LinkLine : public QGraphicsItem
{
public:
    LinkLine(QGraphicsItem *parent = nullptr);

    void setStartPoint(QPoint point);
    QPointF getStartPoint();

    void setEndPoint(QPoint point);
    QPointF getEndPoint();

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPainterPath m_linePath;
    QPointF m_startPoint;
    QPointF m_endPoint;
};

#endif // LINKLINE_H
