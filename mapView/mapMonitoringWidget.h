#ifndef MAPMONITORINGWIDGET_H
#define MAPMONITORINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class MapMonitoringWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapMonitoringWidget(QWidget *parent = nullptr);
    ~MapMonitoringWidget();

    static MapMonitoringWidget* getInstance();

    void initWidget();
    void updateFloorComboBox();
    void setAreaId(int areaId);
    void setMapId(int mapId);
    void setMapVersions();
    void updataAgvItemPos(QVariantMap data);

private:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    int m_areaId;
    int m_mapId;
    int m_floorId;
    QPushButton* m_relocButton;
    QLabel* m_mapVersionsLabel;
    QString m_mapVersions;
    QList<QString> m_floorIdList;
};

#endif // MAPMONITORINGWIDGET_H
