#include "widgetlogin.h"
#include "ui_widgetlogin.h"
#include "webSocket/webSocketClient.h"
#include "customData.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "common/global_config.h"
#include <QUuid>
#include "widgetmain.h"
#include <QKeyEvent>
#include <QDir>

#define autoLoginPath "/usr"

static WidgetLogin* s_widgetLogin = nullptr;

WidgetLogin::WidgetLogin(QWidget* parent)
  : QWidget(parent)
  , logger::Logger("Login")
  , ui(new Ui::WidgetLogin)
  , m_webSocketClient(nullptr)
  , event_loop_(nullptr)
  ,m_timer(nullptr)
{
  ui->setupUi(this);

  s_widgetLogin = this;
  m_webSocketClient = new WebSocketClient(this);
  //setAttribute(Qt::WA_ShowModal, true);
#ifdef Q_OS_WIN
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif
  Initialize();
  TranslateLanguage();
  autoLogig();

  QUuid uuid = QUuid::createUuid();
  UserConfigs::Uuid = uuid.toString().remove("{").remove("}");

  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LoginButtonClicked()));
  connect(ui->pushButton_2, &QPushButton::clicked, this, &WidgetLogin::AutoLoginButtonClicked);
}

WidgetLogin::~WidgetLogin()
{
  delete ui;
  s_widgetLogin = nullptr;
  event_loop_ = nullptr;
  if(m_timer != nullptr)
  {
      m_timer->stop();
      m_timer->deleteLater();
      m_timer = nullptr;
  }
}

WidgetLogin * WidgetLogin::getInstance()
{
  return s_widgetLogin;
}

void WidgetLogin::loginSucceed()
{
  close();
  s_widgetLogin = nullptr;

  if(m_timer != nullptr)
  {
      m_timer->stop();
      m_timer->deleteLater();
      m_timer = nullptr;
  }

  event_loop_->exit(0);
  event_loop_ = nullptr;
}

void WidgetLogin::setTipsServiceLinkStatus(bool status)
{
  if(status)
  {
    ui->label_6->setText("服务链接成功");
  }
  else
  {
    ui->label_6->setText("服务链接失败");
  }
}

void WidgetLogin::SetEventLoop(QEventLoop* loop)
{
  event_loop_ = loop;
}

void WidgetLogin::LoginButtonClicked()
{
  if(ui->lineEdit->text().isEmpty())
  {
    qDebug("id is empty");
    return;
  }

  if(ui->lineEdit_2->text().isEmpty())
  {
    ui->label_5->setHidden(false);
    return;
  }

  QString id = ui->lineEdit->text();
  QString ip = ui->lineEdit_2->text();
  UserConfigs::AgvId = id.toInt();
  QUrl url = QUrl("ws://" + ip + ADDRESS_PORT);
  WebSocketClient::getInstance()->setClickLogin(true);
  WebSocketClient::getInstance()->openWebSocket(url.toString());
  //LOGGER_INFO(this)<<("clickedLoginButton: id-" + id.toStdString() + "  ip-" + ip.toStdString());
}

void WidgetLogin::AutoLoginButtonClicked()
{
  setAutoLogin(true);

  LoginButtonClicked();
}

void WidgetLogin::Initialize()
{
  ui->lineEdit_2->setInputMask("000.000.000.000");
  ui->label_4->setHidden(true);
  ui->label_5->setHidden(true);

  QFont font = ui->label_7->font();
  font.setPixelSize(48);
  ui->label_7->setFont(font);
  ui->label->setFont(font);
  ui->label_2->setFont(font);
  ui->lineEdit->setFont(font);
  ui->lineEdit_2->setFont(font);

  font.setPixelSize(64);
  ui->pushButton->setFont(font);
  ui->pushButton_2->setFont(font);

  font.setPixelSize(28);
  ui->label_4->setFont(font);
  ui->label_5->setFont(font);
  ui->label_6->setFont(font);

  font.setPixelSize(36);
  ui->label_3->setFont(font);

  ui->pushButton->setFixedSize(QSize(547, 144));
  ui->pushButton_2->setFixedSize(QSize(756, 144));
  ui->label_8->setFixedSize(QSize(178, 78));
}

void WidgetLogin::TranslateLanguage()
{
  ui->label->setText(tr("Vehicle ID"));
  ui->label_2->setText(tr("IP address"));
  ui->pushButton->setText(tr("Guest Login"));
  ui->pushButton_2->setText(tr("Automatic login next time"));
  ui->label_3->setText(tr("Remember the vehicle ID and IP address, and log in automatically after the next restart"));
  ui->label_4->setText(tr("*Please enter the correct IP address"));
  ui->label_5->setText(tr("*Please enter ip address"));
  ui->label_7->setText(tr("Dispatching platform of traction satellite system"));
}

void WidgetLogin::setAutoLogin(bool status)
{
    if(status)
    {
        qDebug()<<"autoLogin:1";
        QFile file("/home/linaro/Desktop/AgvMonitor/server.json");
        // bool status1 = file.setPermissions(QFileDevice::WriteUser);
        if(file.open(QIODevice::WriteOnly))
        {
            qDebug()<<"autoLogin:2";
          QString id = ui->lineEdit->text();
          QString ip = ui->lineEdit_2->text();

          QVariantMap content;
          content.insert("agvID", id.toInt());
          content.insert("serverPort", ADDRESS_PORT);
          content.insert("severIP", ip);

          QJsonObject   obj  = QJsonObject::fromVariantMap(content);
          QJsonDocument doc  = QJsonDocument(obj);
          QString       data = doc.toJson();

          int state = file.write(data.toStdString().c_str());
          qDebug()<<"autoLogin:3-" << QString::number(state);
        }

        file.close();
    }
}

void WidgetLogin::autoLogig()
{
  QFile file("/home/linaro/Desktop/AgvMonitor/server.json");
  if(file.open(QIODevice::ReadOnly))
  {
      qDebug()<<"readServer:1";
    QString data = file.readAll();

    QJsonDocument doc     = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject   obj     = doc.object();
    QVariantMap   content = obj.toVariantMap();
    if(content.isEmpty())
    {
        return;
    }

    int     agvId = content.value("agvID").toInt();
    QString port  = content.value("serverPort").toString();
    QString ip    = content.value("severIP").toString();

    qDebug()<<"readServer:2-"<<QString::number(agvId);
    qDebug()<<"readServer:3-"<<ip;
    ui->lineEdit->setText(QString::number(agvId));
    ui->lineEdit_2->setText(ip);

    m_timer = new QTimer(this);
    //启动定时器
    m_timer->start(3000);
    connect(m_timer,&QTimer::timeout,[=](){
        qDebug()<<"autoLogig:Timer";
        LoginButtonClicked();
    });

    LoginButtonClicked();
  }

  file.close();
}

void WidgetLogin::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Escape)
  {
    event_loop_->exit(1);
    QApplication::exit(1);
    QApplication::closeAllWindows();
    qDebug()<<"closeApp";
  }
}

void WidgetLogin::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch(e->type())
    {
    case QEvent::LanguageChange:
      TranslateLanguage();
      break;
    default:
      break;
    }
}
