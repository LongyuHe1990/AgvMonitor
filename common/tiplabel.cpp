#include "tiplabel.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>

#define BZROBOT_UNUSED(x)   (void)x
TipLabel::TipLabel(QWidget *parent)
    : QWidget (parent)
{
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    Initialize();
    TranslateLanguage();

    connect(&timer_, SIGNAL(timeout()), SLOT(CloseAnimation()));
    timer_.setInterval(800);
    timer_.start();
}

TipLabel::TipLabel(const QString &text, QWidget *parent)
    : QWidget (parent)
{
    SetText(text);
}

TipLabel::~TipLabel()
{
    timer_.stop();
}

void TipLabel::SetTimerInterval(int msecond)
{
    timer_.stop();
    timer_.setInterval(msecond);
    timer_.start();
}

int TipLabel::GetTimerInterval() const
{
    return timer_.interval();
}

void TipLabel::Popup()
{
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->screenGeometry(0).width() - width()) / 2, (desktop->screenGeometry(0).height() - height()) / 2);
    show();
}

void TipLabel::Popup(const QString &text)
{
    TipLabel* widget = new TipLabel;
    widget->SetText(text);
    widget->Popup();
}

void TipLabel::SetText(const QString &text)
{
    QFontMetrics fontMetrics(text_label_->font());
    const int    fontSize = fontMetrics.width(text);
    text_label_->setWordWrap(fontSize > (800) ? true : false);
    text_label_->setText(text);
}

void TipLabel::CloseAnimation()
{
    timer_.stop();
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity", this);
    animation->setDuration(800);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();

    connect(animation, SIGNAL(finished()), SLOT(close()));
    connect(animation, SIGNAL(finished()), SIGNAL(AnimationCloseChanged()));
}

void TipLabel::Initialize()
{
    setFixedSize(836, 220);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    tip_label_ = new QLabel(this);
    tip_label_->setAlignment(Qt::AlignCenter);
    tip_label_->setStyleSheet("color: #FFFFFF");

    text_label_ = new QLabel(this);
    text_label_->setStyleSheet("color: #FFCC00");
    text_label_->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(tip_label_);
    layout->addStretch();
    layout->addWidget(text_label_);
    layout->addStretch();

    QFont font = text_label_->font();
    font.setPixelSize(32);
    text_label_->setFont(font);
    font.setPixelSize(24);
    tip_label_->setFont(font);

}

void TipLabel::TranslateLanguage()
{
    tip_label_->setText(tr("Tips"));
}

void TipLabel::paintEvent(QPaintEvent *e)
{
    BZROBOT_UNUSED(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(rect(), QPixmap(":/image/home_all/tip_label.png"));
}

void TipLabel::changeEvent(QEvent *e)
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
