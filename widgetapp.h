/********************************************************
* @author Xiaolan Luo(xiaolan.luo@bozhon.com)
* @brief
* @date 2022.7.8
*
* @copyright CopyRight (c) 2021 BOHHOM, Inc.
********************************************************/
#pragma once
#include "widgetmain.h"
#include <QObject>
#include "webSocket/webSocketClient.h"

class WidgetApp : public QObject
{
public:
  explicit WidgetApp(QObject* parent = nullptr);
  virtual ~WidgetApp();

  void Run();
private:
  WidgetMain* widget_main_;

  WebSocketClient* m_webSocketClient;
};

