#include "widgetmessagebox.h"
#include "ui_widgetmessagebox.h"
#include <QDesktopWidget>

WidgetMessageBox::WidgetMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WidgetMessageBox)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    Initialize();
    TranslateLanguage();

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(CancleButtonClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(OkButtonClicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(CancleButtonClicked()));
}

WidgetMessageBox::~WidgetMessageBox()
{
    delete ui;
}

bool WidgetMessageBox::MessageHint(const QString &title, const QString &content, bool showCancle)
{
    ui->pushButton_2->setHidden(false);
    ui->label->setText(title);
    ui->label_2->setText(TruntString(content));
    ui->pushButton_3->setVisible(showCancle);

    ResizeWidget();
    const int result = exec();
    qApp->processEvents();
    return result;
}

void WidgetMessageBox::OkButtonClicked()
{
    accept();
}

void WidgetMessageBox::CancleButtonClicked()
{
    reject();
    emit QuitWindow();
}

void WidgetMessageBox::TranslateLanguage()
{
    ui->pushButton_2->setText(tr("Confirm"));
    ui->pushButton_3->setText(tr("Cancel"));
}

void WidgetMessageBox::Initialize()
{
    QFont font = ui->label->font();
    font.setPixelSize(24);
    ui->label->setFont(font);

    font = ui->pushButton->font();
    font.setPixelSize(36);
    ui->label_2->setFont(font);
    ui->pushButton_2->setFont(font);
    ui->pushButton_3->setFont(font);

    ui->pushButton->setFixedSize(QSize(68, 68));
    ui->pushButton_2->setFixedSize(QSize(240, 64));
    ui->pushButton_3->setFixedSize(QSize(240, 64));
    ui->label_2->setFixedWidth(800);
    //设置自动换行
    ui->label_2->setWordWrap(true);
    ui->label_2->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void WidgetMessageBox::ResizeWidget()
{
    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    QRect           deskRect       = pDesktopWidget->geometry();
//    setGeometry(deskRect.width() / 2 - 250, deskRect.height() / 2 - 200, 1920, 1200);
    setGeometry(0, 0, 1920, 1200);
}

QString WidgetMessageBox::TruntString(const QString &str)
{
    QString content_str = str;
    if(1000 < content_str.size())
    {
      content_str.remove(1000, content_str.size() - 1000);
      content_str.append("...");
    }
    return content_str;
}

void WidgetMessageBox::changeEvent(QEvent *e)
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
