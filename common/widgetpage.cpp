#include "widgetpage.h"
#include "ui_widgetpage.h"
#include "tiplabel.h"
#include <QValidator>

WidgetPage::WidgetPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPage)
  , total_page_(0)
  , current_page_(0)
  , page_size_(0)
{
    ui->setupUi(this);

    Initialize();
    TranslateLanguage();

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(PreviousPageClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(NextPageClicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(GoButtonClicked()));
}

WidgetPage::~WidgetPage()
{
    delete ui;
}

void WidgetPage::SetTotalPage(int page)
{
    total_page_ = page;
    current_page_ = 1;
}

int WidgetPage::CurrentPage()
{
    return current_page_;
}

void WidgetPage::SetPageSize(int size)
{
    page_size_ = size;
}

int WidgetPage::GetPageSize()
{
    return page_size_;
}

void WidgetPage::PreviousPageClicked()
{
    if(current_page_ < 1)
    {
       TipLabel::Popup(tr("It is already the first page"));
       return;
    }

    current_page_ -= 1;
}

void WidgetPage::NextPageClicked()
{
    if(current_page_ >= total_page_)
    {
        TipLabel::Popup(tr("It is already the last page"));
        return;
    }

    current_page_ += 1;
}

void WidgetPage::GoButtonClicked()
{
    current_page_ = ui->lineEdit->text().toInt();
}

void WidgetPage::Initialize()
{
    QFont font = ui->pushButton->font();
    font.setPixelSize(20);

    ui->pushButton->setFont(font);
    ui->pushButton_2->setFont(font);
    ui->pushButton_3->setFont(font);

    font.setPixelSize(16);
    ui->lineEdit->setFont(font);

    ui->lineEdit->setValidator(new QIntValidator(this));
    ui->widget_2->setFixedSize(QSize(440, 32));
}

void WidgetPage::TranslateLanguage()
{
   ui->pushButton->setText(tr("previous page"));
   ui->pushButton_2->setText(tr("next page"));
   ui->pushButton_3->setText(tr("Go"));
   ui->lineEdit->setPlaceholderText(tr("select or enter a page number"));
}

void WidgetPage::changeEvent(QEvent *e)
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
