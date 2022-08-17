#include "widgetcode.h"
#include "ui_widgetcode.h"

WidgetCode::WidgetCode(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::WidgetCode)
{
  ui->setupUi(this);

  Initialize();
  ui->lineEdit->setMaxLength(6);
  connect(ui->lineEdit, SIGNAL(textChanged(QString)), SLOT(TextChanged(QString)));
}

WidgetCode::~WidgetCode()
{
  delete ui;
}

QString WidgetCode::GetText()
{
  return ui->lineEdit->text();
}

void WidgetCode::TextChanged(QString text)
{
  line_edit_1->clear();
  line_edit_2->clear();
  line_edit_3->clear();
  line_edit_4->clear();
  line_edit_5->clear();
  line_edit_6->clear();

  line_edit_1->setText(text.mid(0, 1));
  line_edit_2->setText(text.mid(1, 2));
  line_edit_3->setText(text.mid(2, 3));
  line_edit_4->setText(text.mid(3, 4));
  line_edit_5->setText(text.mid(4, 5));
  line_edit_6->setText(text.mid(5, 6));
}

void WidgetCode::Initialize()
{
  setFixedSize(440, 50);

  QString style = "QLineEdit{background-color:rgb(255, 226, 96);color:rgb(5, 5, 5);border:none;border-radius:4px;}";
  line_edit_1 = new QLineEdit(this);
  line_edit_1->setStyleSheet(style);
  line_edit_1->setGeometry(0, 0, 50, 50);
  line_edit_2 = new QLineEdit(this);
  line_edit_2->setStyleSheet(style);
  line_edit_2->setGeometry(78, 0, 50, 50);
  line_edit_3 = new QLineEdit(this);
  line_edit_3->setStyleSheet(style);
  line_edit_3->setGeometry(156, 0, 50, 50);
  line_edit_4 = new QLineEdit(this);
  line_edit_4->setStyleSheet(style);
  line_edit_4->setGeometry(234, 0, 50, 50);
  line_edit_5 = new QLineEdit(this);
  line_edit_5->setStyleSheet(style);
  line_edit_5->setGeometry(312, 0, 50, 50);
  line_edit_6 = new QLineEdit(this);
  line_edit_6->setStyleSheet(style);
  line_edit_6->setGeometry(390, 0, 50, 50);

  line_edit_1->setEnabled(false);
  line_edit_2->setEnabled(false);
  line_edit_3->setEnabled(false);
  line_edit_4->setEnabled(false);
  line_edit_5->setEnabled(false);
  line_edit_6->setEnabled(false);

  line_edit_1->setMaxLength(1);
  line_edit_2->setMaxLength(1);
  line_edit_3->setMaxLength(1);
  line_edit_4->setMaxLength(1);
  line_edit_5->setMaxLength(1);
  line_edit_6->setMaxLength(1);

  line_edit_1->setAlignment(Qt::AlignCenter);
  line_edit_2->setAlignment(Qt::AlignCenter);
  line_edit_3->setAlignment(Qt::AlignCenter);
  line_edit_4->setAlignment(Qt::AlignCenter);
  line_edit_5->setAlignment(Qt::AlignCenter);
  line_edit_6->setAlignment(Qt::AlignCenter);

  QFont font = line_edit_1->font();
  font.setPixelSize(24);
  line_edit_1->setFont(font);
  line_edit_2->setFont(font);
  line_edit_3->setFont(font);
  line_edit_4->setFont(font);
  line_edit_5->setFont(font);
  line_edit_6->setFont(font);
}
