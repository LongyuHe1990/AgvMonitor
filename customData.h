#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#define ADDRESS_PORT ":1008"

enum class DataModuleType
{
    Config,
    Agv,
    Task,
    Map,
    Station,
    Alarm,
    Simula,
    User,
    //这个必须放到最后一个
    HeartBeat
};

enum class ConfigType
{
    Plugin ,
    AgvTemplate,
    AgvType ,
    Agv ,
    Area ,
    Map,
    Sql,
    MaterialType ,
    Craft ,
    Device,
    Connection,
    StationType,
    StationInteraction,
    Alarm,
    System,
    Analysis,
    PreTask
};

enum class LoginOperationType
{
    LOGIN_CHECK
};

enum class ConfigOperatedType {
    OPERATED_TYPE_ADD,
    OPERATED_TYPE_UPDATE,
    OPERATED_TYPE_REMOVE,
    OPERATED_TYPE_INIT
};

enum class AgvOperationType {
    AGV_STATUS_UPDATED,
    AGV_SWITCH_LASER,
    AGV_REORIENTATE,
    AGV_PAUSE,
    AGV_CONTINUE,
    AGV_SUSPEND,
    AGV_LOGOUT,
    AGV_RELOC,
    AGV_SLAMMAP_CONSTRUCT_START,
    AGV_SLAMMAP_CONSTRUCT_STOP,
    CalibrateReflector,
    AGV_DEVICE_UPDATED
};

enum class StationOperationType {
    STATION_INITED,
    STATION_Created,
    STATION_Updated,
    STATION_ERASED,
    CRAFTSTATION_INITED,
    Update_Station_Material = 11,
};

enum class TaskOperationType {
    Task_Inited = 0,
    Task_Created = 1,
    Task_Updated = 2,
    Task_Erased = 3,


    Create_Task = 11,
    Cancel_Task = 12,
    Cancel_All_Task = 13,
    Delete_Task = 14
};

enum class AlarmOperationType {
    Alarm_Updated = 2,
};

enum RequestIdType{
    REQUEST_MAP_XML,
    REQUEST_MAP_SLAM,
    REQUEST_RELOCATION,
    REQUEST_CREATE_TASK,
    REQUEST_CANCEL_TASK,
    REQUEST_DELETE_TASK
};
#endif // CUSTOMDATA_H
