#include "widgetlogin.h"
#include "ui_widgetlogin.h"
#include "webSocket/webSocketClient.h"
#include "customData.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "common/global_config.h"

#define autoLoginPath "/usr"

static WidgetLogin* s_widgetLogin = nullptr;

WidgetLogin::WidgetLogin(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetLogin)
  , event_loop_(nullptr)
{
  ui->setupUi(this);

  s_widgetLogin = this;

  setAttribute(Qt::WA_ShowModal, true);
#ifdef Q_OS_WIN
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif
  Initialize();
  TranslateLanguage();
  autoLogig();

  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(LoginButtonClicked()));
  connect(ui->pushButton_2, &QPushButton::clicked, this, &WidgetLogin::AutoLoginButtonClicked);
}

WidgetLogin::~WidgetLogin()
{
  delete ui;
  s_widgetLogin = nullptr;
}

WidgetLogin * WidgetLogin::getInstance()
{
  return s_widgetLogin;
}

void WidgetLogin::loginSucceed()
{
  event_loop_->exit(0);
  s_widgetLogin = nullptr;
  close();
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
  WebSocketClient::getInstance()->setAgvId(id.toInt());
  UserConfigs::AgvId = id.toInt();
  QUrl url = QUrl("ws://" + ip + ADDRESS_PORT);
  WebSocketClient::getInstance()->setClickLogin(true);
  WebSocketClient::getInstance()->openWebSocket(url.toString());
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
  font.setPixelSize(24);
  ui->label_7->setFont(font);
  ui->label->setFont(font);
  ui->label_2->setFont(font);
  ui->lineEdit->setFont(font);
  ui->lineEdit_2->setFont(font);

  font.setPixelSize(32);
  ui->pushButton->setFont(font);
  ui->pushButton_2->setFont(font);

  font.setPixelSize(14);
  ui->label_4->setFont(font);
  ui->label_5->setFont(font);
  ui->label_6->setFont(font);

  font.setPixelSize(9);
  ui->label_3->setFont(font);

// ui->label->setFixedSize(QSize(108, 56));
// ui->label_2->setFixedSize(QSize(108, 56));
// ui->lineEdit->setFixedSize(QSize(576, 56));
// ui->lineEdit_2->setFixedSize(QSize(576, 56));
  ui->pushButton->setFixedSize(QSize(273, 72));
  ui->pushButton_2->setFixedSize(QSize(378, 72));
  ui->label_8->setFixedSize(QSize(89, 39));
}

void WidgetLogin::TranslateLanguage()
{
  ui->label->setText(tr("整车ID"));
  ui->label_2->setText(tr("IP地址"));
  ui->pushButton->setText(tr("访客登录"));
  ui->pushButton_2->setText(tr("下次自动登录"));
  ui->label_3->setText(tr("记住整车ID与IP地址下次重启机器后自动登录"));
  ui->label_4->setText(tr("*请输入正确的IP地址"));
  ui->label_5->setText(tr("*请输入IP地址"));
  ui->label_7->setText(tr("牵星系统调度平台"));
}

void WidgetLogin::setAutoLogin(bool status)
{
  if(status)
  {
    QFile file("/media/linaro/params/server.json");
    // bool status1 = file.setPermissions(QFileDevice::WriteUser);
    if(file.open(QIODevice::ReadWrite))
    {
      QString id = ui->lineEdit->text();
      QString ip = ui->lineEdit_2->text();

      QVariantMap content;
      content.insert("agvID", id.toInt());
      content.insert("serverPort", ADDRESS_PORT);
      content.insert("severIP", ip);

      QJsonObject   obj  = QJsonObject::fromVariantMap(content);
      QJsonDocument doc  = QJsonDocument(obj);
      QString       data = doc.toJson();

      file.write(data.toUtf8());
    }

    file.close();
  }
}

void WidgetLogin::autoLogig()
{
  QFile file("/media/linaro/params/server.json");
  if(file.open(QIODevice::ReadWrite))
  {
    QString data = file.readAll();

    QJsonDocument doc     = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject   obj     = doc.object();
    QVariantMap   content = obj.toVariantMap();

    int     agvId = content.value("agvID").toInt();
    QString port  = content.value("serverPort").toString();
    QString ip    = content.value("severIP").toString();

    ui->lineEdit->setText(QString::number(agvId));
    ui->lineEdit_2->setText(ip);
  }

  file.close();
}
