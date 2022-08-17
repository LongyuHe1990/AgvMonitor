#include "mapMonitoringView.h"
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTransform>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QGridLayout>
#include "mapInfo.h"
#include "customData.h"
#include "moduleData/stationModule.h"

static MapMonitoringView* s_mapMonitoringView = nullptr;

MapMonitoringView::MapMonitoringView(QWidget * parent /*= nullptr*/)
    :QGraphicsView(parent)
    ,m_mapId(-1)
    ,m_floorId(-1)
    ,m_areaId(-1)
    ,m_scale(1.20)
    ,m_scene(nullptr)
    ,m_loadMapDataWatcher(nullptr)
    ,m_loadSlamWatcher(nullptr)
{
    s_mapMonitoringView = this;

    initWidget();
    /*setCacheMode(QGraphicsView::CacheBackground);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);//gpu渲染采用fullview
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setRenderHints(QPainter::Antialiasing);
    setViewport(new QGLWidget());*/

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    m_scene = new MapMonitoringScene();
    setScene(m_scene);
    setTransform(transform().scale(0.02,-0.02));
}

MapMonitoringView::~MapMonitoringView()
{
    if(m_scene!=nullptr)
    {
       m_scene = nullptr;
    }

    if(m_loadMapDataWatcher != nullptr)
    {
        if(m_loadMapDataWatcher->isRunning())
        {
            m_loadMapDataWatcher->cancel();
            m_loadMapDataWatcher->waitForFinished();
        }
        m_loadMapDataWatcher->deleteLater();
        m_loadMapDataWatcher = nullptr;
    }

    if(m_loadSlamWatcher != nullptr)
    {
        if(m_loadSlamWatcher->isRunning())
        {
            m_loadSlamWatcher->cancel();
            m_loadSlamWatcher->waitForFinished();
        }

        m_loadSlamWatcher->deleteLater();
        m_loadSlamWatcher = nullptr;
    }
}

MapMonitoringView *MapMonitoringView::getInstance()
{
    return s_mapMonitoringView;
}

void MapMonitoringView::setMapId(int mapId)
{
    m_mapId = mapId;
    QLabel* mapLabel = findChild<QLabel*>("labelMap");
    if(mapLabel != nullptr)
    {
        QString mapName = "地图" + QString::number(mapId);
        mapLabel->setText(mapName);
    }
}

void MapMonitoringView::setFloorId(int floorId)
{
    m_floorId = floorId;
}

void MapMonitoringView::setAreaId(int areaId)
{
    m_areaId = areaId;
    QLabel* areaLabel = findChild<QLabel*>("labelArea");
    if(areaLabel != nullptr)
    {
        QString areaName = "区域" + QString::number(areaId);
        areaLabel->setText(areaName);
    }
}

void MapMonitoringView::initMap(QVariantMap data)
{
    int type = data.value("RequestId").toInt();
    switch(static_cast<GetMapType>(type))
    {
    case GetMapType::MAP_TYPE_XML:
    {
        loadMapData(data);
        break;
    }
    case GetMapType::MAP_TYPE_SLAM:
    {
        loadSlamDataToImagee(data);
        break;
    }
    }
}

void MapMonitoringView::requestMapDataInfo(QWebSocket* client)
{
    if(client != nullptr)
    {
        QString mapJson = QString("{\"ModuleType\":%1,\"Async\":false,\"ModuleData\":{\"Content\":{\"mapId\":%2,\"floorId\":%3,\"type\":%4},\"OperationType\":%5},\"RequestId\":%6}").arg(int(DataModuleType::Map)).arg(m_mapId).arg(m_floorId).arg(int(MapFileType::XML)).arg(int(MapOperationType::MAP_GETMAP)).arg(int(GetMapType::MAP_TYPE_XML));
        client->sendTextMessage(mapJson);

        QString slamJson = QString("{\"ModuleType\":%1,\"Async\":false,\"ModuleData\":{\"Content\":{\"mapId\":%2,\"floorId\":%3,\"type\":%4},\"OperationType\":%5},\"RequestId\":%6}").arg(int(DataModuleType::Map)).arg(m_mapId).arg(m_floorId).arg(int(MapFileType::PNG)).arg(int(MapOperationType::MAP_GETMAP)).arg(int(GetMapType::MAP_TYPE_SLAM));
        client->sendTextMessage(slamJson);
    }
}

void MapMonitoringView::updataAgvItemPos(QVariantMap data)
{
    int operatorType = data.value("OperationType").toInt();
    if(operatorType == int(AgvOperationType::AGV_STATUS_UPDATED))
    {
        QVariantMap content = data.value("Content").toMap();
        QVariantMap posData = content.value("globalPos").toMap();
        QPoint pos(posData.value("x").toInt(), posData.value("y").toInt());
        int angle = content.value("angle").toInt();
        if(m_scene != nullptr)
        {
            m_scene->updataAgvItemPos(pos, angle);
        }
    }
}

void MapMonitoringView::loadMapData(QVariantMap data)
{
    if(m_loadMapDataWatcher != nullptr && m_loadMapDataWatcher->isRunning())
    {
        m_loadMapDataWatcher->cancel();
        m_loadMapDataWatcher->waitForFinished();
    }

    QFuture<void> future = QtConcurrent::run([=](){
        QMutexLocker locker(&m_loadMapDataMutex);

        QVariantMap jsonMap = data;
        QVariantMap modeuleData = jsonMap.value("ModuleData").toMap();
        QVariantMap content = modeuleData.value("Content").toMap();
        QString xml = content.value("data").toString();

        QDomDocument doc;
        doc.setContent(xml);
        QDomElement root = doc.documentElement();
        QDomNode node = root.firstChild();

        while(!node.isNull())
        {
            QDomNodeList sonList = node.childNodes();
            QString nodeName = node.toElement().tagName();
            if (nodeName == "NodeInfo")
            {
                readNodeXml(sonList);
            }
            else if(nodeName == "LineInfo")
            {
                readLineXml(sonList);
            }
            else if(nodeName == "StationInfo")
            {
             readStationXml(sonList);
            }

            node = node.nextSibling();
        }
    });

    if(m_loadMapDataWatcher != nullptr)
    {
        m_loadMapDataWatcher->deleteLater();
        m_loadMapDataWatcher = nullptr;
    }
    m_loadMapDataWatcher = new QFutureWatcher<void>(this);
    connect(m_loadMapDataWatcher, &QFutureWatcher<void>::finished,[&](){
        qDebug()<<"load: loadMapDataFinished";
        addMapToScene();
        centeredShowMapToView();
    });
    m_loadMapDataWatcher->setFuture(future);
}

void MapMonitoringView::loadSlamDataToImagee(QVariantMap data)
{
    if(m_loadSlamWatcher != nullptr && m_loadSlamWatcher->isRunning())
    {
        m_loadSlamWatcher->cancel();
        m_loadSlamWatcher->waitForFinished();
    }

    QFuture<void> future = QtConcurrent::run([=](){
        QMutexLocker locker(&m_loadSlamMutex);

        QVariantMap jsonMap = data;
        QVariantMap modeuleData = jsonMap.value("ModuleData").toMap();
        QVariantMap content = modeuleData.value("Content").toMap();
        QString pngData = content.value("data").toString();
        QVariantMap geometryJson = content.value("geometry").toMap();
        int height = geometryJson.value("height").toInt();
        int width = geometryJson.value("width").toInt();
        int x = geometryJson.value("x").toInt();
        int y = geometryJson.value("y").toInt();
        m_slamRect = QRect(x,y,width,height);

        QByteArray arr_base64 = pngData.toLatin1();
        m_slamImage.loadFromData(QByteArray::fromBase64(arr_base64));
    });

    if(m_loadSlamWatcher != nullptr)
    {
        m_loadSlamWatcher->deleteLater();
        m_loadSlamWatcher = nullptr;
    }
    m_loadSlamWatcher = new QFutureWatcher<void>(this);
    connect(m_loadSlamWatcher, &QFutureWatcher<void>::finished, this, [&](){
        addSlamToScene();
        centeredShowMapToView();
    });
    m_loadSlamWatcher->setFuture(future);
}

void MapMonitoringView::addMapToScene()
{
    if(m_scene != nullptr)
    {
        m_scene->addNodeItems(m_nodes);
        m_scene->addLineItems(m_lines);
        m_scene->addStationItems(m_stations);
    }
}

void MapMonitoringView::addSlamToScene()
{
    if(m_scene != nullptr)
    {
        m_scene->addSlamItem(m_slamImage, m_slamRect);
    }
}

void MapMonitoringView::readNodeXml(QDomNodeList nodeList)
{
    m_nodes.clear();
    for(int i = 0; i < nodeList.size(); ++i)
    {
        QDomNode sonNode = nodeList.at(i);
        if(sonNode.nodeType() != QDomNode::CommentNode)
        {
            NODEINFO nodeInfo;
            double x = 0.000;
            double y = 0.000;
            QDomElement sonElement = sonNode.toElement();

            QString id = sonElement.attribute("id");
            nodeInfo.id = id;

            QDomNode sonNode = sonElement.firstChild();

            while(!sonNode.isNull())
            {
                QDomElement childElement = sonNode.toElement();
                QString childName = childElement.tagName();
                if(childName.compare("x") == 0)
                {
                    x = childElement.text().toDouble();
                }
                else if(childName.compare("y") == 0)
                {
                    y = childElement.text().toDouble();
                }

                sonNode = sonNode.nextSibling();
            }

            nodeInfo.point = QPointF(x,y);
            m_nodes.insert(id, nodeInfo);
        }
        else{
            QDomComment comment = sonNode.toComment();
            QString strComment = comment.data();
        }
    }
}

void MapMonitoringView::readLineXml(QDomNodeList lineList)
{
    m_lines.clear();
    for(int i = 0; i<lineList.size(); ++i)
    {
        QDomNode sonNode = lineList.at(i);

        if(sonNode.nodeType() != QDomNode::CommentNode)
        {
            QDomElement sonElement = sonNode.toElement();
            QString dd = sonElement.tagName();
            LINEINFO lineInfo;

            QString id = sonElement.attribute("id");
            lineInfo.id = id;

            QDomNode childNode = sonElement.firstChild();

            while (!childNode.isNull()) {

                QDomElement childElement = childNode.toElement();
                QString childName = childElement.tagName();
                if(childName.compare("startNodeId") == 0)
                {
                    QString nodeId = childElement.text();
                    if(m_nodes.find(nodeId) != m_nodes.end())
                    {
                        NODEINFO node = m_nodes.find(nodeId).value();
                        lineInfo.startNode = node;
                    }
                }
                else if(childName.compare("endNodeId") == 0)
                {
                    QString nodeId = childElement.text();
                    if(m_nodes.find(nodeId) != m_nodes.end())
                    {
                        NODEINFO node = m_nodes.find(nodeId).value();
                        lineInfo.endNode = node;
                    }
                }

                childNode = childNode.nextSibling();
            }

            m_lines.insert(id, lineInfo);
        }
        else{
            QDomComment comment = sonNode.toComment();
            QString strComment = comment.data();
        }
    }
}

void MapMonitoringView::readStationXml(QDomNodeList stationList)
{
    for(int i = 0; i<stationList.size(); ++i)
    {
        QDomNode sonNode = stationList.at(i);

        if(sonNode.nodeType() != QDomNode::CommentNode)
        {
            STATIONINFO stationInfo;
            QDomElement sonElement = sonNode.toElement();
            QString id = sonElement.attribute("id");
            stationInfo.id = id;

            QDomNode childNode = sonElement.firstChild();
            double x = 0.000;
            double y = 0.000;
            while(!childNode.isNull())
            {
               QDomElement childElement = childNode.toElement();
               if(childElement.tagName().compare("x") == 0)
               {
                   x = childElement.text().toDouble();
               }
               else if(childElement.tagName().compare("y") == 0)
               {
                   y = childElement.text().toDouble();
               }
               else if(childElement.tagName().compare("stationName") == 0)
               {
                   QString name = childElement.text();
                   stationInfo.name = name;
               }
               else if(childElement.tagName().compare("nodeId") == 0)
               {
                   QString id = childElement.text();
                   if(m_nodes.find(id) != m_nodes.end())
                   {
                       NODEINFO nodeInfo = m_nodes.find(id).value();
                       stationInfo.node = nodeInfo;
                   }
               }
               else if(childElement.tagName().compare("stationType") == 0)
               {
                   QString stationTypeId = childElement.text();
                   stationInfo.stationTypeId = stationTypeId.toInt();
               }
               else if(childElement.tagName().compare("rowNum") == 0)
               {
                   QString rowNum = childElement.text();
                   stationInfo.row = rowNum.toInt();
               }
               else if(childElement.tagName().compare("columnNum") == 0)
               {
                   QString columnNum = childElement.text();
                   stationInfo.column = columnNum.toInt();
               }
               childNode = childNode.nextSibling();
            }

            stationInfo.point = QPointF(x,y);

            StationModule::getInstance()->initStationFromMap(id.toInt(), stationInfo);

            m_stations.insert(id, stationInfo);
        }
        else
        {
            QDomComment comment = sonNode.toComment();
            QString strComment = comment.data();
        }
    }
}

void MapMonitoringView::centeredShowMapToView()
{
    if(m_scene->getSlamItem() != nullptr)
    {
        QRectF rect = m_scene->getSlamItem()->boundingRect();
        rect.adjust(-1000,-1000,1000,1000);
        fitInView(rect, Qt::KeepAspectRatio);
        centerOn(m_scene->getSlamItem());
    }
    else
    {
        QRectF rect = m_scene->itemsBoundingRect();
        rect.adjust(-1000,-1000,1000,1000);
        fitInView(rect, Qt::KeepAspectRatio);
        centerOn(rect.center());
    }
}

void MapMonitoringView::initWidget()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout* titleLayout = new QHBoxLayout(this);
    titleLayout->setSizeConstraint(QLayout::SetFixedSize);

    QFont font("微软雅黑", 13);
    QLabel* imageArea = new QLabel(this);
    imageArea->resize(30,30);
    QPixmap areaImage(30,30);
    areaImage.load(":/image/area.png");
    areaImage = areaImage.scaled(30,30);
    imageArea->setPixmap(areaImage);
    titleLayout->addWidget(imageArea);
    QLabel *labelArea = new QLabel(this);
    labelArea->setStyleSheet("color:white;");
    labelArea->setFont(font);
    QString areaName = "区域" + QString::number(m_areaId);
    labelArea->setText(areaName);
    labelArea->setObjectName("labelArea");
    titleLayout->addWidget(labelArea);

    titleLayout->addSpacing(50);

    QLabel* imageMap = new QLabel(this);
    imageMap->resize(30,30);
    QPixmap mapImage(30,30);
    mapImage.load(":/image/map.png");
    mapImage = mapImage.scaled(25,25);
    imageMap->setPixmap(mapImage);
    titleLayout->addWidget(imageMap);
    QLabel *labelMap = new QLabel(this);
    QString mapName = "地图" + QString::number(m_mapId);
    labelMap->setText(mapName);
    labelMap->setObjectName("labelMap");
    labelMap->setFont(font);
    labelMap->setStyleSheet("color:white");
    titleLayout->addWidget(labelMap);
    titleLayout->addStretch();

    mainLayout->addLayout(titleLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void MapMonitoringView::zoomIn(QPoint pos)
{
    QPointF cursorPoint = pos;
    QPointF scenePos = this->mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));

    setTransform(transform().scale(m_scale, m_scale));

    QPointF viewPoint = this->transform().map(scenePos);
    horizontalScrollBar()->setValue(int(viewPoint.x() - cursorPoint.x()));
    verticalScrollBar()->setValue(int(viewPoint.y() - cursorPoint.y()));
}

void MapMonitoringView::zoomOut(QPoint pos)
{
    QPointF cursorPoint = pos;
    QPointF scenePos = this->mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));

    scale(1.00 / m_scale, 1.00 / m_scale);

    QPointF viewPoint = this->matrix().map(scenePos);

    horizontalScrollBar()->setValue(int(viewPoint.x() - cursorPoint.x()));
    verticalScrollBar()->setValue(int(viewPoint.y() - cursorPoint.y()));
}

void MapMonitoringView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pressPoint = event->pos();
        m_leftMousePress = true;
    }
    QGraphicsView::mousePressEvent(event);
}

void MapMonitoringView::mouseMoveEvent(QMouseEvent *event)
{
    /*if(m_leftMousePress)
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + (m_pressPoint.y() - event->pos().y()));
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + (m_pressPoint.x() - event->pos().x()));
        m_pressPoint = event->pos();
    }*/

    QGraphicsView::mouseMoveEvent(event);
}

void MapMonitoringView::mouseReleaseEvent(QMouseEvent *event)
{
    m_leftMousePress = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void MapMonitoringView::wheelEvent(QWheelEvent *event)
{
    double angle = event->angleDelta().y() / 120;
    if(angle > 0)
    {
        zoomIn(event->pos());
    }
    else
    {
        zoomOut(event->pos());
    }

    //QGraphicsView::wheelEvent(event);
}

void MapMonitoringView::resizeEvent(QResizeEvent *event)
{
    centeredShowMapToView();
    QGraphicsView::resizeEvent(event);
}
