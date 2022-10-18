#ifndef LIFTITEM_H
#define LIFTITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPoint>

class LiftItem : public QGraphicsItem
{
public:
    LiftItem(QGraphicsItem* parent = nullptr);
    void setLiftPoint(QPoint point);

private:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QRect m_boundingRect;
};

#endif // LIFTITEM_H
