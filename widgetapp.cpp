#include "widgetapp.h"
#include "login/widgetlogin.h"
#include <QEventLoop>
#include <QDesktopWidget>
#include <QApplication>

WidgetApp::WidgetApp(QObject* parent)
  : QObject(parent)
  , widget_main_(nullptr)
  , m_webSocketClient(nullptr)
{
    m_webSocketClient = new WebSocketClient(this);
}

WidgetApp::~WidgetApp()
{
    if(m_webSocketClient != nullptr)
    {
        m_webSocketClient->deleteLater();
        m_webSocketClient = nullptr;
    }
}

void WidgetApp::Run()
{
  widget_main_ = new WidgetMain;

  bool runCondition = false;

  #ifdef QT_NO_DEBUG
    runCondition = true;
  #else
    runCondition = false;
  #endif

  do{

    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    QRect deskRect = pDesktopWidget->geometry();

    WidgetLogin login;
#ifdef QT_NO_DEBUG
    login.setGeometry(0,0,deskRect.width(),deskRect.height());
#else
    login.setGeometry(deskRect.width() / 2 - 400,deskRect.height()/2 - 300, 800, 600);
#endif
    login.show();
    QEventLoop loop;
    login.SetEventLoop(&loop);
    loop.exec();

    QEventLoop loop_main;
    widget_main_->SetEventLoop(&loop_main);
#ifdef  QT_NO_QDEBUG
    widget_main_->setGeometry(0,0,deskRect.width(),deskRect.height());
#else
    widget_main_->setGeometry(deskRect.width() / 2 - 800,deskRect.height()/2 - 500, 1600, 1000);
#endif
    widget_main_->show();;
    loop_main.exec();

  }while(runCondition);

}
