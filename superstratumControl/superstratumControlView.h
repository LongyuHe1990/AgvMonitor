#ifndef SUPERSTRATUMCONTROLVIEW_H
#define SUPERSTRATUMCONTROLVIEW_H

#include <QObject>
#include <QWidget>

class SuperstratumControlView : public QWidget
{
    Q_OBJECT
public:
    explicit SuperstratumControlView(QWidget *parent = nullptr);

    static SuperstratumControlView* getInstance();
    void initWidget();
    void initWidgetData();
    void updateCurrentInteractInfo(QVariantMap data);
signals:

public slots:

private:
    void updateProcessSelectBox(QVariantMap stationTypeData);
    void updateActionTypeBox(int stationTypeId);
    void uploadCommParm();
    QVariantList getCommParmList();
    int getStationIdFromBox();
    int getActionIdFromBox();
    int getProcessStepFromBox();
    int getProcessIdByStationId(int stationId);
};

#endif // SUPERSTRATUMCONTROLVIEW_H
