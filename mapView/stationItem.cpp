#include "stationItem.h"
#include <QPainter>
#include <QTransform>
#include <QGraphicsScene>
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include "common/widgetmessagebox.h"
#include "common/global_config.h"
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"
#include "common/global_helper.h"
#include "webSocket/webSocketClient.h"
#include <QJsonDocument>
#include <qjsonobject.h>

StationItem::StationItem(QGraphicsItem *parent/* = nullptr*/)
    :QGraphicsItem(parent)
    ,m_linkLine(nullptr)
    ,m_type(STATION_COMMON)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    m_linkLine = new LinkLine();

    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, [=](){
        if(WidgetMessageBox().MessageHint(tr("任务"), tr("创建【%1】站点任务。").arg(m_stationInfo.name)))
        {
            createTask();
        }
        m_timer->stop();
    });
}

StationItem::~StationItem()
{
    if(m_linkLine != nullptr)
    {
        m_linkLine = nullptr;
    }

    if(m_timer != nullptr)
    {
        m_timer->deleteLater();
        m_timer = nullptr;
    }
}

void StationItem::setStationPoint(QPoint point)
{
    setPos(point);
    m_stationPoint = mapFromScene(point);
    m_linkLine->setStartPoint(point);
}

QPointF StationItem::getStationPoint()
{
    return m_stationPoint;
}

void StationItem::setAssociateNodePoint(QPoint point)
{
    m_associateNodePoint = mapFromScene(point);
    m_linkLine->setEndPoint(point);
}

QPointF StationItem::getAssoicateNodePoint()
{
    return m_associateNodePoint;
}

void StationItem::showLinkLine(bool show /*=true*/)
{
    if(scene() != nullptr)
    {
        if(show)
        {
            scene()->addItem(m_linkLine);
        }
        else{
            scene()->removeItem(m_linkLine);
        }
    }
}

void StationItem::setStationInfo(STATIONINFO info)
{
    m_stationInfo = info;
}

void StationItem::setStationType(StationType type)
{
    m_type = type;
}

void StationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_timer->start(2000);
    event->accept();
}

void StationItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_timer->stop();
    QGraphicsItem::mouseReleaseEvent(event);
}

QRectF StationItem::boundingRect() const
{
    return m_boundingRect;
}

void StationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRect imageRect(-10,-10,20,20);
    painter->setPen(QPen(Qt::green));

    QImage image;
    switch (m_type)
    {
        case STATION_CHARGING:
        {
            image.load(":/image/charge_station.png");
            break;
        }
        case STATION_WAITPOINT:
        {
            image.load(":/image/park_station.png");
            break;
        }
        case STATION_COMMON:
        {
            image.load(":/image/common_station.png");
            break;
        }
    }
    painter->drawImage(imageRect, image);


    QFont font = QApplication::font();
    QFontMetrics metrics(font);
    int nameHeight = metrics.height();
    int nameWidth = metrics.width(m_stationInfo.name);
    QRect titleRect(-(nameWidth / 2), imageRect.bottomLeft().y(), nameWidth, nameHeight);
    painter->setPen(QPen("#1afa29"));
    painter->setBrush(QBrush("#1afa29"));
    painter->drawText(titleRect, m_stationInfo.name);

    m_boundingRect = imageRect;
    m_boundingRect = m_boundingRect | titleRect;
}

void StationItem::createTask()
{
    QVariantMap dataMap;
    dataMap.insert("agvId", UserConfigs::AgvId);
    dataMap.insert("maxLoopTimes", 0);
    dataMap.insert("mapId", ConfigModule::getInstance()->getAgvBelongMapId(UserConfigs::AgvId));
    dataMap.insert("taskNum", UserConfigs::Uuid);

    QVariantList stationList;

    QVariantMap    actionListMap;
    int stationTypeId = m_stationInfo.stationTypeId;
    QVariantList actionList = ConfigModule::getInstance()->getActionTypeOfStationType(stationTypeId);
    int actionType = 0;
    if(actionList.size() > 0)
    {
        actionType = actionList.at(0).toInt();
    }
    NameAndLabelInfo info = GetActionType(actionType);
    actionListMap.insert("action", info.name);

    QVariantMap config = ConfigModule::getInstance()->getConfig(ConfigType::Agv, UserConfigs::AgvId);
    int agvTypeId = config.value("agvTypeId").toInt();
    QVariantMap position = ConfigModule::getInstance()->getConfig(ConfigType::AgvType, agvTypeId);
    QVariantList materialBufferParamList = position.value("materialBufferParamList").toList();
    int agvBufferIndex = 0;
    if(materialBufferParamList.size() > 0)
    {
        QVariantMap map = materialBufferParamList.at(0).toMap();
        agvBufferIndex = map.value("id").toInt();
    }
    actionListMap.insert("agvBufferIndex", agvBufferIndex);


    QVariantMap   stationLevel = ConfigModule::getInstance()->getMaterialBufferParams(stationTypeId);
    QVariantMap::iterator iter = stationLevel.begin();
    int stationBufferIndex = 0;
    if(iter != stationLevel.end())
    {
       QVariantMap map = iter.value().toMap();
       stationBufferIndex = map.value("id").toInt();
    }
    actionListMap.insert("stationBufferIndex", stationBufferIndex);
    actionList.insert(0, actionListMap);
    QVariantMap stationListMap;

    stationListMap.insert("actionList", actionList);
    stationListMap.insert("stationId", m_stationInfo.id);
    stationListMap.insert("stationName", m_stationInfo.name);

    stationList.insert(0, stationListMap);

    dataMap.insert("stationList", stationList);

    QVariantMap Content;
    Content.insert("Content", dataMap);
    Content.insert("OperationType", int(StationOperationType::Update_Station_Material));
    //
    QVariantMap sendData;
    sendData.insert("ModuleType", int(DataModuleType::Task));
    sendData.insert("Async", false);
    sendData.insert("ModuleData", Content);
    sendData.insert("RequestId", int(RequestIdType::REQUEST_CREATE_TASK));

    QJsonObject   obj = QJsonObject::fromVariantMap(sendData);
    QJsonDocument doc(obj);
    QString       strRet = QString(doc.toJson(QJsonDocument::Indented));

    //send request*
    WebSocketClient::getInstance()->sendDataToServer(strRet);
}
