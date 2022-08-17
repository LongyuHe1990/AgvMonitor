#ifndef SLAMITEM_H
#define SLAMITEM_H
#include <QGraphicsPixmapItem>

class SlamItem : public QGraphicsItem
{
public:
    SlamItem(QGraphicsPixmapItem *parent = nullptr);

    void setImageRect(QRect rect, const QImage& image);
    QRectF boundingRect() const override;

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QImage m_image;
    QRectF m_rect;
};

#endif // SLAMITEM_H
