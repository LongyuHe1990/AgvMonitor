#include "widgetapp.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char* argv[])
{
#ifdef QT_NO_QDEBUG
  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
#endif

  QApplication a(argc, argv);

#ifdef Q_OS_LINUX
  int     id   = QFontDatabase::addApplicationFont("/usr/fonts/DroidSansFallback.ttf");
  QStringList list = QFontDatabase::applicationFontFamilies (id);
  if(list.size() > 0)
  {
      QString msyh = list.at(0);
      QFont   font(msyh, 10);
      a.setFont(font);
  }
#endif


  WidgetApp app;
  app.Run();

  return 0;
}
