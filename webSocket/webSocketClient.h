#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QTimer>
#include "logger.h"

class WebSocketClient : public QObject, logger::Logger
{
    Q_OBJECT
public:
    WebSocketClient(QObject *paretn = nullptr);
    ~WebSocketClient();
    static WebSocketClient* getInstance();

    void openWebSocket(QString url);

    void setAgvId(int id);

    void setClickLogin(bool status);
private:
    void loginCheck();

    void reconnectWebSocket();

    void requestMapData(QVariantMap agvData);

    void initWebSocket();

private slots:
    void onDisconnected();
    void onConnected();

    void onTextReceived(QString data);

    void onTimeoutReconnect();
private:
    QWebSocket* m_webSocket;
    QTimer *m_timer;
    QUrl m_url;
    int m_timeoutCount; //3次超时则重连
    int m_agvId;
    bool m_clickLogin;
};

#endif // WEBSOCKETCLIENT_H
