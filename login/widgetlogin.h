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

namespace Ui {
class WidgetLogin;
}

class WidgetLogin : public QWidget
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

  QEventLoop* event_loop_;
  int m_agvId;
};

