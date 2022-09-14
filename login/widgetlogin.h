/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include <QWidget>
#include <QEventLoop>
#include <QUrl>
#include "webSocket/webSocketClient.h"
#include "logger.h"

namespace Ui {
class WidgetLogin;
}

class WidgetLogin : public QWidget, logger::Logger
{
  Q_OBJECT

public:
  explicit WidgetLogin(QWidget* parent = nullptr);
  ~WidgetLogin();

    static WidgetLogin* getInstance();
    void loginSucceed();
    void setTipsServiceLinkStatus(bool status);

  void SetEventLoop(QEventLoop* loop);

private Q_SLOTS:
  void LoginButtonClicked();
  void AutoLoginButtonClicked();

private:
  void Initialize();
  void TranslateLanguage();
  void setAutoLogin(bool status);
  void autoLogig();
private:
  Ui::WidgetLogin* ui;

  WebSocketClient* m_webSocketClient;
  QEventLoop* event_loop_;
  int m_agvId;
};

