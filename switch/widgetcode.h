#pragma once

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class WidgetCode;
}

class WidgetCode : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetCode(QWidget* parent = nullptr);
  ~WidgetCode();

  QString GetText();

private Q_SLOTS:
  void TextChanged(QString text);

private:
  void Initialize();

private:
  Ui::WidgetCode* ui;

  QLineEdit* line_edit_1;
  QLineEdit* line_edit_2;
  QLineEdit* line_edit_3;
  QLineEdit* line_edit_4;
  QLineEdit* line_edit_5;
  QLineEdit* line_edit_6;
};
