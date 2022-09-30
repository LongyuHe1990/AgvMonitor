#ifndef MAPMONITORINGVIEW_H
#define MAPMONITORINGVIEW_H

#include <QObject>
#include <QGraphicsView>
#include "mapMonitoringScene.h"
#include "mapInfo.h"
#include <QDomComment>
#include <QtConcurrent>
#include <QWebSocket>
#include <QPushButton>
#include <QLabel>

class MapMonitoringView : public QGraphicsView
{
    Q_OBJECT
public:
    MapMonitoringView(QWidget *parent = nullptr);
    ~MapMonitoringView();

    static MapMonitoringView* getInstance();

    void setMapId(int mapId);
    void setFloorId(int floorId);
    void setAreaId(int areaId);

    void initMap(QVariantMap data);
    void requestMapDataInfo();

    void updataAgvItemPos(QVariantMap data);

    void setRelocationResult(QVariantMap moduleData);
    void updateFloorComboBox();
private:
    void zoomIn(QPoint pos);
    void zoomOut(QPoint pos);

    void readNodeXml(QDomNodeList nodeList);
    void readLineXml(QDomNodeList lineList);
    void readStationXml(QDomNodeList stationList);

    void loadMapData(QVariantMap data);
    void loadSlamDataToImagee(QVariantMap data);

    void addMapToScene();
    void addSlamToScene();

    void centeredShowMapToView();

    void initWidget();

    void sendRelocationDataToServer();
    void setMapVersions();
    void showFloorInfo(int floor);
private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void paintEvent(QPaintEvent *event);
public slots:
    void onClickRelocButton();
private:
    int m_mapId;
    int m_floorId;
    int m_areaId;
    QPointF m_pressPoint;
    bool m_leftMousePress;
    qreal m_scale;
    MapMonitoringScene* m_scene;

    QMap<QString, NODEINFO> m_nodes;
    QMap<QString, LINEINFO> m_lines;
    QMap<QString, STATIONINFO> m_stations;
    QImage m_slamImage;
    QRect m_slamRect;

    QFutureWatcher<void>* m_loadMapDataWatcher;
    QMutex m_loadMapDataMutex;

    QFutureWatcher<void>* m_loadSlamWatcher;
    QMutex m_loadSlamMutex;
    bool m_isRelocation;
    QPoint m_lastPoint;
    QLineF m_line;
    double m_angle;
    QPixmap m_img;
    QPushButton* m_relocButton;
    QLabel* m_mapVersionsLabel;
    QString m_mapVersions;
    QList<QString> m_floorIdList;
};

#endif // MAPMONITORINGVIEW_H
