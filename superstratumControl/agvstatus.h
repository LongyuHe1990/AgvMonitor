#ifndef AGVSTATUS_H
#define AGVSTATUS_H

#include <QWidget>

namespace Ui {
class AgvStatus;
}

class AgvStatus : public QWidget
{
    Q_OBJECT

public:
    explicit AgvStatus(QWidget *parent = nullptr);
    ~AgvStatus();

    static AgvStatus* GetInstance();
    void   InitData(QVariantMap dataMap);
private:
    void Initialize();
    void TranslateLanguage();

    virtual void changeEvent(QEvent* e) override;
private:
    Ui::AgvStatus *ui;
};

#endif // AGVSTATUS_H
