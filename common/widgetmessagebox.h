#pragma once

#include <QWidget>
#include <QDialog>

namespace Ui {
class WidgetMessageBox;
}

class WidgetMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit WidgetMessageBox(QWidget *parent = nullptr);
    virtual ~WidgetMessageBox();

    /**
     * @brief MessageHint 消息提示框（提示加选择:有确定、取消键）
     * @param content 提示的消息内容content
     * @return 返回值bool - 关闭提示框
     */
    bool MessageHint(const QString &title, const QString &content, bool showCancle = true);

Q_SIGNALS:
  /**
   * @brief QuitWindow 退出窗口
   */
  void QuitWindow();

private Q_SLOTS:
  /**
   * @brief OkButtonClicked 确定键被点击
   */
  void OkButtonClicked();
  /**
   * @brief CancleButtonClicked 取消键被点击
   */
  void CancleButtonClicked();

private:
  void TranslateLanguage();
  void Initialize();
  void ResizeWidget();
  /**
   * @brief TruntString 消息信息大小的处理
   * @param str 消息信息str
   * @return 返回类型QStirng - 消息信息大小1000为界的处理
   */
  QString TruntString(const QString &str);

protected:
  /**
   * @brief QMessage::changeEvent 处理翻译事件
   * @param e 事件e
   */
  virtual void changeEvent(QEvent* e) override;

private:
    Ui::WidgetMessageBox *ui;
};
