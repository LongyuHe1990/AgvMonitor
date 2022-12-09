#include "superstratumControlView.h"
#include <QVBoxLayout>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QLineEdit>
#include <QDebug>
#include <QPushButton>
#include "moduleData/configModule.h"
#include "moduleData/stationModule.h"
#include <QFile>
#include <QListView>
#include "common/global_helper.h".h"

static SuperstratumControlView* s_superstratumControlView = nullptr;

SuperstratumControlView::SuperstratumControlView(QWidget *parent) : QWidget(parent)
{
    s_superstratumControlView = this;

    setAutoFillBackground(true);

    /*QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::transparent);
    pal.setColor(QPalette::WindowText, Qt::transparent);
    setPalette(pal);*/

    setStyleSheet("color:white; background-color:rgba(0,0,0,0)");

    initWidget();
}

SuperstratumControlView *SuperstratumControlView::getInstance()
{
    return s_superstratumControlView;
}

void SuperstratumControlView::initWidget()
{
    QString qss;
    QFile qssFile(":/qss/superstratumControlComboBox.css");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        qssFile.close();
    }

    QString buttonCss;
    QFile qssFileButton(":/qss/superstratumControlButton.css");
    qssFileButton.open(QFile::ReadOnly);
    if(qssFileButton.isOpen())
    {
        buttonCss = QLatin1String(qssFileButton.readAll());
        qssFileButton.close();
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 0, 50, 0);
    //mainLayout->setSpacing(50);

    /*QHBoxLayout* stationLayout = new QHBoxLayout(this);
    QLabel *stationLabel = new QLabel(this);
    stationLabel->setText(tr("站台名称:"));
    stationLabel->setMaximumSize(150, 50);
    //stationLabel->setFrameShape(QFrame::Box);
    stationLabel->setAlignment(Qt::AlignVCenter);
    QComboBox* stationBox = new QComboBox(this);
    stationBox->setView(new QListView());
    stationBox->setStyleSheet(qss);
    //stationBox->setStyleSheet("QComboBox{color:white; background-color:rgba(64, 66, 68); border: 1px solid gray;}");
    stationBox->setMaximumHeight(50);
    stationBox->setObjectName("StationBox");
    stationLayout->addWidget(stationLabel);
    stationLayout->addSpacing(50);
    stationLayout->addWidget(stationBox);
    mainLayout->addLayout(stationLayout);*/

    QVBoxLayout* commParmMainLayout = new QVBoxLayout(this);
    commParmMainLayout->setSpacing(1);
    QLabel* commParmTitle = new QLabel(this);
    commParmTitle->setMaximumHeight(35);
    commParmTitle->setMinimumHeight(35);
    commParmTitle->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //commParmTitle->setFrameShape(QFrame::Box);
    commParmTitle->setText(tr("命令参数"));
    QFrame *commParmFrame = new QFrame(this);
    QVBoxLayout* commParmFrameMainLayout = new QVBoxLayout(this);
    commParmFrame->setLayout(commParmFrameMainLayout);
    commParmFrame->setFrameShape(QFrame::Box);
    QGridLayout* commParmLayout = new QGridLayout(this);
    commParmLayout->setContentsMargins(0,0,50,0);
    commParmLayout->setSpacing(20);
    commParmLayout->setVerticalSpacing(100);
    for(int row = 0; row<2; row++)
    {
        for(int column = 0; column < 4; column++)
        {
            QLabel* commLabel = new QLabel(this);
            //commLabel->setFrameShape(QFrame::Box);
            commLabel->setMinimumWidth(100);
            commLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            commLabel->setText(tr("命令") + QString::number(row * 4 + (column + 1)) + ":");
            QLineEdit* commEdit = new QLineEdit(this);
            qDebug()<<"commParmEdit" + QString::number(row * 4 + column);
            commEdit->setObjectName("CommParmEdit" + QString::number(row * 4 + column));
            commEdit->setMaximumHeight(50);
            commEdit->setMinimumHeight(50);
            commParmLayout->addWidget(commLabel, row, column * 2);
            commParmLayout->addWidget(commEdit, row, column * 2 + 1);
        }
    }
    commParmFrameMainLayout->addStretch();
    commParmFrameMainLayout->addLayout(commParmLayout, 9);
    //commParmFrameMainLayout->setStretchFactor(commParmLayout, 9);
    //commParmFrameMainLayout->setGeometry();
    QHBoxLayout* uploadLayou = new QHBoxLayout(this);
    uploadLayou->setContentsMargins(0,0,0,10);
    QPushButton* uploadButton = new QPushButton(this);
    connect(uploadButton, &QPushButton::clicked, [=](){
        uploadCommParm();
    });
    uploadButton->setStyleSheet(buttonCss);
    uploadButton->setMaximumSize(130, 50);
    uploadButton->setMinimumSize(130, 50);
    uploadButton->setText(tr("上传参数"));
    uploadLayou->addWidget(uploadButton);
    commParmFrameMainLayout->addStretch();
    commParmFrameMainLayout->addLayout(uploadLayou, 1);
    //commParmFrameMainLayout->setStretchFactor(uploadLayou, 1);
    commParmMainLayout->addWidget(commParmTitle);
    commParmMainLayout->addWidget(commParmFrame);
    mainLayout->addLayout(commParmMainLayout, 1);

    QVBoxLayout* interactMainLayout = new QVBoxLayout(this);
    interactMainLayout->setSpacing(1);
    QLabel* interactLabel = new QLabel(this);
    interactLabel->setMaximumSize(150,50);
    interactLabel->setText(tr("交互步骤"));
    QFrame* interactFrame = new QFrame(this);
    interactFrame->setFrameShape(QFrame::Box);
    QVBoxLayout* interactFrameLayout = new QVBoxLayout(this);
    interactFrameLayout->setContentsMargins(50,0,50,0);
    QHBoxLayout *currentInteractLayout = new QHBoxLayout(this);
    QLabel* currentLabel = new QLabel(this);
    currentLabel->setMaximumSize(150, 50);
    currentLabel->setText(tr("当前执行步骤:"));
    currentLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QLabel* currentInteractShow = new QLabel(this);
    currentInteractShow->setFrameShape(QFrame::Box);
    currentInteractShow->setMaximumHeight(50);
    currentInteractShow->setObjectName("CurrentInteractShow");
    currentInteractShow->setStyleSheet("QLabel{color:white; background-color:rgba(64, 66, 68);}");
    QLabel* currentStateLabel = new QLabel(this);
    currentStateLabel->setStyleSheet("QLabel{color:white; background-color:rgba(64, 66, 68);}");
    currentStateLabel->setObjectName("CurrentStateLabel");
    currentStateLabel->setFrameShape(QFrame::Box);
    currentStateLabel->setMaximumSize(150, 50);
    currentStateLabel->setText(tr("执行完成"));
    currentStateLabel->setAlignment(Qt::AlignCenter);
    QPushButton* resetButton = new QPushButton(this);
    resetButton->setStyleSheet(buttonCss);
    resetButton->setMaximumSize(130, 50);
    resetButton->setMinimumSize(130, 50);
    resetButton->setText(tr("执行下一步"));
    currentInteractLayout->addWidget(currentLabel);
    currentInteractLayout->addWidget(currentInteractShow);
    currentInteractLayout->addSpacing(20);
    currentInteractLayout->addWidget(currentStateLabel);
    currentInteractLayout->addSpacing(20);
    currentInteractLayout->addWidget(resetButton);
    interactFrameLayout->addLayout(currentInteractLayout);

    QHBoxLayout* stationLayout = new QHBoxLayout(this);
    QLabel *stationLabel = new QLabel(this);
    stationLabel->setText(tr("站台名称:"));
    stationLabel->setMaximumSize(150, 50);
    //stationLabel->setFrameShape(QFrame::Box);
    stationLabel->setAlignment(Qt::AlignVCenter);
    QComboBox* stationBox = new QComboBox(this);
    connect(stationBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index){
        int id = stationBox->currentData().toInt();
        QVariantMap stationMap = StationModule::getInstance()->getStation(id);
        int stationTypeId = stationMap.value("stationTypeId").toInt();
        QVariantMap stationTypeData = ConfigModule::getInstance()->getConfig(ConfigType::StationType, stationTypeId);

        updateActionTypeBox(stationTypeId);
        updateProcessSelectBox(stationTypeData);
    });
    stationBox->setView(new QListView());
    stationBox->setStyleSheet(qss);
    //stationBox->setStyleSheet("QComboBox{color:white; background-color:rgba(64, 66, 68); border: 1px solid gray;}");
    stationBox->setMaximumHeight(50);
    stationBox->setObjectName("StationBox");
    stationLayout->addWidget(stationLabel);
    stationLayout->addWidget(stationBox);
    interactFrameLayout->addLayout(stationLayout);

    QHBoxLayout* processSelectLayout = new QHBoxLayout(this);
    QLabel* processSelectLabel = new QLabel(this);
    processSelectLabel->setText(tr("步骤选择:"));
    processSelectLabel->setMaximumSize(150, 50);
    processSelectLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QComboBox* processSelectBox = new QComboBox(this);
    processSelectBox->setView(new QListView());
    processSelectBox->setStyleSheet(qss);
    processSelectBox->setObjectName("ProcessSelectBox");
    processSelectBox->setMaximumHeight(50);
    processSelectLayout->addWidget(processSelectLabel);
    processSelectLayout->addWidget(processSelectBox);
    interactFrameLayout->addLayout(processSelectLayout);

    QHBoxLayout *actionTypeLayout = new QHBoxLayout(this);
    QLabel *actionTypeLabel = new QLabel(this);
    actionTypeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    actionTypeLabel->setText(tr("动作类型:"));
    actionTypeLabel->setMaximumSize(150, 50);
    QComboBox* actionTypeBox = new QComboBox(this);
    actionTypeBox->setView(new QListView());
    actionTypeBox->setObjectName("ActionTypeBox");
    actionTypeBox->setStyleSheet(qss);
    actionTypeBox->setMaximumHeight(50);
    actionTypeLayout->addWidget(actionTypeLabel);
    actionTypeLayout->addWidget(actionTypeBox);
    interactFrameLayout->addLayout(actionTypeLayout);
    QHBoxLayout* processOperateLayout = new QHBoxLayout(this);
    QPushButton* executeButton = new QPushButton(this);
    executeButton->setStyleSheet(buttonCss);
    executeButton->setMaximumSize(130, 50);
    executeButton->setMinimumSize(130, 50);
    executeButton->setText(tr("执行"));
    processOperateLayout->addStretch();
    processOperateLayout->addWidget(executeButton);
    processOperateLayout->addStretch();
    interactFrameLayout->addLayout(processOperateLayout);
    interactFrame->setLayout(interactFrameLayout);
    interactMainLayout->addWidget(interactLabel);
    interactMainLayout->addWidget(interactFrame);
    mainLayout->addLayout(interactMainLayout, 1);

    setLayout(mainLayout);
}

void SuperstratumControlView::initWidgetData()
{
    QVariantList stationList = StationModule::getInstance()->getStationFromStationType();

    QComboBox* combox = findChild<QComboBox*>("StationBox");
    combox->clear();
    for(int i = 0; i < stationList.size(); ++i)
    {
        QVariantMap station = stationList.at(i).toMap();
        combox->addItem(station.value("name").toString(), station.value("id").toInt());
    }
}

void SuperstratumControlView::updateCurrentInteractInfo(QVariantMap data)
{
    QString stepName = data.value("name").toString();
    QLabel* lable = findChild<QLabel*>("CurrentInteractShow");
    lable->setText(stepName);

    QLabel* executeLable = findChild<QLabel*>("CurrentStateLabel");
    QVariantMap agvStateMap = data.value("agvState").toMap();
    int executeState = agvStateMap.value("lastestExecuteStatus").toInt();
    if(-1 == executeState)
    {
        executeLable->setText("命令未接收");
    }
    else if(0 == executeState)
    {
        executeLable->setText("命令未执行");
    }
    else if(1 == executeState)
    {
        executeLable->setText("命令执行中");
    }
    else if(2 == executeState)
    {
        executeLable->setText("命令执行完成");
    }
}

void SuperstratumControlView::updateProcessSelectBox(QVariantMap stationTypeData)
{
    if(stationTypeData.find("interactionId") != stationTypeData.end())
    {
        int interactionId = stationTypeData.value("interactionId").toInt();
        QVariantMap interactMap = ConfigModule::getInstance()->getConfig(ConfigType::StationInteraction, interactionId);
        QComboBox* processSelectBox = findChild<QComboBox*>("ProcessSelectBox");
        processSelectBox->clear();
        QVariantList loadInteractionStepList = interactMap.value("loadInteractionStepList").toList();
        for(int i = 0; i < loadInteractionStepList.size(); ++i)
        {
            QVariantMap stepItem = loadInteractionStepList.at(i).toMap();
            QString name = stepItem.value("name").toString();
            int step = stepItem.value("step").toInt();
            int type = stepItem.value("type").toInt();
            if(2 == type || 3 == type)
            {
                processSelectBox->addItem(name, step);
            }
        }

        QVariantList unloadInteractionStepList = interactMap.value("unloadInteractionStepList").toList();
        for(int i = 0; i < unloadInteractionStepList.size(); ++i)
        {
            QVariantMap stepItem = unloadInteractionStepList.at(i).toMap();
            QString name = stepItem.value("name").toString();
            int step = stepItem.value("step").toInt();
            int type = stepItem.value("type").toInt();
            if(2 == type || 3 == type)
            {
                processSelectBox->addItem(name, step);
            }
        }
    }

}

void SuperstratumControlView::updateActionTypeBox(int stationTypeId)
{
    QVariantList actionList = ConfigModule::getInstance()->getActionTypeOfStationType(stationTypeId);
    QComboBox* actionBox = findChild<QComboBox*>("ActionTypeBox");
    actionBox->clear();
    for(int i = 0; i < actionList.size(); ++i)
    {
        int actionId = actionList.at(i).toInt();
        NameAndLabelInfo actionInfo = GetActionType(actionId);
        actionBox->addItem(actionInfo.name, actionId);
    }
}

void SuperstratumControlView::uploadCommParm()
{
    getCommParmList();
}

QVariantList SuperstratumControlView::getCommParmList()
{
    QVariantList parmList;
    for(int i = 0; i<8; ++i)
    {
        QLineEdit* edit = findChild<QLineEdit*>("CommParmEdit" + QString::number(i));
        QString data = edit->text();
        parmList.push_back(data);
    }

    return parmList;
}

int SuperstratumControlView::getStationIdFromBox()
{
    QComboBox* box = findChild<QComboBox*>("StationBox");
    int id = box->currentData().toInt();
    return id;
}

int SuperstratumControlView::getActionIdFromBox()
{
    QComboBox* box = findChild<QComboBox*>("ActionTypeBox");
    int id = box->currentData().toInt();
    return id;
}

int SuperstratumControlView::getProcessStepFromBox()
{
    QComboBox* box = findChild<QComboBox*>("ProcessSelectBox");
    int step = box->currentData().toInt();
    return step;
}

int SuperstratumControlView::getProcessIdByStationId(int stationId)
{
    QVariantMap stationMap = StationModule::getInstance()->getStation(stationId);
    int stationTypeId = stationMap.value("stationTypeId").toInt();
    QVariantMap stationTypeData = ConfigModule::getInstance()->getConfig(ConfigType::StationType, stationTypeId);
    int interactionId = 0;
    if(stationTypeData.find("interactionId") != stationTypeData.end())
    {
        interactionId = stationTypeData.value("interactionId").toInt();
    }

    return interactionId;
}
