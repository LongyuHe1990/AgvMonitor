#include "mapMonitoringScene.h"
#include "stationItem.h"
#include "runningRouteLine.h"
#include "nodeItem.h"
#include <QGraphicsView>

MapMonitoringScene::MapMonitoringScene()
    :m_slamItem(nullptr)
    ,m_agvItem(new AgvItem())
{
    setBackgroundBrush(QBrush("#0b1d2e"));

    setSceneRect(-500000,-500000,1000000,1000000);

    addItem(m_agvItem);
    m_agvTransform = m_agvItem->transform();
}

MapMonitoringScene::~MapMonitoringScene()
{
    m_slamItem = nullptr;
    m_agvItem = nullptr;
}

const SlamItem *MapMonitoringScene::getSlamItem() const
{
    return m_slamItem;
}

void MapMonitoringScene::addNodeItems(QMap<QString, NODEINFO> nodes)
{
    for(auto node : nodes)
    {
        NodeItem *item = new NodeItem();
        item->setNodePoint(node.point.toPoint());
        addItem(item);
    }
}

void MapMonitoringScene::addLineItems(QMap<QString, LINEINFO> lines)
{
    for(auto line : lines)
    {
        RunningRouteLine* lineItem = new RunningRouteLine();
        lineItem->setStartPoint(line.startNode.point.toPoint());
        lineItem->setEndPoint(line.endNode.point.toPoint());
        addItem(lineItem);
    }
}

void MapMonitoringScene::addStationItems(QMap<QString, STATIONINFO> stations)
{
    for(auto station : stations)
    {
        StationItem *stationItem = new StationItem();
        stationItem->setStationPoint(station.point.toPoint());
        stationItem->setAssociateNodePoint(station.node.point.toPoint());
        stationItem->setStationName(station.name);
        addItem(stationItem);
        stationItem->showLinkLine();
    }
}

void MapMonitoringScene::addSlamItem(const QImage &image, QRect rect)
{
    if(!image.isNull() && !rect.isNull())
    {
        m_slamItem = new SlamItem();
        m_slamItem->setImageRect(rect, image);
        addItem(m_slamItem);
    }
}

void MapMonitoringScene::updataAgvItemPos(QPointF pos, int angle)
{
    if(m_agvItem != nullptr)
    {
        m_agvItem->setPos(pos);
        QTransform transform = m_agvTransform;
        transform.rotate(-angle / 100);
        m_agvItem->setTransform(transform);
    }
}
