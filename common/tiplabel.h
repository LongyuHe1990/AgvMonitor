#pragma once

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QApplication>
#include <QDesktopWidget>

class TipLabel : public QWidget
{
    Q_OBJECT
public:
    explicit TipLabel(QWidget* parent = nullptr);
    explicit TipLabel(const QString& text, QWidget* parent = nullptr);
    virtual ~TipLabel();
    /*!
     * Set timer interval to hide.
     */
    void SetTimerInterval(int msecond);
    /*!
     * Get timer interval.
     */
    int  GetTimerInterval() const;
    /*!
     * Popup the toast widget in desktop widget.
     */
    void Popup();
    /*!
     * Popup the toast widget in desktop widget.
     */
    static void Popup(const QString &text);

  Q_SIGNALS:
    /*!
     * Animation close changed.
     */
    void AnimationCloseChanged();

  public Q_SLOTS:
    /*!
     * Set context text.
     */
    void SetText(const QString &text);

  private Q_SLOTS:
    /*!
     * Close animation when tiem out.
     */
    void CloseAnimation();

  private:
    void Initialize();

  protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent* e) override;

  private:
    QTimer  timer_;
    QLabel* text_label_;

};

