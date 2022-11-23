#include "mapMonitoringWidget.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QListView>
#include "mapMonitoringView.h"
#include "moduleData/configModule.h"

static MapMonitoringWidget* s_mapMonitoringWidget = nullptr;

MapMonitoringWidget::MapMonitoringWidget(QWidget *parent) : QWidget(parent)
  ,m_areaId(-1)
  ,m_mapId(-1)
  ,m_floorId(-1)
  ,m_isRelocation(false)
{
    s_mapMonitoringWidget = this;

    initWidget();
}

MapMonitoringWidget::~MapMonitoringWidget()
{

}

MapMonitoringWidget *MapMonitoringWidget::getInstance()
{
    return s_mapMonitoringWidget;
}

void MapMonitoringWidget::initWidget()
{
    MapMonitoringView* mapView = new MapMonitoringView(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QHBoxLayout* titleLayout = new QHBoxLayout(this);
    titleLayout->setContentsMargins(20,0,40,0);
    titleLayout->setSpacing(10);
    titleLayout->setGeometry(QRect(0,0,geometry().width(),40));

    QFont font("微软雅黑", 18);
    QLabel* imageArea = new QLabel(this);
    imageArea->setMinimumSize(40,40);
    imageArea->setAlignment(Qt::AlignCenter);
    QPixmap areaImage(30,30);
    areaImage.load(":/image/area.png");
    areaImage = areaImage.scaled(30,30);
    imageArea->setPixmap(areaImage);
    titleLayout->addWidget(imageArea);
    QLabel *labelArea = new QLabel(this);
    //labelArea->setFrameShape(QFrame::Box);
    labelArea->setMinimumSize(100,40);
    labelArea->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelArea->setStyleSheet("color:white;");
    labelArea->setFont(font);
    QString areaName = "区域" + QString::number(m_areaId);
    labelArea->setText(areaName);
    labelArea->setObjectName("labelArea");
    titleLayout->addWidget(labelArea);

    titleLayout->addSpacing(20);

    QLabel* imageMap = new QLabel(this);
    imageMap->setAlignment(Qt::AlignCenter);
    //imageMap->setFrameShape(QFrame::Box);
    imageMap->setMinimumSize(30,40);
    QPixmap mapImage(30,30);
    mapImage.load(":/image/map.png");
    mapImage = mapImage.scaled(25,25);
    imageMap->setPixmap(mapImage);
    titleLayout->addWidget(imageMap);
    QLabel *labelMap = new QLabel(this);
    //labelMap->setFrameShape(QFrame::Box);
    labelMap->setMinimumSize(100,40);
    labelMap->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QString mapName = "地图" + QString::number(m_mapId);
    labelMap->setText(mapName);
    labelMap->setObjectName("labelMap");
    labelMap->setFont(font);
    labelMap->setStyleSheet("color:white");
    titleLayout->addWidget(labelMap);

    titleLayout->addSpacing(20);

    QHBoxLayout* floorBoxLayout = new QHBoxLayout(this);
    floorBoxLayout->setContentsMargins(0,0,0,0);
    floorBoxLayout->setSpacing(0);
    QLabel* floorLabel = new QLabel(this);
    floorLabel->setMaximumSize(30,40);
    QPixmap floorImage(30,30);
    floorImage.load(":/image/floor.png");
    floorImage = floorImage.scaled(30,30);
    floorLabel->setPixmap(floorImage);
    QComboBox* pComboBox = new QComboBox(this);
    pComboBox->setStyleSheet("QComboBox {combobox-popup: 0;}");
    connect(pComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int index){
        if(index >= 0)
        {
            int floorId = m_floorIdList.at(index).toInt();
            if(m_floorId != floorId)
            {
                m_floorId = floorId;
                mapView->showFloorInfo(floorId);
            }
        }
    });
    pComboBox->setMinimumSize(100, 40);
    pComboBox->setObjectName("mapView_floorComboBox");
    QListView* listView = new QListView();
    pComboBox->setView(listView);
    floorBoxLayout->addWidget(floorLabel);
    floorBoxLayout->addWidget(pComboBox);
    titleLayout->addLayout(floorBoxLayout);

    titleLayout->addStretch();

    QPushButton* relocatButton = new QPushButton(this);
    relocatButton->setStyleSheet("QPushButton{"

                                 "border: 0px solid black;"//边框宽度和颜色

                                 "border-radius: 5px;"//边框圆角

                                 "background-color:#e4bc3a;  "//背景颜色

                                 "color:black;                 "//字体颜色
                                 "}");
    QPixmap pixmap = QPixmap(":/image/relocation.png");
    relocatButton->setIcon(pixmap);
    relocatButton->setIconSize(QSize(35, 35));
    relocatButton->setMinimumSize(80, 40);
    connect(relocatButton, &QPushButton::clicked, [=](){
        if(!m_isRelocation)
        {
            m_isRelocation = true;
            QIcon icon(":/image/showStation.png");
            relocatButton->setIcon(icon);
        }
        else
        {
            m_isRelocation=false;
            QIcon icon(":/image/relocation.png");
            relocatButton->setIcon(icon);
        }

        MapMonitoringView::getInstance()->isRelocation(m_isRelocation);
    });
    titleLayout->addWidget(relocatButton);

    mainLayout->addLayout(titleLayout);

    mainLayout->addWidget(mapView);

    setLayout(mainLayout);
}

void MapMonitoringWidget::setAreaId(int areaId)
{
    m_areaId = areaId;
    QLabel* areaLabel = findChild<QLabel*>("labelArea");
    if(areaLabel != nullptr)
    {
        QString areaName = QString::number(areaId) + tr("号区域");
        areaLabel->setText(areaName);
    }
}

void MapMonitoringWidget::updateFloorComboBox()
{
    QComboBox* pComboBox = this->findChild<QComboBox*>("mapView_floorComboBox");
    pComboBox->clear();
    m_floorIdList.clear();
    QVariantMap mapData = ConfigModule::getInstance()->getConfig(ConfigType::Map, m_mapId);
    QVariantMap floorDatas = mapData.value("floorParams").toMap();
    m_floorIdList = floorDatas.keys();
    for(int i = 0; i < m_floorIdList.size(); i++)
    {
        int floorId = m_floorIdList.at(i).toInt();
        QString info = QString("  %1F (%2楼)").arg(floorId).arg(floorId);
        pComboBox->addItem(info);
    }
}

void MapMonitoringWidget::setMapId(int mapId)
{
    m_mapId = mapId;
    QLabel* mapLabel = findChild<QLabel*>("labelMap");
    if(mapLabel != nullptr)
    {
        QString mapName = QString::number(mapId) + "号地图";
        mapLabel->setText(mapName);
    }

    MapMonitoringView::getInstance()->setMapId(mapId);
}

void MapMonitoringWidget::updataAgvItemPos(QVariantMap data)
{
    int operatorType = data.value("OperationType").toInt();
    if(operatorType == int(AgvOperationType::AGV_STATUS_UPDATED))
    {
        QVariantMap content = data.value("Content").toMap();
        QString currentMapUuid = content.value("currentMapUuid").toString();
    }

    if(MapMonitoringView::getInstance() != nullptr)
    {
        MapMonitoringView::getInstance()->updataAgvItemPos(data);
    }
}

void MapMonitoringWidget::paintEvent(QPaintEvent *event)
{
    //MapMonitoringWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.save();
    QRect rect1(0,0,10,40);
    painter.setPen(Qt::transparent);
    painter.setBrush(QColor(200, 172, 72));
    painter.drawRect(rect1);
    painter.restore();

    painter.save();
    QRect rect2(rect1.topRight().x(), rect1.topRight().y(), geometry().width() - rect1.width(), rect1.bottomRight().y());
    painter.setPen(Qt::transparent);
    QLinearGradient gradient(rect2.topLeft(), rect2.bottomRight());
    gradient.setColorAt(0, QColor(69, 86, 107));
    gradient.setColorAt(1, QColor(37, 51, 64));
    painter.setBrush(gradient);
    painter.drawRect(rect2);
    painter.restore();

    painter.save();
    QRect rect3(rect2.topRight().x() - 30, rect2.topRight().y() + rect2.height() / 2, 4, 4);
    painter.setBrush(QColor(255, 214, 61));
    painter.setPen(Qt::transparent);
    painter.drawRect(rect3);
    painter.restore();
}
