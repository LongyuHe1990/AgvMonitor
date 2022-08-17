#ifndef NODEITEM_H
#define NODEITEM_H
#include <QGraphicsItem>

class NodeItem : public QGraphicsItem
{
public:
    NodeItem();
    void setNodePoint(QPoint point);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    QPointF m_nodePoint;
    QPainterPath m_nodePath;
};

#endif // NODEITEM_H
