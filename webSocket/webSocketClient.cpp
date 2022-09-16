#include "webSocketClient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "customData.h"
#include "mapView/mapMonitoringView.h"
#include "moduleData/configModule.h"
#include <QByteArray>
#include <QFile>
#include "moduleData/stationModule.h"
#include "login/widgetlogin.h"
#include "moduleData/taskModule.h"
#include "baseinfo/widgetbaseinfo.h"
#include "allinfo/widgetallinfo.h"

static WebSocketClient* s_webSocketClient = nullptr;

WebSocketClient::WebSocketClient(QObject *paretn /*= nullptr*/)
    :QObject(paretn)
    ,logger::Logger("WebSocket")
    ,m_webSocket(nullptr)
    ,m_timer(nullptr)
    ,m_timeoutCount(0)
    ,m_clickLogin(false)
{
    s_webSocketClient = this;

    m_timer = new QTimer();

    connect(m_timer, &QTimer::timeout, this, &WebSocketClient::onTimeoutReconnect);
}

WebSocketClient::~WebSocketClient()
{
    if(m_webSocket != nullptr)
    {
        m_webSocket->abort();
        m_webSocket->deleteLater();
        m_webSocket = nullptr;
    }

    if(m_timer != nullptr)
    {
        m_timer->stop();
        m_timer->deleteLater();
        m_timer = nullptr;
    }
}

WebSocketClient *WebSocketClient::getInstance()
{
    return s_webSocketClient;
}

void WebSocketClient::openWebSocket(QString url)
{
    initWebSocket();

    LOGGER_INFO(this)<<("openWebSocket:" + url.toStdString());

    if(m_webSocket != nullptr)
    {
        m_url = url;

        m_webSocket->abort();
        m_webSocket->open(m_url);
        if(m_timer != nullptr)
        {
            m_timer->start(1000);
        }
        m_timeoutCount = 0;
    }
}

void WebSocketClient::setAgvId(int id)
{
    m_agvId = id;
    ConfigModule::getInstance()->setAgvId(id);
}

void WebSocketClient::setClickLogin(bool status)
{
    m_clickLogin = status;
}

void WebSocketClient::sendDataToServer(const QString &data)
{
    m_webSocket->sendTextMessage(data);
}

void WebSocketClient::reconnectWebSocket()
{
    if(m_timer != nullptr)
    {
        m_timer->stop();
    }

    if(m_webSocket != nullptr)
    {
        m_webSocket->deleteLater();
        m_webSocket = nullptr;
    }


    openWebSocket(m_url.toString());
}

void WebSocketClient::loginCheck()
{
    QString mapJson = QString("{\"ModuleType\":%1,\"Async\":false,\"ModuleData\":{\"Content\":{\"agvId\":%2},\"OperationType\":%3},\"RequestId\":-1}").arg(int(DataModuleType::User)).arg(m_agvId).arg(int(LoginOperationType::LOGIN_CHECK));
    if(m_webSocket->isValid())
    {
        m_webSocket->sendTextMessage(mapJson);
    }
}

void WebSocketClient::requestMapData(QVariantMap agvData)
{
    int mapId = agvData.value("mapId").toInt();

    LOGGER_INFO(this)<<("requestMapData:" + mapId);

    QVariantMap mapData = ConfigModule::getInstance()->getConfig(ConfigType::Map, mapId);
    QVariantMap floorDatas = mapData.value("floorParams").toMap();
    int areaId = mapData.value("areaId").toInt();
    QList<QString> floorIds = floorDatas.keys();
    if(floorIds.size() > 0)
    {
        MapMonitoringView::getInstance()->setMapId(mapId);
        MapMonitoringView::getInstance()->setAreaId(areaId);
        MapMonitoringView::getInstance()->setFloorId(floorIds.at(0).toInt());
        MapMonitoringView::getInstance()->requestMapDataInfo(m_webSocket);
    }
}

void WebSocketClient::initWebSocket()
{
    m_webSocket = new QWebSocket();

    connect(m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextReceived);
}

void WebSocketClient::onDisconnected()
{

}

void WebSocketClient::onConnected()
{
    qDebug()<<"connected";

    loginCheck();

    if(m_clickLogin && WidgetLogin::getInstance() != nullptr)
    {
        WidgetLogin::getInstance()->loginSucceed();
        m_clickLogin = false;
    }
}

void WebSocketClient::onTextReceived(QString data)
{
    LOGGER_INFO(this)<<("receivedServerData:" + data.toStdString());

    m_timeoutCount = 0;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &error);
    if(error.error == QJsonParseError::NoError)
    {
        QJsonObject obj = doc.object();
        if(!obj.isEmpty())
        {
            QVariantMap jsonData = obj.toVariantMap();
            int moduleType = jsonData.value("ModuleType").toInt();
            int requestId = jsonData.value("RequestId").toInt();
            QVariantMap moduleData = jsonData.value("ModuleData").toMap();
            int operatorType = moduleData.value("OperationType").toInt();
            if(moduleType == -1)
            {
                if(requestId == RequestIdType::REQUEST_MAP_XML || requestId == RequestIdType::REQUEST_MAP_SLAM)
                {
                    MapMonitoringView::getInstance()->initMap(jsonData);
                }
                else if(requestId == RequestIdType::REQUEST_RELOCATION)
                {
                    MapMonitoringView::getInstance()->setRelocationResult(moduleData);
                }
            }
            else
            {
                switch (static_cast<DataModuleType>(moduleType))
                {
                case DataModuleType::Map:
                {
                    MapMonitoringView::getInstance()->initMap(jsonData);
                    break;
                }
                case DataModuleType::Config:
                {
//                    qDebug() << "configData: " <<data;
                    ConfigModule::getInstance()->initConfig(jsonData);
                    int type = moduleData.value("OperationType").toInt();
                    if(type == int(ConfigOperatedType::OPERATED_TYPE_INIT))
                    {
                        requestMapData(ConfigModule::getInstance()->getConfig(ConfigType::Agv, m_agvId));
                    }

                    break;
                }
                case DataModuleType::Task:
                {
//                    qDebug() << "taskData: " <<data;
                    TaskModule::getInstance()->updataTask(moduleData, m_agvId);
                    break;
                }
                case DataModuleType::Agv:
                {
//                    qDebug() << "agvData: " <<data;

                    WidgetBaseInfo::GetInstance()->InitData(jsonData);
                    WidgetAllInfo::GetInstance()->InitData(jsonData);
                    MapMonitoringView::getInstance()->updataAgvItemPos(moduleData);
                    break;
                }
                case DataModuleType::Station:
                {
                    int type = moduleData.value("OperationType").toInt();
                    if(type == int(StationOperationType::STATION_INITED) || type == int(StationOperationType::STATION_Updated))
                    {
                        StationModule::getInstance()->updataStation(moduleData);
                    }
                    else if(type == int(StationOperationType::CRAFTSTATION_INITED))
                    {
                        StationModule::getInstance()->initCraftStationTypeInfo(moduleData);
                    }
//                    qDebug() << "stationData: " <<data;
                    break;
                }
                }
            }
        }
    }
}

void WebSocketClient::onTimeoutReconnect()
{
    ++m_timeoutCount;

    if(WidgetLogin::getInstance() == nullptr)
    {
        if(m_webSocket != nullptr && m_webSocket->isValid())
        {
            QString heartbeatData = QString("{\"ModuleType\":%1}").arg(int(DataModuleType::HeartBeat));
            m_webSocket->sendTextMessage(heartbeatData);
        }
        else if(m_timeoutCount > 3)
        {
            reconnectWebSocket();
        }
    }
    else if(WidgetLogin::getInstance() != nullptr && m_clickLogin)
    {
        if(m_webSocket != nullptr && m_webSocket->isValid())
        {
            QString heartbeatData = QString("{\"ModuleType\":%1}").arg(int(DataModuleType::HeartBeat));
            m_webSocket->sendTextMessage(heartbeatData);
        }
        else if(m_timeoutCount > 3)
        {
            if(m_webSocket != nullptr)
            {
                m_webSocket->deleteLater();
                m_webSocket = nullptr;
            }

            if(m_timer != nullptr)
            {
                m_timer->stop();
            }

            m_timeoutCount = 0;
            WidgetLogin::getInstance()->setTipsServiceLinkStatus(false);
            m_clickLogin = false;
        }
    }
}
