#include "widgetapp.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDesktopWidget>
#include <QEventLoop>
#include "widgetmain.h"
#include "login/widgetlogin.h"
#include <QDebug>

int main(int argc, char* argv[])
{
  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

  QApplication a(argc, argv);

  QString qss;
  QFile qssFile(":/qss/theme.css");
  qssFile.open(QFile::ReadOnly);
  if(qssFile.isOpen())
  {
      qss = QLatin1String(qssFile.readAll());
      a.setStyleSheet(qss);
      qssFile.close();
  }


#ifdef Q_OS_LINUX
  int         id   = QFontDatabase::addApplicationFont("/usr/fonts/DroidSansFallback.ttf");
  QStringList list = QFontDatabase::applicationFontFamilies (id);
  if(list.size() > 0)
  {
    QString msyh = list.at(0);
    QFont   font(msyh, 10);
    a.setFont(font);
  }
#endif

  QDesktopWidget* pDesktopWidget = QApplication::desktop();
  QRect           deskRect       = pDesktopWidget->geometry();
  WidgetMain      widget_main;
  WidgetLogin     login;
#ifdef QT_NO_DEBUG
  login.setGeometry(0, 0, deskRect.width(), deskRect.height());
#else
  login.setGeometry(deskRect.width() / 2 - 400, deskRect.height()/2 - 300, 800, 600);
#endif
  login.show();
  QEventLoop loop;
  login.SetEventLoop(&loop);
  int status = loop.exec();

  //if(status == 0)
  {
    #ifdef QT_NO_DEBUG
      widget_main.setGeometry(0, 0, deskRect.width(), deskRect.height());
    #else
      widget_main.setGeometry(deskRect.width() / 2 - 900, deskRect.height()/2 - 500, 1800, 1000);
    #endif
      widget_main.show();
//      widget_main.showFullScreen();
      QEventLoop loop_main;
      widget_main.SetEventLoop(&loop_main);
      loop_main.exec();
  }

  return a.exec();
}
