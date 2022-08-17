#ifndef MAPMONITORINGVIEW_H
#define MAPMONITORINGVIEW_H

#include <QObject>
#include <QGraphicsView>
#include "mapMonitoringScene.h"
#include "mapInfo.h"
#include <QDomComment>
#include <QtConcurrent>
#include <QWebSocket>

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
    void requestMapDataInfo(QWebSocket* client);

    void updataAgvItemPos(QVariantMap data);

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
private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

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
};

#endif // MAPMONITORINGVIEW_H
