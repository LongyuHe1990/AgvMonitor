#ifndef MAPINFO_H
#define MAPINFO_H
#include <QString>
#include <QPointF>

typedef struct NodeInfo{
    QString id;
    QPointF point;
}NODEINFO;

typedef struct LineInfo{
    QString id;
    NODEINFO startNode;
    NODEINFO endNode;
}LINEINFO;

typedef struct StationInfo{
    QString id;
    QString name;
    QPointF point;
    NODEINFO node;
    int stationTypeId;
    int row;
    int column;
}STATIONINFO;

enum GetMapType{
    MAP_TYPE_XML,
    MAP_TYPE_SLAM
};

enum class MapOperationType {
    MAP_GETMAP,
    MAP_UPLOAD
};

enum class MapFileType
{
    BMAP,
    PNG,
    XML
};

#endif // MAPINFO_H
