#ifndef AGVITEM_H
#define AGVITEM_H
#include <QGraphicsItem>

enum AgvType{
    AGV_GREEN,
    AGV_PURPLE,
    AGV_RED,
    AGV_YELLOW
};

class AgvItem : public QGraphicsItem
{
public:
    AgvItem(QGraphicsItem* parent = nullptr);
    ~AgvItem();

    void setAgvType(AgvType type);
private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

private:
    QRectF m_rect;
    AgvType m_type;
};

#endif // AGVITEM_H
