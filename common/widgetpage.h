#pragma once

#include <QWidget>

namespace Ui {
class WidgetPage;
}

class WidgetPage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPage(QWidget *parent = nullptr);
    ~WidgetPage();

    void SetTotalPage(int page);
    int CurrentPage();
    void SetPageSize(int size);
    int GetPageSize();

private Q_SLOTS:
    void PreviousPageClicked();
    void NextPageClicked();
    void GoButtonClicked();

private:
    void Initialize();
    void TranslateLanguage();
    virtual void changeEvent(QEvent* e) override;
private:
    Ui::WidgetPage *ui;

    int total_page_;
    int current_page_;
    int page_size_;
};

