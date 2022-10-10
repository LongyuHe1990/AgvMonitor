#include "widgetmessagebox.h"
#include "ui_widgetmessagebox.h"

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

bool WidgetMessageBox::MessageHint(const QString &title, const QString &content)
{
    ResizeWidget();
    //
    ui->pushButton_2->setHidden(false);
    ui->label->setText(title);
    ui->label_2->setText(TruntString(content));

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
    ui->pushButton_2->setText(tr("确定"));
    ui->pushButton_3->setText(tr("取消"));
}

void WidgetMessageBox::Initialize()
{
    QFont font = ui->label->font();
    font.setPixelSize(12);
    ui->label->setFont(font);
    ui->label_2->setFont(font);

    font = ui->pushButton->font();
    font.setPixelSize(18);
    ui->label_2->setFont(font);
    ui->pushButton_2->setFont(font);
    ui->pushButton_3->setFont(font);

    ui->pushButton->setFixedSize(QSize(34, 34));
    ui->pushButton_2->setFixedSize(QSize(120, 32));
    ui->pushButton_3->setFixedSize(QSize(120, 32));
    ui->label_2->setFixedWidth(400);
    //设置自动换行
    ui->label_2->setWordWrap(true);
    ui->label_2->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void WidgetMessageBox::ResizeWidget()
{
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
