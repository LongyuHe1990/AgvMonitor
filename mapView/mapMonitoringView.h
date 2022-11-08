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

    void initMap(QVariantMap data);
    void requestMapDataInfo();

    void updataAgvItemPos(QVariantMap data);

    void setRelocationResult(QVariantMap moduleData);

    void showFloorInfo(int floor);

    void updataAgvConfidence(QVariantMap data);

private:
    void zoomIn(QPoint pos);
    void zoomOut(QPoint pos);

    void readNodeXml(QDomNodeList nodeList);
    void readLineXml(QDomNodeList lineList);
    void readStationXml(QDomNodeList stationList);
    void readLiftXml(QDomNodeList liftList);

    void loadMapData(QVariantMap data);
    void loadSlamDataToImagee(QVariantMap data);

    void addMapToScene();
    void addSlamToScene();

    void centeredShowMapToView();

    void initWidget();

    void sendRelocationDataToServer();

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
    QPointF m_pressPoint;
    bool m_leftMousePress;
    qreal m_scale;
    MapMonitoringScene* m_scene;

    QMap<QString, NODEINFO> m_nodes;
    QMap<QString, LINEINFO> m_lines;
    QMap<QString, STATIONINFO> m_stations;
    QMap<QString, LIFTINFO> m_lifts;
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
    QDialog *m_titleRelocDlg;
    QVariantMap m_agvInfor;
};

#endif // MAPMONITORINGVIEW_H
