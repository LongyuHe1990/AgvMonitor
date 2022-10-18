#include "mapMonitoringScene.h"
#include "stationItem.h"
#include "runningRouteLine.h"
#include "nodeItem.h"
#include <QGraphicsView>
#include "moduleData/configModule.h"
#include "common/global_config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "liftItem.h"

MapMonitoringScene::MapMonitoringScene()
    :m_slamItem(nullptr)
    ,m_agvItem(nullptr)
{
    setBackgroundBrush(QBrush("#0b1d2e"));

    setSceneRect(-500000,-500000,1000000,1000000);
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

void MapMonitoringScene::addLiftItems(QMap<QString, LIFTINFO> lifts)
{
    for(auto lift: lifts)
    {
        LiftItem *liftItem = new LiftItem();
        liftItem->setLiftPoint(lift.node.point.toPoint());
        addItem(liftItem);
    }
}

void MapMonitoringScene::addSlamItem(const QImage &image, QRect rect)
{
    if(!image.isNull() && !rect.isNull())
    {
        if(m_slamItem != nullptr)
        {
            m_slamItem = nullptr;
        }
        m_slamItem = new SlamItem();
        m_slamItem->setImageRect(rect, image);
        addItem(m_slamItem);
    }
}

void MapMonitoringScene::addAgvItem(int floor)
{
    if(m_agvItem != nullptr)
    {
        m_agvItem = nullptr;
    }

    m_agvItem = new AgvItem();
    addItem(m_agvItem);
    m_agvTransform = m_agvItem->transform();
}

void MapMonitoringScene::clearAllItem()
{
    clear();
    if(m_slamItem != nullptr)
    {
        m_slamItem = nullptr;
    }
    if(m_agvItem != nullptr)
    {
        m_agvItem = nullptr;
    }
}

void MapMonitoringScene::updataAgvItemPos(QPointF pos, int angle, bool show)
{
    if(m_agvItem != nullptr)
    {
        if(show)
        {
            m_agvItem->show();
        }
        else
        {
            m_agvItem->hide();
        }

        m_agvItem->setPos(pos);
        QTransform transform = m_agvTransform;
        transform.rotate(360 - angle / 100.0 + 90);
        m_agvItem->setTransform(transform);
    }
}
