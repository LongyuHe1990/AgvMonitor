#include "mapMonitoringView.h"
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTransform>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGridLayout>
#include "mapInfo.h"
#include "customData.h"
#include "moduleData/stationModule.h"
#include <QPainter>
#include "webSocket/webSocketClient.h"
#include "common/global_config.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QComboBox>
#include <QListView>
#include <QLineEdit>
#include "moduleData/configModule.h"

static MapMonitoringView* s_mapMonitoringView = nullptr;

MapMonitoringView::MapMonitoringView(QWidget * parent /*= nullptr*/)
    :QGraphicsView(parent)
    ,m_mapId(-1)
    ,m_floorId(-1)
    ,m_scale(1.20)
    ,m_scene(nullptr)
    ,m_loadMapDataWatcher(nullptr)
    ,m_loadSlamWatcher(nullptr)
    ,m_isRelocation(false)
    ,m_titleRelocDlg(nullptr)
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
    setMouseTracking(true);
    m_scene = new MapMonitoringScene();
    setScene(m_scene);
    setTransform(transform().scale(0.02,-0.02));

    m_img.load(":/image/nav.png");

    setStyleSheet("padding:0px;border:0px");
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

    m_relocButton=nullptr;
    m_titleRelocDlg=nullptr;
}

MapMonitoringView *MapMonitoringView::getInstance()
{
    return s_mapMonitoringView;
}

void MapMonitoringView::setMapId(int mapId)
{
    m_mapId = mapId;
}

void MapMonitoringView::setFloorId(int floorId)
{
    m_floorId = floorId;
}

void MapMonitoringView::initMap(QVariantMap data)
{
    int type = data.value("RequestId").toInt();
    switch(static_cast<RequestIdType>(type))
    {
    case RequestIdType::REQUEST_MAP_XML:
    {
        loadMapData(data);
        break;
    }
    case RequestIdType::REQUEST_MAP_SLAM:
    {
        loadSlamDataToImagee(data);
        break;
    }
    }
    update();
}

void MapMonitoringView::requestMapDataInfo()
{
    if(WebSocketClient::getInstance() != nullptr)
    {
        QString mapJson = QString("{\"ModuleType\":%1,\"Async\":false,\"ModuleData\":{\"Content\":{\"mapId\":%2,\"floorId\":%3,\"type\":%4},\"OperationType\":%5},\"RequestId\":%6}").arg(int(DataModuleType::Map)).arg(m_mapId).arg(m_floorId).arg(int(MapFileType::XML)).arg(int(MapOperationType::MAP_GETMAP)).arg(int(RequestIdType::REQUEST_MAP_XML));
        WebSocketClient::getInstance()->sendDataToServer(mapJson);

        QString slamJson = QString("{\"ModuleType\":%1,\"Async\":false,\"ModuleData\":{\"Content\":{\"mapId\":%2,\"floorId\":%3,\"type\":%4},\"OperationType\":%5},\"RequestId\":%6}").arg(int(DataModuleType::Map)).arg(m_mapId).arg(m_floorId).arg(int(MapFileType::PNG)).arg(int(MapOperationType::MAP_GETMAP)).arg(int(RequestIdType::REQUEST_MAP_SLAM));
        WebSocketClient::getInstance()->sendDataToServer(slamJson);
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
        int floorId = posData.value("z").toInt();
        int angle = content.value("angle").toInt();
        if(m_scene != nullptr)
        {
            m_scene->updataAgvItemPos(pos, angle, m_floorId == floorId);
        }
    }

    m_agvInfor = data;
}

void MapMonitoringView::setRelocationResult(QVariantMap moduleData)
{
    int status = moduleData.value("Result").toInt();
    QMessageBox *msgtest;
    msgtest = new QMessageBox(this);
    msgtest->setWindowTitle("重定位");

    if(status == 0)
    {
        msgtest->setText("重定位成功!");
    }
    else if(status == -2)
    {
        msgtest->setText("非手动模式!");
    }
    else if(status == 1)
    {
        msgtest->setText("车体定位失败!");
    }
    else if(status == -1)
    {
        msgtest->setText("车体断开链接!");
    }
    msgtest->show();
}

void MapMonitoringView::loadMapData(QVariantMap data)
{
    m_nodes.clear();
    m_lines.clear();
    m_stations.clear();
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
            else if(nodeName == "LiftInfo")
            {
                readLiftXml(sonList);
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
        QImage image;
        image.loadFromData(QByteArray::fromBase64(arr_base64));
        m_slamImage = image;
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
        m_scene->addLiftItems(m_lifts);
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

            m_stations.insert(id, stationInfo);
        }
        else
        {
            QDomComment comment = sonNode.toComment();
            QString strComment = comment.data();
        }
    }
}

void MapMonitoringView::readLiftXml(QDomNodeList liftList)
{
    m_lifts.clear();
    for(int i = 0; i < liftList.size(); ++i)
    {
        QDomNode sonNode = liftList.at(i);
        if(sonNode.nodeType() != QDomNode::CommentNode)
        {
            LIFTINFO liftInfo;
            double x = 0.000;
            double y = 0.000;
            QDomElement sonElement = sonNode.toElement();

            QString id = sonElement.attribute("id");
            liftInfo.id = id;

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
                else if(childName.compare("nodeId") == 0)
                {
                    QString id = childElement.text();
                    if(m_nodes.find(id) != m_nodes.end())
                    {
                        NODEINFO nodeInfo = m_nodes.find(id).value();
                        liftInfo.node = nodeInfo;
                    }
                }

                sonNode = sonNode.nextSibling();
            }

            liftInfo.point = QPointF(x,y);

            m_lifts.insert(id, liftInfo);
        }
        else{
            QDomComment comment = sonNode.toComment();
            QString strComment = comment.data();
        }
    }
}

void MapMonitoringView::centeredShowMapToView()
{
    QRectF rect;
    if(m_scene->getSlamItem() != nullptr)
    {
        rect = m_scene->getSlamItem()->boundingRect();
        /*int toolHeight = qCeil(rect.height() / height()) * 50;
        rect.adjust(0,0,0,toolHeight);*/
        fitInView(rect, Qt::KeepAspectRatio);
        centerOn(m_scene->getSlamItem());
    }
    else
    {
        rect = m_scene->itemsBoundingRect();
        /*int toolHeight = (rect.height() / height()) * 50;
        rect.adjust(0,0,0,toolHeight);*/
        fitInView(rect, Qt::KeepAspectRatio);
        centerOn(rect.center());
    }
}

void MapMonitoringView::initWidget()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addStretch();

    QHBoxLayout* operateLayout = new QHBoxLayout(this);
    operateLayout->addStretch();
    m_relocButton = new QPushButton(this);
    QIcon icon(":/image/relocation.png");
    m_relocButton->setIcon(icon);
    m_relocButton->setIconSize(QSize(50,50));
    m_relocButton->setStyleSheet("background-color:transparent");
    m_relocButton->setMaximumSize(60,60);
    m_relocButton->setMinimumSize(60,60);
    operateLayout->addWidget(m_relocButton);
    connect(m_relocButton, &QPushButton::clicked, this, &MapMonitoringView::onClickRelocButton);

    mainLayout->addLayout(operateLayout);

    setLayout(mainLayout);
}

void MapMonitoringView::sendRelocationDataToServer()
{
    //{"ModuleType":1,"Async":false,"ModuleData":{"Content":{"posX":15644.018181818183,"posY":2013.7363636363666,"angle":6.52,"id":6,"posZ":1},"OperationType":7},"RequestId":1}
    QVariantMap rootMap;
    QVariantMap contentMap;
    QPointF mapPoint = mapToScene(m_pressPoint.x(), m_pressPoint.y());
    contentMap.insert("posX",mapPoint.x());
    contentMap.insert("posY", mapPoint.y());
    contentMap.insert("angle", m_angle);
    contentMap.insert("id", UserConfigs::AgvId);
    contentMap.insert("posZ", m_floorId);
    QVariantMap moduleData;
    moduleData.insert("Content", contentMap);
    moduleData.insert("OperationType", static_cast<int>(AgvOperationType::AGV_RELOC));
    rootMap.insert("ModuleType", static_cast<int>(DataModuleType::Agv));
    rootMap.insert("Async",false);
    rootMap.insert("ModuleData", moduleData);
    rootMap.insert("RequestId", RequestIdType::REQUEST_RELOCATION);

    QJsonDocument doc = QJsonDocument::fromVariant(QVariant(rootMap));
    QString data = doc.toJson();
    WebSocketClient::getInstance()->sendDataToServer(data);
}

void MapMonitoringView::showFloorInfo(int floor)
{
    m_scene->clearAllItem();
    m_scene->addAgvItem(floor);
    setFloorId(floor);
    requestMapDataInfo();
}

void MapMonitoringView::updataAgvConfidence(QVariantMap data)
{
    if(m_titleRelocDlg)
    {
        int operatorType = data.value("OperationType").toInt();
        if(operatorType == int(AgvOperationType::AGV_STATUS_UPDATED))
        {
            QVariantMap content = data.value("Content").toMap();
            QVariantMap posData = content.value("globalPos").toMap();
            QPoint pos(posData.value("x").toInt(), posData.value("y").toInt());
            int floorId = posData.value("z").toInt();

            int confidence = content.value("confidence").toInt();

            if(m_titleRelocDlg)
            {
                QLabel *cLabel = m_titleRelocDlg->findChild<QLabel*>("confidenceLabel");
                QLabel *pLabel = m_titleRelocDlg->findChild<QLabel*>("positLabel");
                if(pLabel)
                {
                    QString strPos = QString("x:%1 y:%2 z:%3").arg(QString::number(pos.x())).arg(QString::number(pos.y())).arg(floorId);
                    pLabel->setText(strPos);
                }

                if(cLabel)
                {
                    QString con = QString("%1").arg(confidence);
                    cLabel->setText(con);
                }
            }
        }
    }
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

    if(m_isRelocation && m_leftMousePress)
    {
        m_lastPoint = event->pos();
        m_line.setPoints(m_pressPoint, m_lastPoint);
        double angle = m_line.angle();
        m_angle = m_line.dy() > 0 ? -(360 - angle) : angle;
        qDebug()<<"dy:"<<m_line.dy()<<"angle1:"<<angle<<"angle2:"<<m_angle;
        this->viewport()->update();
    }

    QGraphicsItem *item = itemAt(event->pos());
    if(m_scene)
    {
        if(!m_titleRelocDlg && m_isRelocation && item == m_scene->getAgvItem())
        {
            m_titleRelocDlg = new QDialog(this);
            m_titleRelocDlg->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
            QVBoxLayout* parentLayout = new QVBoxLayout();
            QHBoxLayout* zxdLayout = new QHBoxLayout();
            QLabel* zxdLabel1 = new QLabel();
            zxdLabel1->setText(tr("置信度:"));
            QLabel* zxdLabel2 = new QLabel();
            zxdLabel2->setObjectName("confidenceLabel");
            zxdLayout->addWidget(zxdLabel1);
            zxdLayout->addWidget(zxdLabel2);
            parentLayout->addLayout(zxdLayout);

            QHBoxLayout* positLayout = new QHBoxLayout();
            QLabel* positLabel1 = new QLabel();
            positLabel1->setText(tr("位置:"));
            QLabel* positLabel2 = new QLabel();
            positLabel2->setObjectName("positLabel");
            positLayout->addWidget(positLabel1);
            positLayout->addWidget(positLabel2);
            parentLayout->addLayout(positLayout);

            m_titleRelocDlg->setLayout(parentLayout);
            m_titleRelocDlg->setGeometry(event->globalX(), event->globalY(), 200, 100);
            m_titleRelocDlg->show();
            updataAgvConfidence(m_agvInfor);
        }

        if(item != m_scene->getAgvItem())
        {
            if(m_titleRelocDlg)
            {
                delete m_titleRelocDlg;
                m_titleRelocDlg = nullptr;
            }
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MapMonitoringView::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_leftMousePress && m_isRelocation)
    {
        sendRelocationDataToServer();
    }
    m_leftMousePress = false;
    m_line = QLineF();
    this->viewport()->update();
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

void MapMonitoringView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

    QPainter painter(this->viewport());
    painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setBrush(Qt::white);
    if (!m_isRelocation || !m_leftMousePress)
    {
        return;
    }
    QPen pen(QColor("#f8b62b"), 2, Qt::DashLine);
    painter.setPen(pen);
    painter.drawLine(m_line);
    painter.drawText(m_lastPoint + QPoint(10, 10), QString(u8"%1°").arg(QString::number(m_angle, 'f', 2)));
    painter.translate(m_pressPoint);
    painter.rotate(-m_angle);
    QRect rect = QRect( - 30 / 2, - 30 / 2, 30, 30);
    painter.drawPixmap(rect, m_img);
}

void MapMonitoringView::onClickRelocButton()
{
    if(!m_isRelocation)
    {
        m_isRelocation = true;
        setDragMode(QGraphicsView::NoDrag);
        QIcon icon(":/image/showStation.png");
        m_relocButton->setIcon(icon);
    }
    else
    {
        m_isRelocation=false;
        setDragMode(QGraphicsView::ScrollHandDrag);
        QIcon icon(":/image/relocation.png");
        m_relocButton->setIcon(icon);
    }
}
